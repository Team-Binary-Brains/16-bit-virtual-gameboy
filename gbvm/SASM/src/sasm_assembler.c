#include "sasm_assembler.h"
#include "univ_fileops.h"

void pushScope(Sasm* sasm, Scope* scope)
{
    assert(scope->previous == NULL);
    scope->previous = sasm->scope;
    sasm->scope = scope;
}

void createAndPushScope(Sasm* sasm)
{
    Scope* scope = allocateRegion(&sasm->region, sizeof(*sasm->scope));
    pushScope(sasm, scope);
}

void popScope(Sasm* sasm)
{
    assert(sasm->scope != NULL);
    sasm->scope = sasm->scope->previous;
}

bool resolveIncludeFilePath(Sasm* sasm, String filePath, String* resolvedPath)
{
    for (size_t i = 0; i < sasm->includePathsCnt; ++i) {
        String path = appendToPath(&sasm->region, sasm->includePaths[i],
            filePath);
        if (doesFileExist(convertRegionStrtoCStr(&sasm->region, path))) {
            if (resolvedPath) {
                *resolvedPath = path;
            }
            return true;
        }
    }

    return false;
}

void translateSasmEntryDirective(Sasm* sasm, EntryStmt entry, FileLocation location)
{
    assert(sasm->scope);

    if (sasm->deferredEntry.bindingName.length > 0) {
        fprintf(stderr,
            FLFmt ": ERROR: entry point has been already set within the same scope!\n",
            FLArg(location));
        fprintf(stderr, FLFmt ": NOTE: the first entry point\n",
            FLArg(sasm->deferredEntry.location));
        exit(1);
    }

    if (entry.value.type != EXPR_BINDING) {
        fprintf(stderr, FLFmt ": ERROR: only bindings are allowed to be set as entry points for now.\n",
            FLArg(location));
        exit(1);
    }

    String label = entry.value.value.binding;
    sasm->deferredEntry.bindingName = label;
    sasm->deferredEntry.location = location;
    sasm->deferredEntry.scope = sasm->scope;
}

void translateSasmIncludeDirective(Sasm* sasm, IncludeStmt include, FileLocation location)
{
    String resolved_path = (String) { 0 };
    if (resolveIncludeFilePath(sasm, include.path, &resolved_path)) {
        include.path = resolved_path;
    }

    FileLocation prev_includeLocation = sasm->includeLocation;
    sasm->includeLevel += 1;
    sasm->includeLocation = location;
    translateSasmFile(sasm, include.path);
    sasm->includeLocation = prev_includeLocation;
    sasm->includeLevel -= 1;
}

void translateSasmBindDirective(Sasm* sasm, ConstStmt konst, FileLocation location)
{
    assert(sasm->scope != NULL);
    bindExprLocalScope(sasm->scope, konst.name, konst.value, location);
}

void translateSasmInstruction(Sasm* sasm, InstStmt inst, FileLocation location)
{
    assert(sasm $instructionCount < PROGRAM_CAPACITY);
    sasm $instructions[sasm $instructionCount].type = inst.type;
    sasm $instructions[sasm $instructionCount].operand.u64 = 0;
    OpcodeDetails details = getOpcodeDetails(inst.type);
    if (details.has_operand) {
        pushUnresolvedOperand(sasm, sasm $instructionCount, inst.operand, location);
    }
    if (details.has_operand2) {
        pushUnresolvedOperand(sasm, sasm $instructionCount, inst.operand2, location);
    }
    // printf("%s %d %d %ld %ld\n", details.name, inst.operand.type, inst.operand2.type, inst.operand.value.lit_int, inst.operand2.value.lit_int);

    sasm $instructionCount += 1;
}

void translateSasmStatementChain(Sasm* sasm, StmtNode* block)
{
    for (StmtNode* iter = block; iter != NULL; iter = iter->next) {
        Stmt statement = iter->statement;
        switch (statement.kind) {
        case STMT_LABEL:
            bindUnresolvedLocalScope(sasm->scope, statement.value.label.name, BIND_TYPE_INST_ADDR, statement.location);
            break;
        case STMT_CONST:
            translateSasmBindDirective(sasm, statement.value.constant, statement.location);
            break;
        case STMT_INCLUDE:
            translateSasmIncludeDirective(sasm, statement.value.include, statement.location);
            break;
        case STMT_ENTRY:
            translateSasmEntryDirective(sasm, statement.value.entry, statement.location);
            break;
        case STMT_BLOCK:
            translateSasmStatementChain(sasm, statement.value.block);
            break;

        case STMT_SCOPE:
        case STMT_INST:
            break;

        default:
            assert(false && "translate: unreachable");
            exit(1);
        }
    }
    for (StmtNode* iter = block; iter != NULL; iter = iter->next) {
        Stmt statement = iter->statement;
        switch (statement.kind) {
        case STMT_INST:
            translateSasmInstruction(sasm, statement.value.inst, statement.location);
            break;

        case STMT_LABEL:
            Binding* binding = resolveBinding(sasm, statement.value.label.name);
            assert(binding != NULL);
            assert(binding->status == BIND_STATUS_DEFERRED);

            binding->status = BIND_STATUS_EVALUATED;
            binding->value.u64 = sasm $instructionCount;
            break;

        case STMT_SCOPE:
            createAndPushScope(sasm);
            translateSasmStatementChain(sasm, statement.value.scope);
            popScope(sasm);
            break;

        case STMT_BLOCK:
        case STMT_ENTRY:
        case STMT_INCLUDE:
        case STMT_CONST:
            break;

        default:
            assert(false && "translate: unreachable");
            exit(1);
        }
    }
}

void resolveAllUnresolvedOperands(Sasm* sasm)
{
    Scope* savedScope = sasm->scope;

    for (size_t i = 0; i < sasm->symbolsCount; ++i) {
        assert(sasm->symbols[i].scope);
        sasm->scope = sasm->symbols[i].scope;

        InstAddr addr = sasm->symbols[i].addr;
        Expr expr = sasm->symbols[i].expr;
        FileLocation location = sasm->symbols[i].location;

        EvalResult result = evaluateExpression(sasm, expr, location);
        assert(result.status == EVAL_STATUS_OK);
        sasm $instructions[addr].operand = result.value;
        if (expr.type == EXPR_FUNCALL && expr.value.funcall->args->value.type == EXPR_REG) {
            sasm $instructions[addr].opr1IsReg = true;
        }

        OpcodeDetails inst_def = getOpcodeDetails(sasm $instructions[addr].type);
        assert(inst_def.has_operand);

        if (inst_def.has_operand2) {
            i++;
            Expr expr2 = sasm->symbols[i].expr;
            EvalResult result2 = evaluateExpression(sasm, expr2, location);
            sasm $instructions[addr].operand2 = result2.value;
            if (expr.type == EXPR_FUNCALL && expr.value.funcall->args->value.type == EXPR_REG) {
                sasm $instructions[addr].opr2IsReg = true;
            }
        }
    }

    sasm->scope = savedScope;
}

void resolveProgramEntryPoint(Sasm* sasm)
{
    Scope* savedScope = sasm->scope;
    if (sasm->deferredEntry.bindingName.length > 0) {
        assert(sasm->deferredEntry.scope);
        sasm->scope = sasm->deferredEntry.scope;

        if (sasm->hasEntry) {
            fprintf(stderr,
                FLFmt ": ERROR: entry point has been already set!\n",
                FLArg(sasm->deferredEntry.location));
            fprintf(stderr, FLFmt ": NOTE: the first entry point\n",
                FLArg(sasm->entryLocation));
            exit(1);
        }

        Binding* binding = resolveBinding(
            sasm,
            sasm->deferredEntry.bindingName);
        if (binding == NULL) {
            fprintf(stderr, FLFmt ": ERROR: unknown binding `" strFmt "`\n",
                FLArg(sasm->deferredEntry.location),
                strArg(sasm->deferredEntry.bindingName));
            exit(1);
        }

        if (binding->type != BIND_TYPE_INST_ADDR) {
            fprintf(stderr, FLFmt ": ERROR: Type check error. Trying to set `" strFmt "` that has the type of %s as an entry point. Entry point has to be %s.\n",
                FLArg(sasm->deferredEntry.location),
                strArg(binding->name),
                getNameOfBindType(binding->type),
                getNameOfBindType(BIND_TYPE_INST_ADDR));
            exit(1);
        }

        EvalResult result = evaluateBinding(sasm, binding);
        assert(result.status == EVAL_STATUS_OK);

        sasm->entry = result.value.u64;
        sasm->hasEntry = true;
        sasm->entryLocation = sasm->deferredEntry.location;
    }

    sasm->scope = savedScope;
}

void translateSasmRootFile(Sasm* sasm, String inputFilePath)
{
    createAndPushScope(sasm);
    translateSasmFile(sasm, inputFilePath);
    popScope(sasm);

    resolveAllUnresolvedOperands(sasm);
    resolveProgramEntryPoint(sasm);
}

void generateSmExecutable(Sasm* sasm, const char* filePath)
{
    FILE* f = openFile(filePath, "wb");

    Metadata meta = {
        .magic = FILE_MAGIC,
        .version = FILE_VERSION,
        .entry = sasm->entry,
        .programSize = sasm $instructionCount,
        .memorySize = sasm->memorySize,
        .memoryCapacity = sasm->memoryCapacity,
    };

    fwrite(&meta, sizeof(meta), 1, f);
    if (ferror(f)) {
        fileErrorDispWithExit("Could not write to file", filePath);
    }

    fwrite(sasm $instructions, sizeof(sasm $instructions[0]), sasm $instructionCount, f);
    if (ferror(f)) {
        fileErrorDispWithExit("Could not write to file", filePath);
    }

    fwrite(sasm->memory, sizeof(sasm->memory[0]), sasm->memorySize, f);
    if (ferror(f)) {
        fileErrorDispWithExit("Could not write to file", filePath);
    }

    closeFile(f, filePath);
}

void translateSasmFile(Sasm* sasm, String inputFilePath)
{
    SasmLexer SasmLexer = { 0 };

    if (!loadSasmFileIntoSasmLexer(&SasmLexer, &sasm->region, inputFilePath)) {

        if (sasm->includeLevel > 0)
            fprintf(stderr, FLFmt, FLArg(sasm->includeLocation));

        fileErrorDispWithExit("Could not read file", inputFilePath.data);
    }
    CodeBlock inputFileBlock = getCodeBlockFromLines(&sasm->region, &SasmLexer);
    translateSasmStatementChain(sasm, inputFileBlock.begin);
    generateASTPng(inputFilePath, inputFileBlock.begin);
}

void loadSmExecutableIntoSasm(Sasm* sasm, const char* filePath)
{
    memset(sasm, 0, sizeof(*sasm));

    FILE* f = openFile(filePath, "rb");

    Metadata meta = { 0 };

    size_t n = fread(&meta, sizeof(meta), 1, f);
    if (n < 1) {
        fileErrorDispWithExit("Could not read meta data from file", filePath);
    }

    if (meta.magic != FILE_MAGIC) {
        fprintf(stderr, "Unexpected magic %04X. Expected %04X.\n", meta.magic, FILE_MAGIC);
        fileErrorDispWithExit("Not a valid SASM File ", filePath);
    }

    if (meta.version != FILE_VERSION) {
        fprintf(stderr, "Encountered version %d. Expected version %d.\n", meta.version, FILE_VERSION);
        fileErrorDispWithExit("unsupported version of SASM File ", filePath);
    }

    if (meta.programSize > PROGRAM_CAPACITY) {
        fprintf(stderr,
            "The file contains %" PRIu64 " program instruction. But the capacity is %" PRIu64 "\n",
            meta.programSize, (uint64_t)PROGRAM_CAPACITY);
        fileErrorDispWithExit("program section is too big ", filePath);
    }

    if (meta.memoryCapacity > MEMORY_CAPACITY) {
        fprintf(stderr,
            "The file wants %" PRIu64 " bytes. But the capacity is %" PRIu64 " bytes\n",
            meta.memoryCapacity, (uint64_t)MEMORY_CAPACITY);
        fileErrorDispWithExit(" memory section is too big ", filePath);
    }

    if (meta.memorySize > meta.memoryCapacity) {
        fprintf(stderr,
            "ERROR: %s: memory size %" PRIu64 " is greater than declared memory capacity %" PRIu64 "\n",
            filePath, meta.memorySize, meta.memoryCapacity);
        exit(1);
    }

    if (meta.externalsSize > EXTERNAL_VMCALLS_CAPACITY) {
        fprintf(stderr,
            "ERROR: %s: external names section is too big. The file contains %" PRIu64 " external names. But the capacity is %" PRIu64 " external names\n",
            filePath, meta.externalsSize, (uint64_t)EXTERNAL_VMCALLS_CAPACITY);
        exit(1);
    }

    sasm $instructionCount = fread(sasm $instructions, sizeof(sasm $instructions[0]), meta.programSize, f);

    if (sasm $instructionCount != meta.programSize) {
        fprintf(stderr, "ERROR: %s: read %" PRIu64 " program instructions, but expected %" PRIu64 "\n",
            filePath, sasm $instructionCount, meta.programSize);
        exit(1);
    }

    n = fread(sasm->memory, sizeof(sasm->memory[0]), meta.memorySize, f);

    if (n != meta.memorySize) {
        fprintf(stderr, "ERROR: %s: read %zd bytes of memory section, but expected %" PRIu64 " bytes.\n",
            filePath, n, meta.memorySize);
        exit(1);
    }

    closeFile(f, filePath);
}

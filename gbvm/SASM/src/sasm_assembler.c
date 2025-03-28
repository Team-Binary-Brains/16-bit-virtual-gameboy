#include "sasm_assembler.h"
#include "univ_malloc.h"
#include "univ_strings.h"
#include "univ_fileops.h"

Error bindDirective(Sasm* sasm, String name, String* line)
{
    *line = trim(*line);
    String value = *line;
    QuadWord word = { 0 };
    if (!translateLiteral(sasm, value, &word)) {
        fprintf(stderr, " ERROR: `" str_Fmt "` is not a number", str_Arg(value));
        return ERR_NAN;
    }

    if (!bindValue(sasm, name, word)) {
        fprintf(stderr, " ERROR: name `" str_Fmt "` is already bound\n", str_Arg(name));
        return ERR_ALREADY_BOUND;
    }
    return ERR_OK;
}

Error includeDirective(Sasm* sasm, String* line)
{
    if (*line->data != '"' || line->data[line->length - 1] != '"') {
        fprintf(stderr,
            " ERROR: include file path has to be surrounded with quotation marks\n");
        exit(1);
    }

    line->data += 1;
    line->length -= 2;

    if (sasm->includeLevel + 1 >= MAX_INCLUDE_LEVEL) {
        fprintf(stderr, " ERROR: exceeded maximum include level\n");
        exit(1);
    }

    sasm->includeLevel += 1;
    parseAsmIntoProgram(sasm, *line);
    sasm->includeLevel -= 1;

    return ERR_OK;
}

Error processPreProcessorDirective(Sasm* sasm, String* line, String token)
{
    *line = trim(*line);
    if (strEqu(token, cstrToStr("bind"))) {
        String name = splitStr(line, ' ');
        if (name.length <= 0) {
            displayMsgWithExit(" ERROR: binding name is not provided\n");
        }
        return bindDirective(sasm, name, line);
    }

    if (strEqu(token, cstrToStr("include"))) {
        if (line->length <= 0) {
            displayMsgWithExit(" ERROR: include file path is not provided\n");
        }
        return includeDirective(sasm, line);
    }

    fprintf(stderr,
        " ERROR: unknown pre-processor directive `" str_Fmt "`\n",
        str_Arg(token));
    exit(1);
}

bool bindValue(Sasm* sasm, String name, QuadWord value)
{
    assert(sasm->bindingCount < BINDINGS_CAPACITY);

    QuadWord ignore = { 0 };
    if (resolveBinding(sasm, name, &ignore)) {
        return false;
    }

    sasm->bindings[sasm->bindingCount++] = (Binding) { .name = name, .value = value };
    return true;
}

void pushLabel(Sasm* sasm, InstAddr addr, String name)
{
    assert(sasm->LabelsCount < LABELS_CAPACITY);
    sasm->Labels[sasm->LabelsCount++] = (Label) { .addr = addr, .name = name };
}

QuadWord pushStringToMemory(Sasm* sasm, String str)
{
    assert(sasm->memorySize + str.length <= MEMORY_CAPACITY);

    QuadWord result = quadword_u64(sasm->memorySize);
    memcpy(sasm->memory + sasm->memorySize, str.data, str.length);
    sasm->memorySize += str.length;

    if (sasm->memorySize > sasm->memoryCapacity) {
        sasm->memoryCapacity = sasm->memorySize;
    }

    return result;
}

bool translateLiteral(Sasm* sasm, String str, QuadWord* output)
{
    if (str.length >= 2 && *str.data == '"' && str.data[str.length - 1] == '"') {
        str.data += 1;
        str.length -= 2;
        *output = pushStringToMemory(sasm, str);
        return true;
    }

    const char* cstr = regionStrToCstr(&sasm->region, str);
    char* endptr = 0;
    QuadWord result = { 0 };

    result.as_u64 = strtoull(cstr, &endptr, 10);
    if ((size_t)(endptr - cstr) != str.length) {

        result.as_f64 = strtod(cstr, &endptr);

        if ((size_t)(endptr - cstr) != str.length)
            return false;
    }

    *output = result;
    return true;
}

bool resolveBinding(const Sasm* sasm, String name, QuadWord* output)
{
    for (size_t i = 0; i < sasm->bindingCount; ++i) {
        if (strEqu(sasm->bindings[i].name, name)) {
            *output = sasm->bindings[i].value;
            return true;
        }
    }

    return false;
}

void assembleProgramIntoBytecode(Sasm* sasm, const char* filePath)
{
    FILE* f = openFile(filePath, "wb");

    Metadata meta = {
        .magic = FILE_MAGIC,
        .version = FILE_VERSION,
        .programSize = sasm->prog.instruction_count,
        .memorySize = sasm->memorySize,
        .memoryCapacity = sasm->memoryCapacity,
    };

    fwrite(&meta, sizeof(meta), 1, f);
    if (ferror(f)) {
        fprintf(stderr, "ERROR: Could not write to file `%s`: %s\n",
            filePath, strerror(errno));
        exit(1);
    }

    fwrite(sasm->prog.instructions, sizeof(sasm->prog.instructions[0]), sasm->prog.instruction_count, f);
    if (ferror(f)) {
        fprintf(stderr, "ERROR: Could not write to file `%s`: %s\n",
            filePath, strerror(errno));
        exit(1);
    }

    fwrite(sasm->memory, sizeof(sasm->memory[0]), sasm->memorySize, f);
    if (ferror(f)) {
        fprintf(stderr, "ERROR: Could not write to file `%s`: %s\n",
            filePath, strerror(errno));
        exit(1);
    }

    closeFile(f, filePath);
}

Error processLine(Sasm* sasm, String* line)
{
    String token = trim(splitStr(line, ' '));

    if (token.length > 0 && *token.data == PREP_SYMBOL) {
        token.length -= 1;
        token.data += 1;
        return processPreProcessorDirective(sasm, line, token);
    }

    if (token.length > 0 && token.data[token.length - 1] == ':') {
        String label = {
            .length = token.length - 1,
            .data = token.data
        };

        if (!bindValue(sasm, label, quadword_u64(sasm->prog.instruction_count))) {
            fprintf(stderr, " ERROR: name `" str_Fmt "` is already bound to something\n", str_Arg(label));
            return ERR_ALREADY_BOUND;
        }

        token = trim(splitStr(line, ' '));
    }

    if (token.length <= 0) {
        return ERR_OK;
    }

    String operand;
    OpcodeDetails details;
    if (strAsOpcode(token, &details)) {
        assert(sasm->prog.instruction_count < PROGRAM_CAPACITY);
        Instruction* inst = &sasm->prog.instructions[sasm->prog.instruction_count];
        inst->type = details.type;

        operand = trim(splitStr(line, ','));
        if (details.has_operand) {
            // printString(operand);
            if (operand.length == 0) {
                fprintf(stderr, " ERROR: instruction `" str_Fmt "` requires an operand\n",
                    str_Arg(token));
                exit(1);
            }
            if (!translateLiteral(
                    sasm,
                    operand, &inst->operand)) {
                pushLabel(
                    sasm, sasm->prog.instruction_count, operand);
            }
        }

        operand = trim(splitStr(line, ' '));
        operand.data += 1;
        operand.length -= 1;
        if (details.has_operand2) {
            if (operand.length == 0) {
                fprintf(stderr, " ERROR: instruction `" str_Fmt "` requires 2 operands\n",
                    str_Arg(token));
                exit(1);
            }
            if (!translateLiteral(
                    sasm,
                    operand, &inst->operand2)) {
                pushLabel(
                    sasm, sasm->prog.instruction_count, operand);
            }
        }

        sasm->prog.instruction_count += 1;

    } else {
        fprintf(stderr, " ERROR: unknown instruction `" str_Fmt "`\n",
            str_Arg(token));
        exit(1);
    }

    return ERR_OK;
}

void parseAsmIntoProgram(Sasm* sasm, String inputFilePath)
{
    String original_source;
    regionSlurpFile(&sasm->region, inputFilePath, &original_source);
    String source = original_source;

    int line_number = 0;

    while (source.length > 0) {
        String line = trim(splitStr(&source, '\n'));
        line = trim(splitStr(&line, COMMENT_SYMBOL));
        // printString(line);
        line_number += 1;
        if (line.length > 0) {
            if (processLine(sasm, &line) != ERR_OK) {
                displayErrorDetailsWithExit(inputFilePath, line_number);
            }
        }
    }

    for (size_t i = 0; i < sasm->LabelsCount; ++i) {
        String name = sasm->Labels[i].name;
        if (!resolveBinding(sasm, name,
                &sasm->prog.instructions[sasm->Labels[i].addr].operand)) {
            fprintf(stderr, str_Fmt ": ERROR: unknown binding `" str_Fmt "`\n",
                str_Arg(inputFilePath), str_Arg(name));
            exit(1);
        }
    }
}

void loadProgramIntoSasm(Sasm* sasm, const char* file_path)
{
    memset(sasm, 0, sizeof(*sasm));

    FILE* f = fopen(file_path, "rb");
    if (f == NULL) {
        fprintf(stderr, "ERROR: Could not open file `%s`: %s\n",
            file_path, strerror(errno));
        exit(1);
    }

    Metadata meta = { 0 };

    size_t n = fread(&meta, sizeof(meta), 1, f);
    if (n < 1) {
        fprintf(stderr, "ERROR: Could not read meta data from file `%s`: %s\n",
            file_path, strerror(errno));
        exit(1);
    }

    if (meta.magic != FILE_MAGIC) {
        fprintf(stderr,
            "ERROR: %s does not appear to be a valid sasm file. "
            "Unexpected magic %04X. Expected %04X.\n",
            file_path,
            meta.magic, FILE_MAGIC);
        exit(1);
    }

    if (meta.version != FILE_VERSION) {
        fprintf(stderr,
            "ERROR: %s: unsupported version of sasm file %d. Expected version %d.\n",
            file_path,
            meta.version, FILE_VERSION);
        exit(1);
    }

    if (meta.programSize > PROGRAM_CAPACITY) {
        fprintf(stderr,
            "ERROR: %s: program section is too big. The file contains %" PRIu64 " program instruction. But the capacity is %" PRIu64 "\n",
            file_path,
            meta.programSize,
            (uint64_t)PROGRAM_CAPACITY);
        exit(1);
    }

    if (meta.memoryCapacity > MEMORY_CAPACITY) {
        fprintf(stderr,
            "ERROR: %s: memory section is too big. The file wants %" PRIu64 " bytes. But the capacity is %" PRIu64 " bytes\n",
            file_path,
            meta.memoryCapacity,
            (uint64_t)MEMORY_CAPACITY);
        exit(1);
    }

    if (meta.memorySize > meta.memoryCapacity) {
        fprintf(stderr,
            "ERROR: %s: memory size %" PRIu64 " is greater than declared memory capacity %" PRIu64 "\n",
            file_path,
            meta.memorySize,
            meta.memoryCapacity);
        exit(1);
    }

    if (meta.externalsSize > EXTERNAL_VMCALLS_CAPACITY) {
        fprintf(stderr,
            "ERROR: %s: external names section is too big. The file contains %" PRIu64 " external names. But the capacity is %" PRIu64 " external names\n",
            file_path,
            meta.externalsSize,
            (uint64_t)EXTERNAL_VMCALLS_CAPACITY);
        exit(1);
    }

    sasm->prog.instruction_count = fread(sasm->prog.instructions, sizeof(sasm->prog.instructions[0]), meta.programSize, f);

    if (sasm->prog.instruction_count != meta.programSize) {
        fprintf(stderr, "ERROR: %s: read %" PRIu64 " program instructions, but expected %" PRIu64 "\n",
            file_path,
            sasm->prog.instruction_count,
            meta.programSize);
        exit(1);
    }

    n = fread(sasm->memory, sizeof(sasm->memory[0]), meta.memorySize, f);

    if (n != meta.memorySize) {
        fprintf(stderr, "ERROR: %s: read %zd bytes of memory section, but expected %" PRIu64 " bytes.\n",
            file_path,
            n,
            meta.memorySize);
        exit(1);
    }

    fclose(f);
}
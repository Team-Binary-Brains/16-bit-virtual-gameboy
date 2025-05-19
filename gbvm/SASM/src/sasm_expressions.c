#include "sasm_assembler.h"

QuadWord pushStringToMemory(Sasm* sasm, String str)
{
    assert(sasm->memorySize + str.length <= MEMORY_CAPACITY);

    QuadWord result = quadwordFromU64(sasm->memorySize);
    memcpy(sasm->memory + sasm->memorySize, str.data, str.length);
    sasm->memorySize += str.length;

    if (sasm->memorySize > sasm->memoryCapacity) {
        sasm->memoryCapacity = sasm->memorySize;
    }

    sasm->stringLens[sasm->strLensCnt++] = (StringLength) {
        .addr = result.u64,
        .length = str.length,
    };

    return result;
}

bool getStrLenByAddr(Sasm* sasm, InstAddr addr, QuadWord* length)
{
    for (size_t i = 0; i < sasm->strLensCnt; ++i) {
        if (sasm->stringLens[i].addr == addr) {
            if (length) {
                *length = quadwordFromU64(sasm->stringLens[i].length);
            }
            return true;
        }
    }

    return false;
}

FuncallArg* parseFuncallArgs(Region* region, Tokenizer* tokenizer, FileLocation location)
{
    Token token = { 0 };

    if (!moveSasmTokenizerToNextToken(tokenizer, &token, location) || token.type != TOKEN_TYPE_OPEN_PAREN) {
        fprintf(stderr, FLFmt ": ERROR: expected %s\n",
            FLArg(location),
            getTokenName(TOKEN_TYPE_OPEN_PAREN));
        exit(1);
    }

    if (fetchCachedSasmTokenFromSasmTokenizer(tokenizer, &token, location) && token.type == TOKEN_TYPE_CLOSING_PAREN) {
        moveSasmTokenizerToNextToken(tokenizer, NULL, location);
        return NULL;
    }

    FuncallArg* first = NULL;
    FuncallArg* last = NULL;

    do {
        FuncallArg* arg = allocateRegion(region, sizeof(FuncallArg));
        arg->value = parsePrimaryOfSasmTokens(region, tokenizer, location);

        if (first == NULL) {
            first = arg;
            last = arg;
        } else {
            last->next = arg;
            last = arg;
        }

        if (!moveSasmTokenizerToNextToken(tokenizer, &token, location)) {
            fprintf(stderr, FLFmt ": ERROR: expected %s or %s\n",
                FLArg(location),
                getTokenName(TOKEN_TYPE_CLOSING_PAREN),
                getTokenName(TOKEN_TYPE_COMMA));
            exit(1);
        }
    } while (token.type == TOKEN_TYPE_COMMA);

    if (token.type != TOKEN_TYPE_CLOSING_PAREN) {
        fprintf(stderr, FLFmt ": ERROR: expected %s\n",
            FLArg(location),
            getTokenName(TOKEN_TYPE_CLOSING_PAREN));
        exit(1);
    }

    return first;
}

size_t getFunCallArgCnt(FuncallArg* args)
{
    size_t result = 0;
    while (args != NULL) {
        result += 1;
        args = args->next;
    }
    return result;
}

void checkFuncArgs(Funcall* funcall, size_t expected_arity, FileLocation location)
{
    const size_t actual_arity = getFunCallArgCnt(funcall->args);
    if (actual_arity != expected_arity) {
        fprintf(stderr, FLFmt ": ERROR: " strFmt "() expects %zu but got %zu",
            FLArg(location),
            strArg(funcall->name),
            expected_arity,
            actual_arity);
        exit(1);
    }
}

EvalResult resolveFuncall(Sasm* sasm, Expr expr, FileLocation location)
{
    if (compareStr(expr.value.funcall->name, convertCstrToStr("len"))) {
        checkFuncArgs(expr.value.funcall, 1, location);

        QuadWord addr = { 0 };
        EvalResult result = evaluateExpression(
            sasm,
            expr.value.funcall->args->value,
            location);
        if (result.status == EVAL_STATUS_DEFERRED) {
            return result;
        }
        addr = result.value;
        QuadWord length = { 0 };
        if (!getStrLenByAddr(sasm, addr.u64, &length)) {
            fprintf(stderr, FLFmt ": ERROR: Could not compute the length of string at address %" PRIu64 "\n", FLArg(location), addr.u64);
            exit(1);
        }

        return resultOK(length, BIND_TYPE_UINT);
    }
    if (compareStr(expr.value.funcall->name, convertCstrToStr("res"))) {
        checkFuncArgs(expr.value.funcall, 1, location);

        QuadWord addr = { 0 };
        EvalResult result = evaluateExpression(
            sasm,
            expr.value.funcall->args->value,
            location);

        assert(sasm->memorySize + result.value.u64 <= MEMORY_CAPACITY);

        addr = quadwordFromU64(sasm->memorySize);
        sasm->memorySize += result.value.u64;

        if (sasm->memorySize > sasm->memoryCapacity) {
            sasm->memoryCapacity = sasm->memorySize;
        }

        return resultOK(addr, BIND_TYPE_UINT);
    }
    if (compareStr(expr.value.funcall->name, convertCstrToStr("ref"))) {
        checkFuncArgs(expr.value.funcall, 1, location);

        if (expr.value.funcall->args->value.type != EXPR_REG) {
            fprintf(stderr, FLFmt ": ERROR: ref expects a register ", FLArg(location));
        }

        EvalResult result = evaluateExpression(
            sasm,
            expr.value.funcall->args->value,
            location);
        return result;
    }
    if (compareStr(expr.value.funcall->name, convertCstrToStr("val"))) {
        checkFuncArgs(expr.value.funcall, 1, location);

        if (expr.value.funcall->args->value.type != EXPR_REG) {
            fprintf(stderr, FLFmt ": ERROR: val expects a register ", FLArg(location));
        }

        EvalResult result = evaluateExpression(
            sasm,
            expr.value.funcall->args->value,
            location);

        if (result.status == EVAL_STATUS_DEFERRED) {
            return result;
        }
        uint64_t val = result.value.u64 + REG_COUNT;
        return resultOK(
            quadwordFromU64(val), BIND_TYPE_UINT);
    }
    displayErrorDetailsWithExit(location, "Unknown translation time function", expr.value.funcall->name);
    exit(1);
}

EvalResult resultOK(QuadWord value, BindingType type)
{
    return (EvalResult) {
        .status = EVAL_STATUS_OK,
        .value = value,
        .type = type,
    };
}

EvalResult resultUnresolved(Binding* unresolvedBinding)
{
    return (EvalResult) {
        .status = EVAL_STATUS_DEFERRED,
        .unresolvedBinding = unresolvedBinding
    };
}

EvalResult evaluateExpression(Sasm* sasm, Expr expr, FileLocation location)
{
    switch (expr.type) {
    case EXPR_LIT_INT:
        return resultOK(
            quadwordFromU64(expr.value.lit_int),
            BIND_TYPE_UINT);

    case EXPR_LIT_FLOAT:
        return resultOK(
            quadwordFromF64(expr.value.lit_float),
            BIND_TYPE_FLOAT);

    case EXPR_LIT_CHAR:
        return resultOK(
            quadwordFromU64(expr.value.lit_char),
            BIND_TYPE_UINT);

    case EXPR_LIT_STR:
        return resultOK(
            pushStringToMemory(sasm, expr.value.lit_str),
            BIND_TYPE_MEM_ADDR);

    case EXPR_FUNCALL:
        return resolveFuncall(sasm, expr, location);

    case EXPR_BINDING:

        String name = expr.value.binding;
        Binding* binding = resolveBinding(sasm, name);
        if (binding == NULL) {
            fprintf(stderr, FLFmt ": ERROR: couldnt find binding `" strFmt "`.\n",
                FLArg(location), strArg(name));
            exit(1);
        }

        return evaluateBinding(sasm, binding);

    case EXPR_REG:
        return resultOK(
            quadwordFromU64(expr.value.reg_id),
            BIND_TYPE_UINT);

    default:
        assert(false && "evaluateExpression: unreachable");
        exit(1);
    }
    exit(1);
}

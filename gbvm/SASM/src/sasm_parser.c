#include "sasm_assembler.h"

void confirmNoMoreTokensRemain(Tokenizer* tokenizer, FileLocation location)
{
    Token token = { 0 };
    if (moveSasmTokenizerToNextToken(tokenizer, &token, location)) {
        fprintf(stderr, FLFmt ": ERROR: unexpected token `" strFmt "`\n",
            FLArg(location), strArg(token.text));
        exit(1);
    }
}

void pushStatementIntoBlock(Region* region, CodeBlock* list, Stmt statement)
{
    assert(list);
    StmtNode* stmtNode = allocateRegion(region, sizeof(StmtNode));
    stmtNode->statement = statement;

    if (list->end == NULL) {
        assert(list->begin == NULL);
        list->begin = stmtNode;
        list->end = stmtNode;
    } else {
        assert(list->begin != NULL);
        list->end->next = stmtNode;
        list->end = stmtNode;
    }
    // printf("%d %d",list->end->statement.kind,list->end==NULL);
    // printString(list->end->statement.value.as_bind_label.name);
}

Expr parseExprFromStr(Region* region, String source, FileLocation location)
{
    Tokenizer tokenizer = loadStringIntoTokenizer(source);
    Expr result = parsePrimaryOfSasmTokens(region, &tokenizer, location);

    return result;
}

static Expr parseNumFromSasmTokens(Region* region, Tokenizer* tokenizer, FileLocation location)
{
    Token token = { 0 };

    if (!moveSasmTokenizerToNextToken(tokenizer, &token, location)) {
        fprintf(stderr, FLFmt ": ERROR: Cannot parse empty expression\n",
            FLArg(location));
        exit(1);
    }

    Expr result = { 0 };

    if (token.type == TOKEN_TYPE_NUMBER) {
        String text = token.text;

        const char* cstr = convertRegionStrtoCStr(region, text);
        char* endptr = 0;

        if (startsWith(text, convertCstrToStr("0x"))) {
            result.value.lit_int = strtoull(cstr, &endptr, 16);
            if ((size_t)(endptr - cstr) != text.length) {
                fprintf(stderr, FLFmt ": ERROR: `" strFmt "` is not a hex literal\n",
                    FLArg(location), strArg(text));
                exit(1);
            }

            result.type = EXPR_LIT_INT;
        } else {
            result.value.lit_int = strtoull(cstr, &endptr, 10);
            if ((size_t)(endptr - cstr) != text.length) {
                result.value.lit_float = strtod(cstr, &endptr);
                if ((size_t)(endptr - cstr) != text.length) {
                    fprintf(stderr, FLFmt ": ERROR: `" strFmt "` is not a number literal\n",
                        FLArg(location), strArg(text));
                } else {
                    result.type = EXPR_LIT_FLOAT;
                }
            } else {
                result.type = EXPR_LIT_INT;
            }
        }
    } else {
        fprintf(stderr, FLFmt ": ERROR: expected %s but got %s",
            FLArg(location),
            getTokenName(TOKEN_TYPE_NUMBER),
            getTokenName(token.type));
        exit(1);
    }

    return result;
}

void parseSasmDirectiveFromLine(Region* region, SasmLexer* lineInterpreter, CodeBlock* output)
{
    Line line = { 0 };

    if (!moveSasmLexerToNextLine(lineInterpreter, &line) || line.kind != LINE_DIRECTIVE) {
        fprintf(stderr, FLFmt ": ERROR: expected a directive line\n",
            FLArg(lineInterpreter->location));
        exit(1);
    }

    FileLocation location = line.location;
    String name = line.value.directive.name;
    String body = line.value.directive.body;
    // printString(body);

    if (compareStr(name, convertCstrToStr("include"))) {
        Stmt statement = { 0 };
        statement.location = location;
        statement.kind = STMT_INCLUDE;
        Expr path = parseExprFromStr(region, body, line.location);

        if (path.type != EXPR_LIT_STR) {
            fprintf(stderr, FLFmt "ERROR: expected string literal as path for %%include directive\n",
                FLArg(location));
            exit(1);
        }

        statement.value.include.path = path.value.lit_str;

        pushStatementIntoBlock(region, output, statement);
    } else if (compareStr(name, convertCstrToStr("bind"))) {
        Stmt statement = { 0 };
        statement.location = location;
        statement.kind = STMT_CONST;

        Tokenizer tokenizer = loadStringIntoTokenizer(body);
        Expr bindingName = parsePrimaryOfSasmTokens(region, &tokenizer, location);
        if (bindingName.type != EXPR_BINDING) {
            fprintf(stderr, FLFmt ": ERROR: expected binding name for %%bind binding\n",
                FLArg(location));
            exit(1);
        }
        statement.value.constant.name = bindingName.value.binding;

        statement.value.constant.value = parsePrimaryOfSasmTokens(region, &tokenizer, location);
        confirmNoMoreTokensRemain(&tokenizer, location);

        pushStatementIntoBlock(region, output, statement);
    } else if (compareStr(name, convertCstrToStr("entry"))) {
        body = trim(body);
        bool inline_entry = false;

        if (endsWith(body, convertCstrToStr(":"))) {
            splitStrByLenReversed(&body, 1);
            inline_entry = true;
        }

        Expr expr = parseExprFromStr(region, body, line.location);

        Stmt statement = { 0 };
        statement.location = location;
        statement.kind = STMT_ENTRY;
        statement.value.entry.value = expr;
        pushStatementIntoBlock(region, output, statement);

        if (inline_entry) {
            Stmt statement = { 0 };
            statement.location = location;
            statement.kind = STMT_LABEL;

            if (expr.type != EXPR_BINDING) {
                fprintf(stderr, FLFmt ": ERROR: expected binding name for a label\n",
                    FLArg(location));
                exit(1);
            }

            statement.value.label.name = expr.value.binding;
            // printf("%d", statement.kind);
            pushStatementIntoBlock(region, output, statement);
        }
    } else if (compareStr(name, convertCstrToStr("scope"))) {
        Stmt statement = { 0 };
        statement.location = location;
        statement.kind = STMT_SCOPE;
        statement.value.scope = getCodeBlockFromLines(region, lineInterpreter).begin;

        if (!moveSasmLexerToNextLine(lineInterpreter, &line) || line.kind != LINE_DIRECTIVE || !compareStr(line.value.directive.name, convertCstrToStr("end"))) {
            fprintf(stderr, FLFmt ": ERROR: expected `%%end` directive at the end of the `%%scope` block\n",
                FLArg(lineInterpreter->location));
            fprintf(stderr, FLFmt ": NOTE: the %%scope block starts here\n",
                FLArg(statement.location));
            exit(1);
        }
        pushStatementIntoBlock(region, output, statement);
    } else {
        fprintf(stderr, FLFmt ": ERROR: unknown directive `" strFmt "`\n",
            FLArg(line.location), strArg(name));
        exit(1);
    }
}

String ParseStrFromSasmTokens(Tokenizer* tokenizer, FileLocation location)
{
    Token token = { 0 };
    if (!moveSasmTokenizerToNextToken(tokenizer, &token, location) || token.type != TOKEN_TYPE_STR) {
        fprintf(stderr, FLFmt ": ERROR: expected token %s\n",
            FLArg(location), getTokenName(token.type));
        exit(1);
    }

    return token.text;
}

Expr parsePrimaryOfSasmTokens(Region* region, Tokenizer* tokenizer, FileLocation location)
{
    Token token = { 0 };

    if (!fetchCachedSasmTokenFromSasmTokenizer(tokenizer, &token, location)) {
        fprintf(stderr, FLFmt ": ERROR: Cannot parse empty expression\n",
            FLArg(location));
        exit(1);
    }

    Expr result = { 0 };

    switch (token.type) {
    case TOKEN_TYPE_STR:
        result.type = EXPR_LIT_STR;
        result.value.lit_str = ParseStrFromSasmTokens(tokenizer, location);
        break;

    case TOKEN_TYPE_CHAR:
        moveSasmTokenizerToNextToken(tokenizer, NULL, location);

        if (token.text.length != 1) {
            fprintf(stderr, FLFmt ": ERROR: the length of char literal has to be exactly one\n",
                FLArg(location));
            exit(1);
        }

        result.type = EXPR_LIT_CHAR;
        result.value.lit_char = token.text.data[0];
        break;

    case TOKEN_TYPE_NAME:
        moveSasmTokenizerToNextToken(tokenizer, NULL, location);

        Token next = { 0 };
        if (fetchCachedSasmTokenFromSasmTokenizer(tokenizer, &next, location) && next.type == TOKEN_TYPE_OPEN_PAREN) {
            result.type = EXPR_FUNCALL;
            result.value.funcall = allocateRegion(region, sizeof(Funcall));
            result.value.funcall->name = token.text;
            result.value.funcall->args = parseFuncallArgs(region, tokenizer, location);
        } else {
            result.value.binding = token.text;
            result.type = EXPR_BINDING;
        }
        break;

    case TOKEN_TYPE_NUMBER:
        return parseNumFromSasmTokens(region, tokenizer, location);

    case TOKEN_TYPE_OPEN_PAREN:
        moveSasmTokenizerToNextToken(tokenizer, NULL, location);
        Expr expr = parsePrimaryOfSasmTokens(region, tokenizer, location);

        if (!moveSasmTokenizerToNextToken(tokenizer, &token, location) || token.type != TOKEN_TYPE_CLOSING_PAREN) {
            fprintf(stderr, FLFmt ": ERROR: expected `%s`\n",
                FLArg(location), getTokenName(TOKEN_TYPE_CLOSING_PAREN));
            exit(1);
        }
        return expr;
        break;
    case TOKEN_TYPE_REGISTER:
        moveSasmTokenizerToNextToken(tokenizer, NULL, location);
        String str = token.text;
        // printString(token.text);
        switch (str.data[0]) {
        case 'H':
            if (str.data[1] > '1') {
                fprintf(stderr, FLFmt ": ERROR: Invalid register %s\n",
                    FLArg(location), str.data);
                exit(1);
            }
            result.value.reg_id = (uint64_t)(REG_H0 + str.data[1] - '0');
            break;
        case 'I':
            if (str.data[1] > '1') {
                fprintf(stderr, FLFmt ": ERROR: Invalid register %s\n",
                    FLArg(location), str.data);
                exit(1);
            }
            result.value.reg_id = (uint64_t)(REG_I0 + str.data[1] - '0');
            break;
        case 'L':
            if (str.data[1] > '3') {
                fprintf(stderr, FLFmt ": ERROR: Invalid register %s\n",
                    FLArg(location), str.data);
                exit(1);
            }
            result.value.reg_id = (uint64_t)(REG_L0 + str.data[1] - '0');
            break;
        case 'P':
            if (str.data[1] > '3') {
                fprintf(stderr, FLFmt ": ERROR: Invalid register %s\n",
                    FLArg(location), str.data);
                exit(1);
            }
            result.value.reg_id = (uint64_t)(REG_P0 + str.data[1] - '0');
            break;
        case 'J':
            result.value.reg_id = (uint64_t)REG_JS;
            break;
        case 'K':
            result.value.reg_id = (uint64_t)REG_KC;
            break;
        case 'O':
            result.value.reg_id = (uint64_t)REG_OP;
            break;
        case 'Q':
            result.value.reg_id = (uint64_t)REG_QT;
            break;
        case 'R':
            result.value.reg_id = (uint64_t)REG_RF;
            break;

        default:
            fprintf(stderr, FLFmt ": ERROR: Invalid register %s\n",
                FLArg(location), str.data);
            exit(1);
        };

        result.type = EXPR_REG;

        break;
    case TOKEN_TYPE_COMMA:
    case TOKEN_TYPE_CLOSING_PAREN:
        fprintf(stderr, FLFmt ": ERROR: expected primary expression but found %s\n",
            FLArg(location), getTokenName(token.type));
        exit(1);
        break;

    default:
        assert(false && "parsePrimaryOfSasmTokens: unreachable");
        exit(1);
    }

    return result;
}

CodeBlock getCodeBlockFromLines(Region* region, SasmLexer* lineInterpreter)
{
    CodeBlock result = { 0 };

    Line line = { 0 };
    while (fetchCachedLineFromSasmLexer(lineInterpreter, &line)) {
        const FileLocation location = line.location;
        Stmt statement = { 0 };
        statement.location = location;
        switch (line.kind) {
        case LINE_INSTRUCTION:
            String name = line.value.instruction.name;
            String operandList = line.value.instruction.operand;

            OpcodeDetails details;
            if (!getOpcodeDetailsFromName(name, &details)) {
                continue;
            }

            statement.kind = STMT_INST;
            statement.value.inst.type = details.type;

            if (details.has_operand) {
                String opr1 = trim(splitStrByChar(&operandList, ' '));
                // printString(opr1);
                Expr operand = parseExprFromStr(region, opr1, location);
                statement.value.inst.operand = operand;
            }

            if (details.has_operand2) {
                String opr2 = trim(operandList);
                // printString(opr2);
                Expr operand = parseExprFromStr(region, opr2, location);
                statement.value.inst.operand2 = operand;
            }
            pushStatementIntoBlock(region, &result, statement);
            moveSasmLexerToNextLine(lineInterpreter, NULL);
            break;

        case LINE_LABEL:
            Expr label = parseExprFromStr(region, line.value.label.name, location);

            if (label.type != EXPR_BINDING) {
                displayErrorLocationWithExit(location, "expected binding name for a label");
            }

            statement.kind = STMT_LABEL;
            statement.value.label.name = label.value.binding;
            pushStatementIntoBlock(region, &result, statement);
            moveSasmLexerToNextLine(lineInterpreter, NULL);
            break;

        case LINE_DIRECTIVE:
            if (compareStr(line.value.directive.name, convertCstrToStr("end"))) {
                return result;
            }

            parseSasmDirectiveFromLine(region, lineInterpreter, &result);
            continue;
        }
    }

    return result;
}

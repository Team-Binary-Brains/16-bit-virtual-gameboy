#include "sasm_assembler.h"

static bool isName(char x)
{
    return isalnum(x) || x == '_';
}

static bool isNumber(char x)
{
    return isalnum(x) || x == '.' || x == '-';
}

bool fetchCachedSasmTokenFromSasmTokenizer(Tokenizer* tokenizer, Token* output, FileLocation location)
{
    if (tokenizer->hasCachedToken) {
        if (output) {
            *output = tokenizer->cachedToken;
        }
        return true;
    }

    tokenizer->source = ltrim(tokenizer->source);

    if (tokenizer->source.length == 0) {
        return false;
    }

    Token token = { 0 };
    switch (*tokenizer->source.data) {
    case '(':
        {
            token.type = TOKEN_TYPE_OPEN_PAREN;
            token.text = splitStrByLen(&tokenizer->source, 1);
        }
        break;

    case ')':
        {
            token.type = TOKEN_TYPE_CLOSING_PAREN;
            token.text = splitStrByLen(&tokenizer->source, 1);
        }
        break;

    case ',':
        {
            token.type = TOKEN_TYPE_COMMA;
            token.text = splitStrByLen(&tokenizer->source, 1);
        }
        break;

    case '"':
        {
            splitStrByLen(&tokenizer->source, 1);

            size_t index = 0;

            if (getIndexOf(tokenizer->source, '"', &index)) {
                String text = splitStrByLen(&tokenizer->source, index);
                splitStrByLen(&tokenizer->source, 1);
                token.type = TOKEN_TYPE_STR;
                token.text = text;
            } else {
                fprintf(stderr, FLFmt ": ERROR: Could not find closing \"\n",
                    FLArg(location));
                exit(1);
            }
        }
        break;

    case '\'':
        {
            splitStrByLen(&tokenizer->source, 1);

            size_t index = 0;

            if (getIndexOf(tokenizer->source, '\'', &index)) {
                String text = splitStrByLen(&tokenizer->source, index);
                splitStrByLen(&tokenizer->source, 1);
                token.type = TOKEN_TYPE_CHAR;
                token.text = text;
            } else {
                fprintf(stderr, FLFmt ": ERROR: Could not find closing \'\n",
                    FLArg(location));
                exit(1);
            }
        }
        break;

    default:
        {
            if (isalpha(*tokenizer->source.data)) {
                token.type = TOKEN_TYPE_NAME;
                token.text = splitStrByCondition(&tokenizer->source, isName);
            } else if (isdigit(*tokenizer->source.data) || *tokenizer->source.data == '-') {
                token.type = TOKEN_TYPE_NUMBER;
                token.text = splitStrByCondition(&tokenizer->source, isNumber);
            } else if (tokenizer->source.length >=3 && *tokenizer->source.data == '[' && tokenizer->source.data[3] == ']') {
                if (tokenizer->source.length - 2 < 2) {
                    fprintf(stderr, FLFmt ": ERROR: Check register name %c\n",
                        FLArg(location), *tokenizer->source.data);
                }
                splitStrByLen(&tokenizer->source, 1);
                size_t index = 0;
                if (getIndexOf(tokenizer->source, ']', &index)) {
                    token.type = TOKEN_TYPE_REGISTER;
                    token.text = splitStrByLen(&tokenizer->source, index);
                    splitStrByLen(&tokenizer->source, 1);
                } else {
                    fprintf(stderr, FLFmt ": ERROR: Could not find closing \'\n",
                        FLArg(location));
                    exit(1);
                }
            } else {
                fprintf(stderr, FLFmt ": ERROR: Unknown token starts with %c\n",
                    FLArg(location), *tokenizer->source.data);
                exit(1);
            }
        }
    }

    tokenizer->hasCachedToken = true;
    tokenizer->cachedToken = token;

    if (output) {
        *output = token;
    }

    return true;
}

bool moveSasmTokenizerToNextToken(Tokenizer* tokenizer, Token* token, FileLocation location)
{
    if (fetchCachedSasmTokenFromSasmTokenizer(tokenizer, token, location)) {
        tokenizer->hasCachedToken = false;
        return true;
    }

    return false;
}

Tokenizer loadStringIntoTokenizer(String source)
{
    return (Tokenizer) {
        .source = source
    };
}

const char* getTokenName(TokenType type)
{
    switch (type) {
    case TOKEN_TYPE_STR:
        return "string";
    case TOKEN_TYPE_CHAR:
        return "character";
    case TOKEN_TYPE_NUMBER:
        return "number";
    case TOKEN_TYPE_NAME:
        return "name";
    case TOKEN_TYPE_OPEN_PAREN:
        return "open paren";
    case TOKEN_TYPE_CLOSING_PAREN:
        return "closing paren";
    case TOKEN_TYPE_COMMA:
        return "comma";
    case TOKEN_TYPE_REGISTER:
        return "register";
    default:
        {
            assert(false && "getTokenName: unreachable");
            exit(1);
        }
    }
}
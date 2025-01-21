#include "univ_defs.h"
#include "O_lexer.h"

size_t lineNumber = 0;

// void __printToken(Token token)
// {
//     printf("TOKEN VALUE: ");
//     printf("'");
//     for (int i = 0; token.value[i] != '\0'; i++) {
//         printf("%c", token.value[i]);
//     }
//     printf("'");
//     printf("\nline number: %zu", token.lineNum);

//     switch (token.type) {
//     case INT:
//         printf(" TOKEN TYPE: INT\n");
//         break;
//     case KEYWORD:
//         printf(" TOKEN TYPE: KEYWORD\n");
//         break;
//     case SEPARATOR:
//         printf(" TOKEN TYPE: SEPARATOR\n");
//         break;
//     case OPERATOR:
//         printf(" TOKEN TYPE: OPERATOR\n");
//         break;
//     case IDENTIFIER:
//         printf(" TOKEN TYPE: IDENTIFIER\n");
//         break;
//     case STRING:
//         printf(" TOKEN TYPE: STRING\n");
//         break;
//     case COMP:
//         printf(" TOKEN TYPE: COMPARATOR\n");
//         break;
//     case END_OF_TOKENS:
//         printf(" END OF TOKENS\n");
//         break;
//     case BEGINNING:
//         printf("BEGINNING\n");
//         break;
//     }
// }

Token *generateKeywordOrIdentifier(char *current, int* currentIndex){
    Token *token = initToken(UNKNOWN);
    char *keyword = malloc(sizeof(char) * 8);
    int keywordIndex = 0;
    while(current[*currentIndex]!='\0' && isalpha(current[*currentIndex])){
        keyword[keywordIndex] = current[*currentIndex];
        keywordIndex++;
        *currentIndex+=1;
    }

    keyword[keywordIndex] = '\0';

    size_t len = sizeof(StringTokenMap) / sizeof(StringTokenMap[0]);
    for(int i=0; i<len; i++){
        if((*keyword) == SingleCharTokenMap[i].value){
            token->value = *keyword;
            token->type = SingleCharTokenMap[i].type;
            return token;
        }
    }

    token->value = *keyword;
    token->type = IDENTIFIER;


    return token;
}

Token* initToken(TokenType type){
    Token* token = malloc(sizeof(Token));
    token->lineNum = (size_t)malloc(sizeof(size_t));
    token->lineNum = lineNumber;
    token->type = type;
    return token;
}

Token* generateIntLToken(char* current, int* currentIndex)
{
    // Token Creation
    Token* token = initToken(INT_L);
    char* value = malloc(sizeof(char) * 8);
    int value_index = 0;
    while (current[*currentIndex] != '\0' && isdigit(current[*currentIndex])) {
        value[value_index] = current[*currentIndex];
        value_index++;
        *currentIndex += 1;
    }
    value[value_index] = '\0';
    token->value = value;
    return token;
}

void generateSingleCharToken(char ch, Token* token)
{
    size_t len = sizeof(SingleCharTokenMap) / sizeof(SingleCharTokenMap[0]);
    for(int i=0; i<len; i++){
        if(ch == SingleCharTokenMap[i].value){
            token->value = ch;
            token->type = SingleCharTokenMap[i].type;
        }
    }
}

void generateTwoCharToken(char* current, int* currentIndex, Token* token){
    char* value = malloc(sizeof(char)*3);
    value[0] = current[*currentIndex];
    currentIndex++;
    value[1] = current[*currentIndex];
    value[2] = '\0';

    size_t len = sizeof(TwoCharTokenMap) / sizeof(TwoCharTokenMap[0]);
    for(int i=0; i<len; i++){
        if(value == SingleCharTokenMap[i].value){
            token->value = value;
            token->type = SingleCharTokenMap[i].type;
        }
    }
    currentIndex--;
    
}

Token* generateStringLToken(char* current, int* currentIndex)
{
    Token* token = initToken(STR_L);
    char* value = malloc(sizeof(char) * 64);
    int value_index = 0;
    *currentIndex += 1;
    while (current[*currentIndex] != '"') {
        value[value_index] = current[*currentIndex];
        value_index++;
        *currentIndex += 1;
    }
    value[value_index] = '\0';
    token->value = value;
    return token;
}

size_t tokensIndex;

Token* lexer(FILE* file)
{
    int length;
    char* current = 0;

    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);

    current = malloc(sizeof(char) * length);
    fread(current, 1, length, file);
    fclose(file);

    current[length] = '\0';
    int currentIndex = 0;

    int number_of_tokens = 15;
    int tokens_size = 0;
    Token* tokens = malloc(sizeof(Token) * number_of_tokens);
    tokensIndex = 0;

    while (current[currentIndex] != '\0') {
        if (current[currentIndex] == ' ') {
            currentIndex++;
            continue;
        }

        tokens_size++;
        if (tokens_size > number_of_tokens) {
            number_of_tokens *= 1.5;
            tokens = realloc(tokens, sizeof(Token) * number_of_tokens);
        }

        if(current[currentIndex] == '\n'){
            lineNumber++;
        }else{
            Token* token = initToken(UNKNOWN);
            generateTwoCharToken(current, &currentIndex, token);
            generateSingleCharToken(current[currentIndex], token);

            if(strcmp(current[currentIndex], '"')){
                token = generateStringLToken(current, &currentIndex);
            }else if (isdigit(current[currentIndex])) {
                token = generateIntLToken(current, &currentIndex);
                currentIndex--;
            } else if (isalpha(current[currentIndex])) {
                token = generateKeywordOrIdentifier(current, &currentIndex);
                currentIndex--;
            }
            tokens[tokensIndex] = *token;
            tokensIndex++;
            free(token);
        }
        currentIndex++;
    }
    // Token *token = malloc(sizeof(Token));
    // token->value = malloc(sizeof(char)*1);
    // token->value = "a";
    // token->type = END_OF_TOKENS;
    // tokens[tokensIndex] = *token;
    // free(token);
    // printf("token index: %zu\n", tokensIndex);
    tokens[tokensIndex].value = '\0';
    tokens[tokensIndex].type = END_OF_TOKENS;
    // for(int i=0; i<tokensIndex; i++){
    //     __printToken(tokens[i]);
    // }
    return tokens;
}
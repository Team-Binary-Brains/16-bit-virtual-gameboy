#include "univ_defs.h"
#include "univ_hashmap.h"
#include "univ_fileops.h"
#include "O_lexer.h"
#include "O_debug_help.h"
#include "O_token_types.h"

size_t lineNumber = 1;
HashTable *OpAndSepTokenMap;
HashTable *KeywordTokenMap;

void createOpAndSepTokenMap(){
    OpAndSepTokenMap = createHashTable(20, stringHashFunc, stringKeyCompare, stringKeyDestroy, intValueDestroy);
    size_t len = sizeof(OpAndSepTokens) / sizeof(OpAndSepTokens[0]);
    for (size_t i = 0; i < len; i++) {
        char *key = strdup(OpAndSepTokens[i].value);           // Dynamically allocate memory for the key
        TokenType *value = malloc(sizeof(int));     // Dynamically allocate memory for the value
        *value = OpAndSepTokens[i].type;
        insert(OpAndSepTokenMap, key, value);
    }
}

void createKeywordTokenMap(){
    KeywordTokenMap = createHashTable(20, stringHashFunc, stringKeyCompare, stringKeyDestroy, intValueDestroy);
    size_t len = sizeof(KeywordTokens) / sizeof(KeywordTokens[0]);
    for (int i = 0; i < len; i++) {
        char *key = strdup(KeywordTokens[i].value);           
        TokenType *value = malloc(sizeof(int));     
        *value = KeywordTokens[i].type;
        insert(KeywordTokenMap, key, value);
    }
}

Token* initToken(TokenType type){
    Token* token = malloc(sizeof(Token));
    token->lineNum = lineNumber;
    token->type = type;
    return token;
}

void generateKeywordOrIdentifier(char *current, int* currentIndex, Token* token){
    char *keyword = malloc(sizeof(char) * 8);
    int keywordIndex = 0;
    while(current[*currentIndex]!='\0' && (isalpha(current[*currentIndex]) || isdigit(current[*currentIndex]))){
        keyword[keywordIndex] = current[*currentIndex];
        keywordIndex++;
        *currentIndex+=1;
    }

    keyword[keywordIndex] = '\0';

    token->value = keyword;

    int *res = retrieve(KeywordTokenMap, keyword);
    if(res == NULL){
        token->type = IDENTIFIER;
    }else{
        token->type = *res;
    }
    //free(keyword);
}

void generateOpAndSepToken(char* current, int* currentIndex, Token* token){
    char* value = malloc(sizeof(char)*3);
    value[0] = current[*currentIndex];
    if(current[(*currentIndex)+1] == '='){
        *currentIndex +=1 ;
        value[1] = current[(*currentIndex)];
        value[2] = '\0';
    }else{
        value = realloc(value, sizeof(char)*2);
        value[1] = '\0';
    }

    // O(1) operation
    int *res = retrieve(OpAndSepTokenMap, value);
    if(res != NULL){
        token->type = *res;
        token->value = value;
    }
}

void generateIntLorFloatLToken(char* current, int* currentIndex, Token* token){
    char* value = malloc(sizeof(char) * 16);
    int value_index = 0;
    while (current[*currentIndex] != '\0' && isdigit(current[*currentIndex])) {
        value[value_index] = current[*currentIndex];
        value_index++;
        *currentIndex += 1;
    }
    if(current[*currentIndex] == '.'){
        do{
            value[value_index] = current[*currentIndex];
            value_index++;
            *currentIndex += 1;
        }while (current[*currentIndex] != '\0' && isdigit(current[*currentIndex]));
        token->type = FLOAT_L;
    }else token->type = INT_L;
    value[value_index] = '\0';
    token->value = value;
}

void generateStringLToken(char* current, int* currentIndex, Token* token){
    token->type = STR_L;
    char* value = malloc(sizeof(char) * 64);
    int value_index = 0;
    *currentIndex += 1;
    while (current[*currentIndex] != '"' && current[*currentIndex]!='\0') {
        value[value_index] = current[*currentIndex];
        value_index++;
        *currentIndex += 1;
    }
    if(current[*currentIndex] == '\0'){
        printf("\nError: String Never Closed");
        exit(1);
    }
    value[value_index] = '\0';
    token->value = value;
}

void generateCharLToken(char* current, int *currentIndex, Token* token){
    if(current[*currentIndex+2] != '\''){
        printf("\nError: Character Literal Cannot be of more than one character");
        exit(1);
    }
    token->type = CHAR_L;
    char* value = malloc(sizeof(char) * 2);
    *currentIndex+=1;
    value[0] = current[*currentIndex];
    value[1] = '\0';
    *currentIndex+=1;
    token->value = value;
}

size_t tokensIndex;

Token* lexer(FILE* file, char* inputFile){
    long file_size;
    char* current = 0;

    // 'file_size' will contain actual size of file ('\n' are also counted)
    file_size = getFileSize(file, inputFile);

    // Allocate data to store all the contents of file (including '\n')
    // file_size +1 is for null character
    current = malloc(sizeof(char) * file_size+1u);

    // 'read_len' will contain no. of characters(including spaces) in the file. (Note: '\n' are excluded)
    // Also, No. of lines in the file = file_size - read_len
    size_t read_len = fread(current, 1, file_size, file);
    current[read_len] = '\0';

    fclose(file);

    createOpAndSepTokenMap();
    createKeywordTokenMap();
    int currentIndex = 0;

    int numberOfTokens = 15;
    int tokens_size = 0;
    Token* tokens = malloc(sizeof(Token) * numberOfTokens);
    tokensIndex = 0;

    while (current[currentIndex] != '\0') {
        if(current[currentIndex] == ' '){
            currentIndex++;
            continue;
        }

        if(current[currentIndex] == '/' && current[currentIndex+1] == '/'){
            while(current[currentIndex] != '\n' && current[currentIndex+1] != '\0'){
                currentIndex++;
            }
            if(current[currentIndex+1] == '\0'){
                currentIndex++;
                break;
            }
        }
        
        if(current[currentIndex] == '/' && current[currentIndex+1] == '*'){
            while((current[currentIndex] != '*' || current[currentIndex+1] != '/') && current[currentIndex+2] != '\0'){
                currentIndex++;
            }
            if(current[currentIndex+2] == '\0'){
                currentIndex+=2;
                break;
            }
            currentIndex = currentIndex+2;
        }

        Token* token = malloc(sizeof(Token));
        tokens_size++;
        if (tokens_size > numberOfTokens) {
            numberOfTokens *= 1.5;
            tokens = realloc(tokens, sizeof(Token) * numberOfTokens);
        }

        if(current[currentIndex] == '\n'){
            lineNumber++;
        }else{
            Token* token = initToken(UNKNOWN);

            if(current[currentIndex] == '"'){
                generateStringLToken(current, &currentIndex, token);
            }else if (isdigit(current[currentIndex])) {
                generateIntLorFloatLToken(current, &currentIndex, token);
                currentIndex--;
            } else if (current[currentIndex] == '\''){
                generateCharLToken(current, &currentIndex, token);
            }else if (isalpha(current[currentIndex])) {
                generateKeywordOrIdentifier(current, &currentIndex, token);
                currentIndex--;
            }else{
                generateOpAndSepToken(current, &currentIndex, token);
            }

            tokens[tokensIndex] = *token;
            tokensIndex++;
        }
        free(token);
        currentIndex++;
    }

    tokens[tokensIndex].value = "\0";
    tokens[tokensIndex].type = END_OF_TOKENS;
    __printTokens(tokens, tokensIndex);
    destroyHashTable(OpAndSepTokenMap);
    destroyHashTable(KeywordTokenMap);
    return tokens;
}
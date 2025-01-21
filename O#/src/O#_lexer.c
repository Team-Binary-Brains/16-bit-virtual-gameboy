#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

#include "../headers/O#_lexer.h"

size_t lineNumber = 0;

void __printToken(Token token){
    printf("TOKEN VALUE: ");
    printf("'");
    for(int i = 0; token.value[i] != '\0'; i++){
        printf("%c", token.value[i]);
    }
    printf("'");
    printf("\nline number: %zu", token.lineNum);

    switch(token.type){
        case INT:
        printf(" TOKEN TYPE: INT\n");
        break;
        case KEYWORD:
        printf(" TOKEN TYPE: KEYWORD\n");
        break;
        case SEPARATOR:
        printf(" TOKEN TYPE: SEPARATOR\n");
        break;
        case OPERATOR:
        printf(" TOKEN TYPE: OPERATOR\n");
        break;
        case IDENTIFIER:
        printf(" TOKEN TYPE: IDENTIFIER\n");
        break;
        case STRING:
        printf(" TOKEN TYPE: STRING\n");
        break;
        case COMP:
        printf(" TOKEN TYPE: COMPARATOR\n");
        break;
        case END_OF_TOKENS:
        printf(" END OF TOKENS\n");
        break;
        case BEGINNING:
        printf("BEGINNING\n");
        break;
    }
}

Token *generateNumber(char *current, int* currentIndex){
    // Token Creation
    Token *token = malloc(sizeof(Token));
    token->lineNum = (size_t)malloc(sizeof(size_t));
    token->lineNum = lineNumber;
    token->type = INT;

    char* value = malloc(sizeof(char)*8);
    int value_index = 0;
    while(current[*currentIndex] != '\0' && isdigit(current[*currentIndex])){
        value[value_index] = current[*currentIndex];
        value_index++;
        *currentIndex+=1;
    }
    value[value_index] = '\0';
    token->value = value;
    return token;
}

Token *generateKeywordOrIdentifier(char *current, int* currentIndex){
    Token *token = malloc(sizeof(Token));
    token->lineNum = (size_t)malloc(sizeof(size_t));
    token->lineNum = lineNumber;
    char *keyword = malloc(sizeof(char) * 8);
    int keywordIndex = 0;
    while(current[*currentIndex]!='\0' && isalpha(current[*currentIndex])){
        keyword[keywordIndex] = current[*currentIndex];
        keywordIndex++;
        *currentIndex+=1;
    }

    keyword[keywordIndex] = '\0';


    if (strcmp(keyword, "exit") == 0){ 
        token->type = KEYWORD;
        token->value = "EXIT";
    }else if(strcmp(keyword, "int") == 0){
        token->type = KEYWORD;
        token->value = "INT";
    }else if(strcmp(keyword, "if")==0){
        token->type = KEYWORD;
        token->value = "IF";
    }else if(strcmp(keyword, "while")==0){
        token->type = KEYWORD;
        token->value = "WHILE";
    }else if(strcmp(keyword, "write")==0){
        token->type = KEYWORD;
        token->value = "WRITE";
    }else if(strcmp(keyword, "eq")==0){
        token->type = KEYWORD;
        token->value = "EQ";
    }else if(strcmp(keyword, "neq")==0){
        token->type = COMP;
        token->value = "NEQ";
    }else if(strcmp(keyword, "less") == 0){
        token->type = COMP;
        token->value = "LESS";
    } else if(strcmp(keyword, "greater") == 0){
        token->type = COMP;
        token->value = "GREATER";
    } else {
        token->type = IDENTIFIER;
        token->value = keyword;
    }
    return token;
}

Token *generateStringToken(char *current, int *currentIndex){
  Token *token = malloc(sizeof(Token));
  token->lineNum = (size_t)malloc(sizeof(size_t));
  token->lineNum = lineNumber;
  char *value = malloc(sizeof(char) * 64);
  int value_index = 0;
  *currentIndex += 1;
  while(current[*currentIndex] != '"'){
    value[value_index] = current[*currentIndex];
    value_index++;
    *currentIndex += 1;
  }
  value[value_index] = '\0';
  token->type = STRING;
  token->value = value;
  return token;
}

Token *generateSeparatorOrOperator(char *current, int *currentIndex, TokenType type){
  Token *token = malloc(sizeof(Token));
  token->value = malloc(sizeof(char) * 2);
  token->value[0] = current[*currentIndex];
  token->value[1] = '\0';
  token->lineNum = (size_t)malloc(sizeof(size_t));
  token->lineNum = lineNumber;
  token->type = type;
  return token;
}

size_t tokensIndex;

Token *lexer(FILE *file){
    int length;
    char *current = 0;

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
    Token *tokens = malloc(sizeof(Token) * number_of_tokens);
    tokensIndex = 0;

    while(current[currentIndex] != '\0'){
        Token *token = malloc(sizeof(Token));
        tokens_size++;
        if(tokens_size>number_of_tokens){
            number_of_tokens *= 1.5;
            tokens = realloc(tokens, sizeof(Token) * number_of_tokens);
        }

        if(current[currentIndex] == ' '){
            free(token);
            currentIndex++;
            continue;
        }

        switch(current[currentIndex]){ // Improvement
            case ';':
            case ',':
            case '(':
            case ')':
            case '{':
            case '}':
                token = generateSeparatorOrOperator(current, &currentIndex, SEPARATOR);
                break;
            
            case '=':
            case '+':
            case '-':
            case '*':
            case '/':
            case '%':
                token = generateSeparatorOrOperator(current, &currentIndex, OPERATOR);
                break;
            case '"':
                token = generateStringToken(current, &currentIndex);
                break;
        }
        
        if(isdigit(current[currentIndex])){
            token = generateNumber(current, &currentIndex);
            currentIndex--;
        }else if(isalpha(current[currentIndex])){
            token = generateKeywordOrIdentifier(current, &currentIndex);
            currentIndex--;
        }

        if(current[currentIndex]=='\n'){ // Improvement
            lineNumber += 1;
        }else{
            tokens[tokensIndex] = *token;
            tokensIndex++;
        }
        free(token);
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
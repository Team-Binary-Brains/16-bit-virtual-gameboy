#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

#include "O#_lexer.h"

size_t line_number = 0;

Token *generate_number(char *current, int* current_index){
    // Token Creation
    Token *token = malloc(sizeof(Token));
    token->line_num = malloc(sizeof(size_t));
    token->line_num = line_number;
    token->type = INT;

    char* value = malloc(sizeof(char)*8);
    int value_index = 0;
    while(current[*current_index] != '\0' && isdigit(current[*current_index])){
        value[value_index] = current[*current_index];
        value_index++;
        *current_index+=1;
    }
    value[value_index] = '\0';
    token->value = value;
    return token;
}

Token *generate_keyword_or_identifier(char *current, int* current_index){
    Token *token = malloc(sizeof(Token));
    token->line_num = malloc(sizeof(size_t));
    token->line_num = line_number;
    char *keyword = malloc(sizeof(char) * 8);
    int keyword_index = 0;
    while(current[*current_index]!='\0' && isalpha(current[*current_index])){
        keyword[keyword_index] = current[*current_index];
        keyword_index++;
        *current_index+=1;
    }

    keyword[keyword_index] = '\0';

    if (strcmp(keyword, "exit") == 0){
        token->type = KEYWORD;
        token->value = "EXIT";
    }else if(strcmp(keyword, "int") == 0){
        token->type = KEYWORD;
        token->value = "INT";
    }else if(strcmp(keyword, 'if')==0){
        token->type = KEYWORD;
        token->value = "IF";
    }else if(strcmp(keyword, 'while')==0){
        token->type = KEYWORD;
        token->value = "WHILE";
    }else if(strcmp(keyword, 'write')==0){
        token->type = KEYWORD;
        token->value = "WRITE";
    }else if(strcmp(keyword, 'eq')==0){
        token->type = KEYWORD;
        token->value = "EQ";
    }else if(strcmp(keyword, 'neq')){
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

Token *generate_string_token(char *current, int *current_index){
  Token *token = malloc(sizeof(Token));
  token->line_num = malloc(sizeof(size_t));
  token->line_num = line_number;
  char *value = malloc(sizeof(char) * 64);
  int value_index = 0;
  *current_index += 1;
  while(current[*current_index] != '"'){
    value[value_index] = current[*current_index];
    value_index++;
    *current_index += 1;
  }
  value[value_index] = '\0';
  token->type = STRING;
  token->value = value;
  return token;
}

Token *generate_separator_or_operator(char *current, int *current_index, TokenType type){
  Token *token = malloc(sizeof(Token));
  token->value = malloc(sizeof(char) * 2);
  token->value[0] = current[*current_index];
  token->value[1] = '\0';
  token->line_num = malloc(sizeof(size_t));
  token->line_num = line_number;
  token->type = type;
  return token;
}

size_t tokens_index;

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
    int current_index = 0;

    int number_of_tokens = 12;
    int tokens_size = 0;
    Token *tokens = malloc(sizeof(Token) * number_of_tokens);
    tokens_index = 0;

    while(current[current_index] != '\0'){
        Token *token = malloc(sizeof(Token));
        tokens_size++;
        if(tokens_size>number_of_tokens){
            number_of_tokens *= 1.5;
            tokens = realoc(tokens, sizeof(Token) * number_of_tokens);
        }

        switch(current[current_index]){ // Improvement
            case ';':
            case ',':
            case '(':
            case ')':
            case '{':
            case '}':
                token = generate_separator_or_operator(current, &current_index, SEPARATOR);
                break;
            
            case '=':
            case '+':
            case '-':
            case '*':
            case '/':
            case '%':
                token = generate_separator_or_operator(current, &current_index, OPERATOR);
                break;
        }
        
        if(isdigit(current[current_index])){
            Token *test_token = generate_number(current, &current_index);
            current_index--;
        }else if(isalpha(current[current_index])){
            Token *test_keyword = generate_keyword(current, &current_index);
            current_index--;
        }

        if(current[current_index]=='\n'){ // Improvement
            line_number += 1;
        }else{
            tokens[tokens_index] = *token;
            tokens_index++;
        }
        free(token);
        current_index++;
    }

    tokens[tokens_index].value = '\0';
    tokens[tokens_index].type = END_OF_TOKENS;
    return tokens;
}
#pragma once

typedef struct Node{
    char *value;
    TokenType type;
    struct Node *right;
    struct Node *left;
}Node;



Node *parser(Token *tokens);
void print_tree(Node *node, int indent, char *identifier);
Node *init_node(Node *node, char *value, TokenType type);

void print_error(char *error_type, size_t line_number);
void handle_token_errors(char *error_text, Token *current_token, TokenType type);

Node *handle_exit_syscall(Node *root, Token *current_token, Node *current);
Node *create_variables(Token *current_token, Node *current);
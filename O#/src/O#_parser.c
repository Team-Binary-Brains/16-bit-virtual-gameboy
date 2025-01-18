#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../headers/O#_parser.h"
#include "../headers/O#_lexer.h"

void print_error(char *error_type, size_t line_number){
    printf("ERROR: %s on line number: %zu\n", error_type, line_number);
    exit(1);
}

void handle_token_errors(char *error_text, Token *current_token, TokenType type){
    if(current_token->type ==END_OF_TOKENS || current_token->type != type){
        print_error(error_text, current_token->line_num);
    }
}

Node *init_node(Node *node, char *value, TokenType type){
    node = malloc(sizeof(Node));
    node->value = malloc(sizeof(char) * 2);
    node->type = (int)type;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}


Node *handle_exit_syscall(Node *root, Token *current_token, Node * current){
    Node *exit_node = malloc(sizeof(Node));
    exit_node = init_node(exit_node, current_token->value, KEYWORD);
    current->right = exit_node;
    current = exit_node;
    current_token++;

    handle_token_errors("Invalid Syntax on OPEN", current_token, SEPARATOR);
    if(strcmp(current_token->value, '(') == 0){ // Improvement
        Node *open_paren_node = malloc(sizeof(Node));
        open_paren_node = init_node(open_paren_node, current_token->value, SEPARATOR);
        current->left = open_paren_node;
        current_token++;
        if(current_token->type == END_OF_TOKENS){
            print_error("Invalid Syntax on INT", current_token->line_num);
        }
        if(current_token->type==INT || current_token->type == IDENTIFIER){
            current_token++;
            if(current_token->type == OPERATOR && current_token != NULL){
                current_token = generate_operation_nodes(current_token, current);
                current_token--;
            }else{
                current_token--;
                Node *expr_node = malloc(sizeof(Node));
                expr_node = init_node(expr_node, current_token->value, current_token->type);
                current->left->left = expr_node;
            }
            current_token++;
            handle_token_errors("Invalid Syntax on CLOSE", current_token, SEPARATOR);
            if(strcmp(current_token->value, ")")==0){ // Improvement
                Node *close_paren_node = malloc(sizeof(Node));
                close_paren_node = init_node(close_paren_node, current_token->value, SEPARATOR);
                current->left->right = close_paren_node;
                current_token++;

                handle_token_errors("Invalid Syntax on SEMI", current_token, SEPARATOR);

                if(strcmp(current_token->value, ';') == 0){
                    Node *semi_node = malloc(sizeof(Node));
                    semi_node = init_node(semi_node, current_token->value, SEPARATOR);
                    current->right = semi_node;
                    current= semi_node;
                } else{
                    print_error("Invalid Syntax on SEMI", current_token->line_num);
                }
            }else {
                print_error("Invalid Syntax on CLOSE", current_token->line_num);
            }
        }else{
            print_error("Invalid Syntax on INT", current_token->line_num);
        }

    }else{
        print_error("Invalid Syntax on OPEN", current_token->line_num);
    }
    return current;
}

Node *create_variables(Token *current_token, Node *current){
    Node *var_node = malloc(sizeof(Node));
    var_node = init_node(var_node, current_token->value, KEYWORD);
    current->left = var_node;
    current = var_node;
    current_token++;

    handle_token_errors("Invalid syntax after INT", current_token, IDENTIFIER); //improvement
    Node *identifer_node = malloc(sizeof(Node));
    identifer_node = init_node(identifer_node, current_token->value, IDENTIFIER);
    current->left = identifer_node;
    current = identifer_node;
    current_token++;

    handle_token_errors("Invalid syntax after Identifier", current_token, OPERATOR);
    if(strcmp(current_token->value, "=") != 0){
        print_error("Error: Invalid Variable Syntax on =", current_token->line_num);
    }
    Node *equals_node = malloc(sizeof(Node));
    equals_node = init_node(equals_node, current_token->value, OPERATOR);
    current->left = equals_node;
    current = equals_node;
    current_token++;

    if(current_token->type == END_OF_TOKENS || (current_token->type != INT && current_token->type != IDENTIFIER)){
        print_error("ERROR: Invalid Syntax After Equals", current_token->line_num);
    }

    current_token++;
    if(current_token->type == OPERATOR){
        Node *oper_node = malloc(sizeof(Node));
        oper_node = init_node(oper_node, current_token->value, OPERATOR);
        current->left = oper_node;
        current = oper_node;
        current_token--;
        if(current_token->type == INT){
            Node *expr_node = malloc(sizeof(Node));
            expr_node = init_node(expr_node, current_token->value, INT);
            current_token++;
            current_token++;
        }else if(current_token->type == IDENTIFIER){
            Node *identifer_node = malloc(sizeof(Node));
            identifer_node = init_node(identifer_node, current_token->value, IDENTIFIER);
            oper_node->left = identifer_node;
            current_token++;
            current_token++;
        }else{
            print_error("ERROR: Expected IDENTIFIER or INT", current_token->line_num);

        }

        current_token++;

        if(current_token->type == OPERATOR){
            Node *oper_node = malloc(sizeof(Node));
            oper_node = init_node(oper_node, current_token->value, OPERATOR);
            current->right = oper_node;
            current = oper_node;
            int operation = 1;
            current_token--;
            current_token--;
            while(operation){
                current_token++;
                if(current_token->type == INT){
                Node *expr_node = malloc(sizeof(Node));
                expr_node = init_node(expr_node, current_token->value, INT);
                current->left = expr_node;
                } else if(current_token->type == IDENTIFIER){
                Node *identifier_node = malloc(sizeof(Node));
                identifier_node = init_node(identifier_node, current_token->value, IDENTIFIER);
                current->left = identifier_node;
                } else {
                printf("ERROR: Unexpected Token\n");
                exit(1);
                }
                current_token++;
                if(current_token->type == OPERATOR){
                current_token++;
                current_token++;
                if(current_token->type != OPERATOR){
                    current_token--;
                    if(current_token->type == INT){
                    Node *expr_node = malloc(sizeof(Node));
                    expr_node = init_node(expr_node, current_token->value, INT);
                    current->right = expr_node;
                    current_token++;
                    } else if(current_token->type == IDENTIFIER){
                    Node *identifier_node = malloc(sizeof(Node));
                    identifier_node = init_node(identifier_node, current_token->value, IDENTIFIER);
                    current->right = identifier_node;
                    current_token++;
                    } else {
                    printf("ERROR: UNRECOGNIZED TOKEN!\n");
                    exit(1);
                    }
                    operation = 0;
                } else {
                    current_token--;
                    current_token--;
                    Node *oper_node = malloc(sizeof(Node));
                    oper_node = init_node(oper_node, current_token->value, OPERATOR);
                    current->right = oper_node;
                    current = oper_node;
                }
                } else {
                operation = 0;
                }
            }
            
            } else {
            current_token--;
            if(current_token->type == INT){
                Node *expr_node = malloc(sizeof(Node));
                expr_node = init_node(expr_node, current_token->value, INT);
                oper_node->right = expr_node;
            } else if(current_token->type == IDENTIFIER){
                Node *identifier_node = malloc(sizeof(Node));
                identifier_node = init_node(identifier_node, current_token->value, IDENTIFIER);
                oper_node->right = identifier_node;
            }
            current_token++;
            }

    }else {
        current_token--;
        if(current_token->type == INT){
        Node *expr_node = malloc(sizeof(Node));
        expr_node = init_node(expr_node, current_token->value, INT);
        current->left = expr_node;
        current_token++;
        } else if(current_token->type == IDENTIFIER){
        Node *identifier_node = malloc(sizeof(Node));
        identifier_node = init_node(identifier_node, current_token->value, IDENTIFIER);
        current->left = identifier_node;
        current_token++;
        }
    }

    handle_token_errors("Invalid Syntax After Expression", current_token, SEPARATOR);

    current = var_node;
    if(strcmp(current_token->value, ";") == 0){
        Node *semi_node = malloc(sizeof(Node));
        semi_node = init_node(semi_node, current_token->value, SEPARATOR);
        current->right = semi_node;
        current = semi_node;
    }
    return current;
}

Node *parser(Token *tokens){
    Token *current_token = &tokens[0];
    Node *root = malloc(sizeof(Node));
    root = init_node(root, "PROGRAM", BEGINNING);

    Node *current = root;

    while(current_token->type !=END_OF_TOKENS){
        if(current == NULL){
            break;
        }

        switch(current_token->type){
            case KEYWORD:
                if(strcmp(current_token->value, "EXIT") == 0){
                    current = handle_exit_syscall(root, current_token, current);
                }else if(strcmp(current_token->value, "INT") == 0){
                    current = create_variables(current_token, current);
                }
                break;
            
        }
        current_token++;
    }
    return root;
}
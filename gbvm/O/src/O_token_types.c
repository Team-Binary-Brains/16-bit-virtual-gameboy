#include "univ_defs.h"
#include "O_token_types.h"

void __printTokens(Token* tokens, size_t tokensIndex) {
    printf("+--------------------------+--------------------------+\n");
    printf("| %-24s | %-24s |\n", "Token Value", "Token Type");
    printf("+--------------------------+--------------------------+\n");
    for(size_t i=0; i<tokensIndex; i++){
        printf("| %-24s | %-24s |\n", tokens[i].value, StrTokenType[tokens[i].type]);
    }
    printf("+--------------------------+--------------------------+\n");
}

// For printing single token
void __printToken(Token token) {
    printf("+--------------------------+--------------------------+\n");
    printf("| %-24s | %-24s |\n", "Token Value", "Token Type");
    printf("+--------------------------+--------------------------+\n");
    printf("| %-24s | %-24s |\n", token.value, StrTokenType[token.type]);
    printf("+--------------------------+--------------------------+\n");
}

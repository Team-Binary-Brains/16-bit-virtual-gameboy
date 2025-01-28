#include "univ_defs.h"
#include "O_token_types.h"

void __printToken(Token token){
    printf("Token value: %s\t\tToken Type:%d\n", token.value, token.type);
}
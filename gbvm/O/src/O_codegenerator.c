#pragma GCC diagnostic ignored "-Wunused-parameter"

#include "univ_defs.h"
#include "O_codegenerator.h"
#include "O_lexer.h"
#include "O_parser.h"

void traverseTree(Node* node, int is_left, FILE* file)
{
    if (node == NULL) {
        return;
    }

    if (strcmp(node->value, "EXIT") == 0) {
        fprintf(file, "HLT");
    }

    traverseTree(node->left, 1, file);
    traverseTree(node->right, 0, file);
}

int generateCode(Node* root, char* filename)
{
    FILE* file = fopen(filename, "w");
    assert(file != NULL && "ERROR: FILE COULD NOT BE OPENED\n");
    traverseTree(root, 0, file);
    fclose(file);
    return 0;
}

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

#include "./headers/O#_lexer.h"
#include "headers/O#_parser.h"
#include "headers/O#_codegenerator.h"

int main(int argc, char *argv[]){
    if(argc!=3){
        printf("Error: Correc Syntax: ./build/occ <filename> <output_filename>");
    }

    char *output_filename = malloc(sizeof(char) * 16);
    sprintf(output_filename, "%s.pblasm", argv[2]);
    FILE *file;
    file = fopen(argv[1], "r");

    if(!file){
        printf("FILE_ERROR: %s file not found\n", argv[1]);
        exit(1);
    }

    Token *tokens = lexer(file);

    Node *root = parser(tokens);

    code_generator(root, "%s.pblasm", argv[2]);

    FILE *assembly_file = fopen("gbvmasm.asm", "r");
    if(!assembly_file){
        printf("ERROR NOT KNOWN: Kya coder banega re tu\n");
        exit(1);
    }

    printf("Compiler Ready!!!\n");

    /*
    TODO: inputting assembly file to assembler
    */
    return 0;
}
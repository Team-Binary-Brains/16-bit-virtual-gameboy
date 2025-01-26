/**
 * @file O_codegenerator.h
 * @brief Header file for the code generator module.
 *
 * This file contains the declarations of functions related to code generation from the syntax tree.
 *
 * @author Omkar Jagtap
 * @date January 2025
 */

#pragma once
#include "O_lexer.h"
#include "O_parser.h"

/**
 * @brief Traverses the syntax tree and generates code based on the nodes.
 *
 * This function recursively traverses the syntax tree and generates code based on the nodes.
 * If the node value is "EXIT", it writes "HLT" to the specified file.
 *
 * @param node The current node being traversed.
 * @param file The file to write the generated code to.
 * @return The number of instructions generated.
 */
int generateCode(Node* node, char* file);

/**
 * @brief Generates code from the syntax tree and writes it to a file.
 *
 * This function generates code from the syntax tree and writes it to the specified file.
 * It opens the file in write mode and calls the traverseTree function to generate the code.
 * Finally, it closes the file and returns 0.
 *
 * @param root The root of the syntax tree.
 * @param is_left Flag to indicate if the node is a left child.
 * @param file The file to write the generated code to.
 */
void traverseTree(Node* root, int is_left, FILE* file);

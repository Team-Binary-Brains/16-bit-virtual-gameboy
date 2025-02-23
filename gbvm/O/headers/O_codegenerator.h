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
#include "univ_defs.h"
#include "O_inter_codegen.h"
#include "O_debug_help.h"

void intermediateCodeGen(ParseTreeNode*);
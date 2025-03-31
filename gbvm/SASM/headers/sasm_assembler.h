/**
 * @file sasm_assembler.h
 * @brief This file contains functions for assembling and disassembling programs in a custom assembly language.
 *
 * The functions in this file are used to manipulate custom assembly code and bytecode.
 */

#pragma once

#include "sasm_instructions.h"
#include "univ_errors.h"
#include "univ_malloc.h"

typedef struct {
    String name;
    QuadWord value;
} Binding;

typedef struct {
    InstAddr addr;
    String name;
} Label;

typedef struct {
    Binding bindings[BINDINGS_CAPACITY];
    size_t bindingCount;

    Label Labels[LABELS_CAPACITY];
    size_t LabelsCount;

    Program prog;

    Byte memory[MEMORY_CAPACITY];
    size_t memorySize;
    size_t memoryCapacity;

    Region region;

    size_t includeLevel;
} Sasm;

typedef struct Metadata {
    DoubleWord magic;
    Word version;
    DataEntry programSize;
    DataEntry entry;
    DataEntry memorySize;
    DataEntry memoryCapacity;
    DataEntry externalsSize;
} __attribute__((__packed__)) Metadata;

bool resolveBinding(const Sasm*, String name, QuadWord* output);

bool bindValue(Sasm*, String name, QuadWord QuadWord);

void pushLabel(Sasm*, InstAddr addr, String name);

bool translateLiteral(Sasm*, String s, QuadWord* output);

void assembleProgramIntoBytecode(Sasm*, const char* outputFilePath);

QuadWord pushStringToMemory(Sasm*, String s);

void parseAsmIntoProgram(Sasm*, String inputFilePath);

void loadProgramIntoSasm(Sasm* sasm, const char* file_path);

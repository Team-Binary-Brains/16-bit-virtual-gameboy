/**
 * @file sasm_addresser.h
 * @brief This file contains the declarations for the addresser functions used in the SASM assembler.
 *
 * @author Soham Metha
 * @date January 2025
 */

#pragma once
#include "sasm_memory.h"

/**
 * @brief Evaluates the value of a register.
 *
 * This function evaluates the value of a register based on the given CPU and temporary string.
 *
 * @param cpu The CPU structure.
 * @param tmp The temporary string.
 * @return The value of the register.
 */
Word evalRegister(CPU* cpu, String tmp);

/**
 * @brief Resolves the address based on the given  string.
 *
 * @param cpu The CPU structure.
 * @param s The string containing the address.
 * @return The resolved address.
 */
Word resolveAddress(CPU* cpu, String* s);

Word resolveImmediateAddress(String* s);
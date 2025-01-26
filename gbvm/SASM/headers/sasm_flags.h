/**
 * @file sasm_flags.h
 * @brief Declaration of the CPU flags manipulation functions.
 *
 * @author Soham Metha
 * @date January 2025
 */

#pragma once

#include "sasm_memory.h"

typedef enum {
    HALT = 1 << 0,
    SIGN = 1 << 1,
    OVERFLOW = 1 << 2,
    CARRY = 1 << 3,
    BORROW = 1 << 4,
    PARITY = 1 << 5,
    ZERO = 1 << 6,
} Flags;

/**
 * @brief Sets the Halt flag of the CPU.
 *
 * @param cpu   Pointer to the CPU structure.
 * @param halt  Boolean value indicating whether to set the Halt flag.
 */
void setHalt(CPU* cpu, bool halt);

/**
 * @brief Gets the value of the Halt flag from the CPU.
 *
 * @param cpu   Pointer to the CPU structure.
 * @return      Boolean value indicating the state of the Halt flag.
 */
bool getHalt(const CPU* cpu);

/**
 * @brief Sets the Sign flag of the CPU.
 *
 * @param cpu   Pointer to the CPU structure.
 * @param sign  Boolean value indicating whether to set the Sign flag.
 */
void setSign(CPU* cpu, bool sign);

/**
 * @brief Gets the value of the Sign flag from the CPU.
 *
 * @param cpu   Pointer to the CPU structure.
 * @return      Boolean value indicating the state of the Sign flag.
 */
bool getSign(const CPU* cpu);

/**
 * @brief Sets the Overflow flag of the CPU.
 *
 * @param cpu       Pointer to the CPU structure.
 * @param overflow  Boolean value indicating whether to set the Overflow flag.
 */
void setOverflow(CPU* cpu, bool overflow);

/**
 * @brief Gets the value of the Overflow flag from the CPU.
 *
 * @param cpu   Pointer to the CPU structure.
 * @return      Boolean value indicating the state of the Overflow flag.
 */
bool getOverflow(const CPU* cpu);

/**
 * @brief Sets the Carry flag of the CPU.
 *
 * @param cpu   Pointer to the CPU structure.
 * @param carry Boolean value indicating whether to set the Carry flag.
 */
void setCarry(CPU* cpu, bool carry);

/**
 * @brief Gets the value of the Carry flag from the CPU.
 *
 * @param cpu   Pointer to the CPU structure.
 * @return      Boolean value indicating the state of the Carry flag.
 */
bool getCarry(const CPU* cpu);

/**
 * @brief Sets the Borrow flag of the CPU.
 *
 * @param cpu       Pointer to the CPU structure.
 * @param borrow    Boolean value indicating whether to set the Borrow flag.
 */
void setBorrow(CPU* cpu, bool borrow);

/**
 * @brief Gets the value of the Borrow flag from the CPU.
 *
 * @param cpu   Pointer to the CPU structure.
 * @return      Boolean value indicating the state of the Borrow flag.
 */
bool getBorrow(const CPU* cpu);

/**
 * @brief Sets the Parity flag of the CPU.
 *
 * @param cpu       Pointer to the CPU structure.
 * @param parity    Boolean value indicating whether to set the Parity flag.
 */
void setParity(CPU* cpu, bool parity);

/**
 * @brief Gets the value of the Parity flag from the CPU.
 *
 * @param cpu   Pointer to the CPU structure.
 * @return      Boolean value indicating the state of the Parity flag.
 */
bool getParity(const CPU* cpu);

/**
 * @brief Sets the Zero flag of the CPU.
 *
 * @param cpu   Pointer to the CPU structure.
 * @param zero  Boolean value indicating whether to set the Zero flag.
 */
void setZero(CPU* cpu, bool zero);

/**
 * @brief Gets the value of the Zero flag from the CPU.
 *
 * @param cpu   Pointer to the CPU structure.
 * @return      Boolean value indicating the state of the Zero flag.
 */
bool getZero(const CPU* cpu);

# 1. SASM ALU Operations

**File:** `sasm_alu.h`
**Author:** Soham Metha  
**Date:** January 2025

---

## 1.1. Table of Contents

- [1. SASM ALU Operations](#1-sasm-alu-operations)
    - [1.1. Table of Contents](#11-table-of-contents)
    - [1.2. Enums](#12-enums)
        - [1.2.1. **Operation Types**](#121-operation-types)
        - [1.2.2. **Arithmetic Operations**](#122-arithmetic-operations)
        - [1.2.3. **Logical Operations**](#123-logical-operations)
        - [1.2.4. **Shift Operations**](#124-shift-operations)
        - [1.2.5. **Rotate Operations**](#125-rotate-operations)
        - [1.2.6. **Compare Operations**](#126-compare-operations)
    - [1.3. Function Declaration](#13-function-declaration)
        - [1.3.1. Perform Arithmetic Operation](#131-perform-arithmetic-operation)
    - [1.4. TODOs](#14-todos)

---

## 1.2. Enums

### 1.2.1. **Operation Types**

Represents the category of operations that can be performed.

| Operation Type | Description           |
| -------------- | --------------------- |
| ARITHMETIC     | Arithmetic operations |
| LOGICAL        | Logical operations    |
| SHIFT          | Shift operations      |
| ROTATE         | Rotate operations     |
| COMPARE        | Comparison operations |

### 1.2.2. **Arithmetic Operations**

Defines specific arithmetic operations.

| Arithmetic Operations | Description              |
| --------------------- | ------------------------ |
| ADDITION              | Addition operation       |
| SUBTRACT              | Subtraction operation    |
| MULTIPLY              | Multiplication operation |
| DIVIDE                | Division operation       |
| MODULO                | Modulo operation         |

### 1.2.3. **Logical Operations**

Defines specific logical operations.

| Logical Operations | Description |
| ------------------ | ----------- |
| AND                | Logical AND |
| OR                 | Logical OR  |
| XOR                | Logical XOR |
| NOT                | Logical NOT |

### 1.2.4. **Shift Operations**

Defines left and right shift operations.

| Shift Operations | Description |
| ---------------- | ----------- |
| LEFT_SHIFT       | Left shift  |
| RIGHT_SHIFT      | Right shift |

### 1.2.5. **Rotate Operations**

Defines left and right rotate operations.

| Rotate Operations | Description  |
| ----------------- | ------------ |
| LEFT_ROTATE       | Left rotate  |
| RIGHT_ROTATE      | Right rotate |

### 1.2.6. **Compare Operations**

Defines comparison operations.

| Compare Operations | Description                      |
| ------------------ | -------------------------------- |
| EQUAL              | Equal comparison                 |
| NOT_EQUAL          | Not equal comparison             |
| GREATER            | Greater than comparison          |
| LESS               | Less than comparison             |
| GREATER_EQUAL      | Greater than or equal comparison |
| LESS_EQUAL         | Less than or equal comparison    |

---

## 1.3. Function Declaration

### 1.3.1. Perform Arithmetic Operation

**Declaration:**

```c
Error performArithmeticOperation(CPU* cpu, Memory* mem, ArithmeticOperations operation);
```

**Description:**
Executes an arithmetic operation on the top two elements of the CPU stack, updates the stack, and modifies CPU flags based on the result (e.g., carry, overflow, zero, sign, parity).

**Parameters:**

-   `cpu`: Pointer to the `CPU` structure containing registers and the stack.
-   `mem`: Pointer to the `Memory` structure containing stack memory.
-   `operation`: The arithmetic operation to perform, specified using the `ArithmeticOperations` enum.

**Returns:**
An `Error` code indicating success or failure of the operation.

---

## 1.4. TODOs

1. **Implement the following operation types in future development:**
    - `LOGICAL`: Add logic for AND, OR, XOR, and NOT operations.
    - `SHIFT`: Add handling for left and right shift operations.
    - `ROTATE`: Implement left and right rotate operations.
    - `COMPARE`: Include comparison logic for equality, inequality, and relational operators.

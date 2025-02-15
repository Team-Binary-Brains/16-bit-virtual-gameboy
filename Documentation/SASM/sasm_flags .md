# 1. SASM Flags Manipulation

**File:** `sasm_flags.h`  
**Author:** Soham Metha  
**Date:** January 2025

---

## 1.1. Table of Contents

- [1. SASM Flags Manipulation](#1-sasm-flags-manipulation)
    - [1.1. Table of Contents](#11-table-of-contents)
    - [1.2. Flags Enum](#12-flags-enum)
    - [1.3. Flag Manipulation Functions](#13-flag-manipulation-functions)
        - [1.3.1. Halt Flag](#131-halt-flag)
        - [1.3.2. Sign Flag](#132-sign-flag)
        - [1.3.3. Overflow Flag](#133-overflow-flag)
        - [1.3.4. Carry Flag](#134-carry-flag)
        - [1.3.5. Borrow Flag](#135-borrow-flag)
        - [1.3.6. Parity Flag](#136-parity-flag)
        - [1.3.7. Zero Flag](#137-zero-flag)

---

## 1.2. Flags Enum

The `Flags` enumeration defines individual CPU flag bit masks, allowing manipulation of specific flags using bitwise operations.

| Flag     | Bit Mask | Description   |
| -------- | -------- | ------------- |
| Halt     | 1 << 0   | Halt flag     |
| Sign     | 1 << 1   | Sign flag     |
| Overflow | 1 << 2   | Overflow flag |
| Carry    | 1 << 3   | Carry flag    |
| Borrow   | 1 << 4   | Borrow flag   |
| Parity   | 1 << 5   | Parity flag   |
| Zero     | 1 << 6   | Zero flag     |

---

## 1.3. Flag Manipulation Functions

### 1.3.1. Halt Flag

-   **Set Halt Flag**

    ```c
    void setHalt(CPU* cpu, bool halt);
    ```

    Sets the Halt flag in the `CPU` structure based on the given `halt` value.

-   **Get Halt Flag**
    ```c
    bool getHalt(const CPU* cpu);
    ```
    Returns the current state of the Halt flag.

---

### 1.3.2. Sign Flag

-   **Set Sign Flag**

    ```c
    void setSign(CPU* cpu, bool sign);
    ```

    Sets the Sign flag in the `CPU` structure based on the given `sign` value.

-   **Get Sign Flag**
    ```c
    bool getSign(const CPU* cpu);
    ```
    Returns the current state of the Sign flag.

---

### 1.3.3. Overflow Flag

-   **Set Overflow Flag**

    ```c
    void setOverflow(CPU* cpu, bool overflow);
    ```

    Sets the Overflow flag in the `CPU` structure based on the given `overflow` value.

-   **Get Overflow Flag**
    ```c
    bool getOverflow(const CPU* cpu);
    ```
    Returns the current state of the Overflow flag.

---

### 1.3.4. Carry Flag

-   **Set Carry Flag**

    ```c
    void setCarry(CPU* cpu, bool carry);
    ```

    Sets the Carry flag in the `CPU` structure based on the given `carry` value.

-   **Get Carry Flag**
    ```c
    bool getCarry(const CPU* cpu);
    ```
    Returns the current state of the Carry flag.

---

### 1.3.5. Borrow Flag

-   **Set Borrow Flag**

    ```c
    void setBorrow(CPU* cpu, bool borrow);
    ```

    Sets the Borrow flag in the `CPU` structure based on the given `borrow` value.

-   **Get Borrow Flag**
    ```c
    bool getBorrow(const CPU* cpu);
    ```
    Returns the current state of the Borrow flag.

---

### 1.3.6. Parity Flag

-   **Set Parity Flag**

    ```c
    void setParity(CPU* cpu, bool parity);
    ```

    Sets the Parity flag in the `CPU` structure based on the given `parity` value.

-   **Get Parity Flag**
    ```c
    bool getParity(const CPU* cpu);
    ```
    Returns the current state of the Parity flag.

---

### 1.3.7. Zero Flag

-   **Set Zero Flag**

    ```c
    void setZero(CPU* cpu, bool zero);
    ```

    Sets the Zero flag in the `CPU` structure based on the given `zero` value.

-   **Get Zero Flag**
    ```c
    bool getZero(const CPU* cpu);
    ```
    Returns the current state of the Zero flag.



# 1. Universal Definitions for the 16-bit Virtual Gameboy (GBVM) Library

**File:** `univ_defs.h`  
**Author:** Soham Metha  
**Date:** January 2025

The `univ_defs.h` file provides core definitions essential for the functioning of the GBVM library. It ensures consistency and reusability of standard types and constants across the library, which simulates a 16-bit virtual machine.

---

## 1.1. Table of Contents

- [1. Universal Definitions for the 16-bit Virtual Gameboy (GBVM) Library](#1-universal-definitions-for-the-16-bit-virtual-gameboy-gbvm-library)
    - [1.1. Table of Contents](#11-table-of-contents)
    - [1.2. Included Libraries](#12-included-libraries)
    - [1.3. Constants](#13-constants)
        - [1.3.1. Example Usage](#131-example-usage)
    - [1.4. Type Definitions](#14-type-definitions)
    - [1.5. Notes](#15-notes)

---

## 1.2. Included Libraries

The following standard C libraries are included in `univ_defs.h`:

-   `assert.h`
-   `ctype.h`
-   `errno.h`
-   `stdbool.h`
-   `stdint.h`
-   `stdio.h`
-   `stdlib.h`
-   `string.h`

---

## 1.3. Constants

This file defines several constants used for configuring the GBVM's operational limits:

| Constant Name      | Value   | Description                                                                  |
| ------------------ | ------- | ---------------------------------------------------------------------------- |
| `EXECUTION_LIMIT`  | `1024`  | The maximum number of instructions that can be executed in a single session. |
| `PROGRAM_CAPACITY` | `1024`  | The maximum size of the program memory.                                      |
| `STACK_CAPACITY`   | `1024`  | The maximum size of the stack memory.                                        |
| `MAX_WORD`         | `32767` | The maximum value that can be stored in a `Word` type.                       |

### 1.3.1. Example Usage

```c
if (instructionCount > EXECUTION_LIMIT) {
    printf("Error: Execution limit exceeded.\n");
    exit(1);
}
```

---

## 1.4. Type Definitions

The file introduces custom type definitions for the GBVM's memory and processing needs:

| Type Name     | Base Type       | Description                             |
| ------------- | --------------- | --------------------------------------- |
| `Byte`        | `char`          | Represents a single byte.               |
| `Word`        | `short int`     | Represents a 16-bit signed word.        |
| `Double_Word` | `int`           | Represents a 32-bit signed double word. |
| `Quad_Word`   | `long long int` | Represents a 64-bit signed quad word.   |

---

## 1.5. Notes

-   The constants and types are designed to align with the 16-bit architecture of the virtual machine.
-   All custom types (`Byte`, `Word`, etc.) rely on their corresponding standard C types to ensure portability and clarity.
-   Always include `univ_defs.h` in files that require these constants or types.

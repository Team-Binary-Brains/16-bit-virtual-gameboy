# 1. File Operations Functions for the GBVM Library

**File:** `univ_fileops.h`  
**Author:** Soham Metha  
**Date:** January 2025

The `univ_fileops.h` file contains utility functions for performing essential file operations.
All functions display informative error messages and terminate execution if a critical error is encountered.

---

## 1.1. Table of Contents

-   [Function Documentation](#function-documentation)
    -   [openFile](#openfile)
    -   [closeFile](#closefile)
    -   [getFileSize](#getfilesize)
-   [Example Usage](#example-usage)

---

## 1.2. Function Documentation

### 1.2.1. `FILE* openFile(const char* filePath, const char* mode)`

**Description:**  
Opens a file with the specified file path and mode. If the file cannot be opened, an error message is displayed, and the program exits.

**Parameters:**

| Parameter  | Type          | Description                         |
| ---------- | ------------- | ----------------------------------- |
| `filePath` | `const char*` | The path of the file to be opened.  |
| `mode`     | `const char*` | The mode in which to open the file. |

**Returns:**  
A pointer to the opened file (`FILE*`).

**Behavior:**

-   Exits the program if the file cannot be opened.

---

### 1.2.2. `void closeFile(const char* filePath, FILE* file)`

**Description:**  
Closes the specified file. If the file pointer is `NULL`, the function safely returns without performing any operations. If the file cannot be closed, an error message is displayed, and the program exits.

**Parameters:**

| Parameter  | Type          | Description                         |
| ---------- | ------------- | ----------------------------------- |
| `filePath` | `const char*` | The path of the file to be closed.  |
| `file`     | `FILE*`       | A pointer to the file to be closed. |

**Returns:**  
None.

**Behavior:**

-   Safely handles `NULL` file pointers.
-   Exits the program if the file cannot be closed.

---

### 1.2.3. `int getFileSize(FILE* f, const char* filePath)`

**Description:**  
Gets the size of the specified file in bytes. If the file pointer is `NULL` or an error occurs while reading the file, an error message is displayed, and the program exits.

**Parameters:**

| Parameter  | Type          | Description            |
| ---------- | ------------- | ---------------------- |
| `f`        | `FILE*`       | A pointer to the file. |
| `filePath` | `const char*` | The path of the file.  |

**Returns:**  
The size of the file in bytes (`int`).

**Behavior:**

-   Exits the program if the file pointer is `NULL` or an error occurs during size calculation.

---

## 1.3. Example Usage

```c
#include "univ_fileops.h"
#include <stdio.h>

int main() {
    const char* filePath = "example.txt";
    FILE* file = openFile(filePath, "r");

    int size = getFileSize(file, filePath);
    printf("Size of '%s': %d bytes\n", filePath, size);

    closeFile(filePath, file);
    return 0;
}
```

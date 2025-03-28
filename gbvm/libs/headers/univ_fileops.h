/**
 * @file univ_fileops.h
 * @brief Declaration of file operations functions.
 *
 * This file contains the declarations of functions related to file operations.
 * The functions handle error conditions and display appropriate error messages.
 *
 * @author Soham Metha
 * @date January 2025
 *
 * @cite Tsoding Playlist specified in readme
 */

 #pragma once
 #include "univ_defs.h"

/**
 * @brief Opens a file with the specified file path and mode.
 *
 * This function opens a file with the given file path and mode.
 * If the file cannot be opened, an error message is displayed and the program exits.
 *
 * @param filePath The path of the file to be opened.
 * @param mode The mode in which the file should be opened.
 * @return A pointer to the opened file.
 */
FILE* openFile(const char* filePath, const char* mode);

/**
 * @brief Closes the specified file.
 *
 * This function closes the specified file.
 * If the file pointer is NULL, the function returns without doing anything.
 * If the file cannot be closed, an error message is displayed and the program exits.
 *
 * @param filePath The path of the file to be closed.
 * @param file A pointer to the file to be closed.
 */
void closeFile(const char* filePath, FILE* file);

/**
 * @brief Gets the size of the specified file.
 *
 * This function returns the size of the specified file in bytes.
 * If the file pointer is NULL, an error message is displayed and the program exits.
 * If there is an error while reading from the file, an error message is displayed
 * and the program exits.
 *
 * @param f A pointer to the file.
 * @param filePath The path of the file.
 * @return The size of the file in bytes.
 */
int getFileSize(FILE* f, const char* filePath);

/**
 * @file univ_errors.h
 * @brief Contains error handling functions.
 *
 * This file defines various error handling functions.
 * The Error enum defines different error codes that
 * can be used to identify specific types of errors.
 *
 * @author Soham Metha
 * @date January 2025
 */

 #pragma once

 #include "univ_strings.h"
 
 typedef enum {
     ERR_OK = 0,                /**< No error */
     ERR_STACK_OVERFLOW,        /**< Stack overflow error */
     ERR_STACK_UNDERFLOW,       /**< Stack underflow error */
     ERR_DIV_BY_ZERO,           /**< Division by zero error */
     ERR_ILLEGAL_INST,          /**< Illegal instruction error */
     ERR_ILLEGAL_INST_ACCESS,   /**< Illegal instruction access error */
     ERR_ILLEGAL_OPERAND,       /**< Illegal operand error */
     ERR_ILLEGAL_ALU_OPERATION, /**< Illegal ALU operation error */
 } Error;
 
 /**
  * @brief Retrieve C-style string representation of an Error enum value.
  *
  * This function converts an Error enum value to a C-style string representation.
  * If the Error enum value is not recognized, the executable crashes.
  * The crash message will be: 'univ_errors : errorAsCstr : Unreachable'
  *
  * @param error The Error enum value to convert.
  * @return The C-style string representation of the error.
  *
  * @cite Tsoding Playlist specified in readme
  */
 const char* errorAsCstr(const Error*);
 
 /**
  * @brief Displays an error message along with the file path.
  *
  * This function displays an error message along with the file path
  * and exits the program.
  *
  * @param message The error message to display.
  * @param filePath The file path associated with the error.
  */
 void fileErrorDispWithExit(const char*, const char*);
 
 /**
  * @brief Displays an error as a C string message and exits the program.
  *
  * This function displays an execution error message and exits the program.
  *
  * @param error The Error enum value representing the execution error.
  */
 void executionErrorWithExit(const Error*);
 
 /**
  * @brief Displays an error message and exits the program.
  *
  * This function displays an error message and exits the program.
  *
  * @param message The error message to display.
  */
 void displayMsgWithExit(const char*);
 
 /**
  * @brief Displays a string message warning along with a String object.
  *
  * This function displays a string message warning along with a String
  * object and formatting.
  *
  * @param msg The message to display.
  * @param str The String object to display.
  */
 void displayStringMessageError(const char*, String);
 
 /**
  * @brief Displays a debug comment written in the String data.
  *
  * This function displays a debug comment written in the sasm file passed
  * as a String object.
  * The comment is displayed with formatting.
  * The current width limit is 125 characters.
  * The current debug comment identifiers are '#' and ';'.
  *
  * @param s The String object to display as a debug comment.
  */
 void debugCommentDisplay(String*);
 
 void debugMessageDisplay(String* s); 

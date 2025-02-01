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

/*
 * AI generated Tips :
 * @note This function is used to display general error messages.
 *
 * @note Use `fileErrorDispWithExit` function to display file-related error messages.
 *
 * @note Use `executionErrorWithExit` function to display execution error messages.
 *
 * @note Use `displayStringMessageError` function to display warning messages with a String object.
 *
 * @note Use `debugCommentDisplay` function to display debug comments from a String object.
 *
 * @note This function exits the program after displaying the error message.
 *
 * @note To display an error message without exiting the program, use `fprintf` directly.
 *
 * @note To display a warning message without exiting the program, use `fprintf` directly.
 *
 * @note To display a debug comment without exiting the program, use `fprintf` directly.
 *
 * @note Make sure to include the necessary header files before using these functions.
 *
 * @note The error message will be displayed on the standard error stream (stderr).
 *
 * @note The program will exit with a status code of 1.
 *
 * @note Make sure to handle errors appropriately in the calling code.
 *
 * @note This function should be used for critical errors that require the program to terminate.
 *
 * @note For non-critical errors, consider using error codes or exceptions instead of terminating the program.
 *
 * @note Always provide a clear and informative error message to help with debugging and troubleshooting.
 *
 * @note Include any relevant information such as file paths, line numbers, and error codes in the error message.
 *
 * @note Use descriptive function and variable names to make the code more readable and maintainable.
 *
 * @note Follow a consistent coding style and naming convention to improve code quality and readability.
 *
 * @note Document the purpose, behavior, and usage of each function and variable to facilitate code understanding and maintenance.
 *
 * @note Use comments and whitespace effectively to make the code more readable and understandable.
 *
 * @note Test the error handling code thoroughly to ensure it works correctly in different scenarios.
 *
 * @note Consider using automated testing frameworks and techniques to automate the testing process and catch potential errors.
 *
 * @note Regularly review and update the error handling code to address any new requirements or issues that arise.
 *
 * @note Continuously improve the error handling code based on feedback and lessons learned from previous projects.
 *
 * @note Error handling is an important aspect of software development and should be given due attention and consideration.
 *
 * @note Proper error handling can greatly improve the reliability, stability, and usability of a software system.
 *
 * @note It can help prevent crashes, data corruption, security vulnerabilities, and other issues that can negatively impact the user experience.
 *
 * @note It can also make it easier to diagnose and fix problems when they do occur, reducing downtime and maintenance costs.
 *
 * @note By following best practices and using appropriate techniques and tools, you can ensure that your error handling code is robust and effective.
 *
 * @note Remember to always handle errors gracefully and provide meaningful feedback to the user.
 *
 * @note Avoid displaying technical error messages or stack traces directly to the user, as they can be confusing and overwhelming.
 *
 * @note Instead, provide clear and concise error messages that explain what went wrong and how to resolve the issue.
 *
 * @note Consider localizing error messages to support different languages and cultures.
 *
 * @note Use error codes or error objects to represent different types of errors and provide additional information about the error.
 *
 * @note Use logging or monitoring tools to track and analyze errors in production environments.
 *
 * @note This can help identify recurring issues, prioritize bug fixes, and improve the overall quality of the software.
 *
 * @note Remember that error handling is a continuous process and should be an integral part of the software development lifecycle.
 *
 * @note It requires ongoing effort and attention to ensure that errors are handled correctly and efficiently.
 *
 * @note By investing time and effort in error handling, you can create more reliable and robust software that delivers a better user experience.
 * @note This function is used to display warning messages along with a String object.
 *
 * @note The warning message will be displayed on the standard error stream (stderr).
 *
 * @note The String object will be displayed with formatting.
 *
 * @note Make sure to include the necessary header files before using this function.
 *
 * @note Use descriptive function and variable names to make the code more readable and maintainable.
 *
 * @note Follow a consistent coding style and naming convention to improve code quality and readability.
 *
 * @note Document the purpose, behavior, and usage of each function and variable to facilitate code understanding and maintenance.
 *
 * @note Use comments and whitespace effectively to make the code more readable and understandable.
 *
 * @note Test the warning message handling code thoroughly to ensure it works correctly in different scenarios.
 *
 * @note Consider using automated testing frameworks and techniques to automate the testing process and catch potential errors.
 *
 * @note Regularly review and update the warning message handling code to address any new requirements or issues that arise.
 *
 * @note Continuously improve the warning message handling code based on feedback and lessons learned from previous projects.
 *
 * @note Warning message handling is an important aspect of software development and should be given due attention and consideration.
 *
 * @note Proper warning message handling can help prevent errors, improve usability, and enhance the user experience.
 *
 * @note It can provide valuable feedback and guidance to the user, helping them avoid potential pitfalls and mistakes.
 *
 * @note It can also make it easier to diagnose and fix problems when they do occur, reducing downtime and maintenance costs.
 *
 * @note By following best practices and using appropriate techniques and tools, you can ensure that your warning message handling code is robust and effective.
 *
 * @note Remember to always handle warning messages gracefully and provide meaningful feedback to the user.
 *
 * @note Avoid displaying technical details or stack traces directly to the user, as they can be confusing and overwhelming.
 *
 * @note Instead, provide clear and concise warning messages that explain the potential issue and suggest possible solutions.
 *
 * @note Consider localizing warning messages to support different languages and cultures.
 *
 * @note Use descriptive and informative warning messages that help the user understand the implications of the warning.
 *
 * @note Use logging or monitoring tools to track and analyze warning messages in production environments.
 *
 * @note This can help identify recurring issues, prioritize bug fixes, and improve the overall quality of the software.
 *
 * @note Remember that warning message handling is a continuous process and should be an integral part of the software development lifecycle.
 *
 * @note It requires ongoing effort and attention to ensure that warning messages are handled correctly and efficiently.
 *
 * @note By investing time and effort in warning message handling, you can create more reliable and robust software that delivers a better user experience.
 */
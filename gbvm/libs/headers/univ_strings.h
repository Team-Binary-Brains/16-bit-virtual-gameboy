/**
 * @file univ_strings.h
 * @brief Declaration of string manipulation functions.
 *
 * This file contains the declarations of various string manipulation functions.
 *
 * @author Soham Metha
 * @date January 2025
 */

#pragma once

#include "univ_defs.h"

/**
 * @struct String
 * @brief Represents a string with its length and data.
 *
 * The String struct contains two members: length and data.
 * @param length The length of the string.
 * @param data A pointer to the character data of the string.
 */
struct String {
    size_t length;
    const char* data;
};

#define strFmt "%.*s"
#define strArg(str) (int)(str).length, (str).data

/**
 * @brief Removes leading whitespace characters from a string.
 *
 * This function removes any leading whitespace characters
 * from the input string and returns the modified string.
 *
 * @param s The input string.
 * @return The string with leading whitespace characters removed.
 *
 * @cite Tsoding Playlist specified in readme
 */
String ltrim(String);

/**
 * @brief Removes trailing whitespace characters from a string.
 *
 * This function removes any trailing whitespace characters
 * from the input string and returns the modified string.
 *
 * @param s The input string.
 * @return The string with trailing whitespace characters removed.
 *
 * @cite Tsoding Playlist specified in readme
 */
String rtrim(String);

/**
 * @brief Removes leading and trailing whitespace characters from a string.
 *
 * This function removes any leading and trailing whitespace characters
 * from the input string and returns the modified string.
 *
 * @param s The input string.
 * @return The string with leading and trailing whitespace characters removed.
 *
 * @cite Tsoding Playlist specified in readme
 */
String trim(String);

/**
 * @brief Splits a string into 2 parts at the first occurrence of a specific character.
 *
 * This function splits the input string into two parts at the first occurrence
 * of the specified character.
 * The first part of the string is returned, and the input string is modified
 * to contain only the second part.
 *
 * @param s The input string.
 * @param c The character at which to split the string.
 * @return The first part of the string before the specified character.
 *
 * @cite Tsoding Playlist specified in readme
 */
String splitStrByChar(String*, char);

/**
 * @brief Converts a string to an integer.
 *
 * This function converts the input string to an integer and returns the integer value.
 *
 * @param s The input string.
 * @return The integer value represented by the string.
 */
uint64_t convertStrToInt(String);

/**
 * @brief Prints a string to the standard output.
 *
 * This function prints the input string to the standard output.
 *
 * @param s The string to be printed.
 */
void printString(String s);

bool compareStr(String a, String b);

String convertCstrToStr(const char*);

bool getIndexOf(String str, char c, size_t* index);

bool startsWith(String str, String prefix);

String splitStrByLen(String* str, size_t n);

String splitStrByCondition(String* str, bool (*predicate)(char x));

String splitStrByLenReversed(String* str, size_t n);

bool endsWith(String str, String expected_suffix);
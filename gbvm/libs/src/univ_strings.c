#include "univ_strings.h"

String convertCstrToStr(const char* cstr)
{
    return (String) {
        .length = strlen(cstr),
        .data = (cstr)
    };
}

String ltrim(String s)
{
    size_t i = 0;
    while (i < s.length && isspace(s.data[i])) {
        i++;
    }
    return (String) {
        .length = s.length - i,
        .data = s.data + i
    };
}

String rtrim(String s)
{
    size_t i = s.length;
    while (i > 0 && isspace(s.data[i - 1])) {
        i--;
    }
    return (String) {
        .length = i,
        .data = s.data
    };
}

String trim(String s)
{
    return rtrim(ltrim(s));
}

String splitStr(String* s, char c)
{
    size_t i = 0;
    while (i < s->length && s->data[i] != c) {
        i++;
    }
    String res = {

        .length = i,
        .data = s->data
    };

    s->length -= i;
    s->data += i;

    if (i < s->length) {
        s->length -= 1;
        s->data += 1;
    }

    return res;
}

uint64_t convertStrToInt(String s)
{
    uint64_t val = 0;
    size_t i = 0;

    while (i < s.length && isdigit(s.data[i])) {
        val = val * 10 + s.data[i] - '0';
        i++;
    }
    return val;
}

void printString(String s)
{
    printf("%.*s\n", (int)(s.length), s.data);
}

bool compareStr(String a, String b)
{
    if (a.length != b.length) {
        return false;
    } else {
        return memcmp(a.data, b.data, a.length) == 0;
    }
}

// TODO: add ability to process hex inputs

bool parseStrHex(String str, uint64_t* output)
{
    uint64_t result = 0;

    for (size_t i = 0; i < str.length; ++i) {
        const char x = str.data[i];
        result = result * 0x10;

        if ('0' <= x && x <= '9')
            result = result + (uint64_t)(x - '0');
        else if ('A' <= x && x <= 'F')
            result = result + (uint64_t)(x + 10 - 'A');
        else
            return false;
    }

    if (output)
        *output = result;

    return true;
}

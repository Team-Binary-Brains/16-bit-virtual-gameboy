#include "gbvm_strings.h"

String ltrim(String s)
{
    size_t i = 0;
    while (i < s.parts && isspace(s.data[i])) {
        i++;
    }
    return (String) {
        .parts = s.parts - i,
        .data = s.data + i
    };
}

String rtrim(String s)
{
    size_t i = 0;
    while (i < s.parts && isspace(s.data[s.parts - i - 1])) {
        i++;
    }
    return (String) {
        .parts = s.parts - i,
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
    while (i < s->parts && s->data[i] != c) {
        i++;
    }
    String res = {
        .parts = i,
        .data = s->data
    };

    if (i < s->parts) {
        s->parts -= i + 1;
        s->data += i + 1;
    } else {
        s->parts -= i;
        s->data += i;
    }
    return res;
}

Word strToWord(String s)
{
    Word val = 0;
    size_t i = 0;

    while (i < s.parts && isdigit(s.data[i])) {
        val = val * 10 + s.data[i] - '0';
        i++;
    }
    return val;
}

void printString(String s)
{
    printf("%.*s\n", (int)(s.parts), s.data);
}
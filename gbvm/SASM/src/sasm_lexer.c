#include "sasm_assembler.h"

bool loadSasmFileIntoSasmLexer(SasmLexer* lineInterpreter, Region* region, String filePath)
{
    assert(lineInterpreter);
    if (loadFileIntoRegionStr(region, filePath, &lineInterpreter->source) < 0) {
        return false;
    }

    lineInterpreter->location.filePath = filePath;

    return true;
}

bool fetchCachedLineFromSasmLexer(SasmLexer* lineInterpreter, Line* output)
{
    if (lineInterpreter->hasCachedToken) {
        if (output) {
            *output = lineInterpreter->cachedToken;
        }
        return true;
    }

    String line = { 0 };
    do {
        line = trim(splitStrByChar(&lineInterpreter->source, '\n'));
        line = trim(splitStrByChar(&line, COMMENT_SYMBOL));
        lineInterpreter->location.lineNumber += 1;
    } while (line.length == 0 && lineInterpreter->source.length > 0);

    if (line.length == 0 && lineInterpreter->source.length == 0) {
        return false;
    }

    Line result = { 0 };
    result.location = lineInterpreter->location;
    if (startsWith(line, convertCstrToStr("%"))) {
        splitStrByLen(&line, 1);
        result.kind = LINE_DIRECTIVE;
        result.value.directive.name = trim(splitStrByChar(&line, ' '));
        result.value.directive.body = trim(line);
    } else if (endsWith(line, convertCstrToStr(":"))) {
        result.kind = LINE_LABEL;
        result.value.label.name = trim(splitStrByChar(&line, ':'));
        // printString(result.value.label.name);
    } else {
        result.kind = LINE_INSTRUCTION;
        result.value.instruction.name = trim(splitStrByChar(&line, ' '));
        result.value.instruction.operand = trim(line);
    }

    if (output) {
        *output = result;
    }

    lineInterpreter->hasCachedToken = true;
    lineInterpreter->cachedToken = result;

    return true;
}

bool moveSasmLexerToNextLine(SasmLexer* lineInterpreter, Line* output)
{
    if (fetchCachedLineFromSasmLexer(lineInterpreter, output)) {
        lineInterpreter->hasCachedToken = false;
        return true;
    }

    return false;
}

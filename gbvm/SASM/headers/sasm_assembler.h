/**
 * @file sasm_assembler.h
 * @brief This file contains functions for assembling and disassembling programs in a custom assembly language.
 *
 * The functions in this file are used to manipulate custom assembly code and bytecode.
 */

#pragma once

#include "sasm_instructions.h"
#include "univ_errors.h"
#include "univ_malloc.h"

#define $instructionCount ->prog.instruction_count
#define $instructions ->prog.instructions
#define DEFERRED_ASSERTS_CAPACITY 1024
#define STRING_LENGTHS_CAPACITY 1024
#define INCLUDE_PATHS_CAPACITY 1024
#define MAX_PRECEDENCE 2
#define Token_Fmt "%s"
#define Token_Arg(token) token_kind_name((token).kind)

enum StmtType {
    STMT_INST,
    STMT_LABEL,
    STMT_CONST,
    STMT_INCLUDE,
    STMT_ENTRY,
    STMT_BLOCK,
    STMT_SCOPE,
};

enum BindingStatus {
    BIND_STATUS_UNEVALUATED = 0,
    BIND_STATUS_EVALUATING,
    BIND_STATUS_EVALUATED,
    BIND_STATUS_DEFERRED,
};

enum EvalStatus {
    EVAL_STATUS_OK = 0,
    EVAL_STATUS_DEFERRED
};

enum TokenType {
    TOKEN_TYPE_STR,
    TOKEN_TYPE_CHAR,
    TOKEN_TYPE_NUMBER,
    TOKEN_TYPE_NAME,
    TOKEN_TYPE_OPEN_PAREN,
    TOKEN_TYPE_CLOSING_PAREN,
    TOKEN_TYPE_COMMA,
    TOKEN_TYPE_REGISTER
};

enum LineType {
    LINE_INSTRUCTION = 0,
    LINE_LABEL,
    LINE_DIRECTIVE,
};

enum ExprType {
    EXPR_BINDING,
    EXPR_LIT_INT,
    EXPR_LIT_FLOAT,
    EXPR_LIT_CHAR,
    EXPR_LIT_STR,
    EXPR_FUNCALL,
    EXPR_REG,
};

enum BindingType {
    BIND_TYPE_FLOAT,
    BIND_TYPE_UINT,
    BIND_TYPE_MEM_ADDR,
    BIND_TYPE_INST_ADDR,
};

typedef struct SasmLexer SasmLexer;                         // Lexer ( performs "Get Next Line", "Move To Next Line", "Load File In")
typedef struct Line Line;                                   // └───>  Line
typedef enum LineType LineType;                             //        └───> LineType
typedef union LineValue LineValue;                          //        └───> LineValue
typedef struct SasmDirectiveLine SasmDirectiveLine;         //              └───> Directives
typedef struct SasmLabelLine SasmLabelLine;                 //              └───> Labels
typedef struct SasmInstructionLine SasmInstructionLine;     //              └───> Instructions
typedef struct Tokenizer Tokenizer;                         // Tokenizer ( performs "Get Next Token", "Move To Next Token", "Load Line In")
typedef struct Token Token;                                 // └───> Token
typedef enum TokenType TokenType;                           //       └───> TokenType
typedef struct CodeBlock CodeBlock;                         // CodeBlock ( Linked List Of Statements )
typedef struct StmtNode StmtNode;                           // └───> StmtNode
typedef struct Stmt Stmt;                                   //       └───> Statement
typedef enum StmtType StmtType;                             //             └───> StatementType
typedef union StmtValue StmtValue;                          //             └───> StatementValue
typedef struct LabelStmt LabelStmt;                         //                   └───> Labels
typedef struct ConstStmt ConstStmt;                         //                   └───> Constant Declaration
typedef struct IncludeStmt IncludeStmt;                     //                   └───> Include
typedef struct EntryStmt EntryStmt;                         //                   └───> Entry Point Declaration
typedef struct InstStmt InstStmt;                           //                   └───> Instructions
typedef struct Expr Expr;                                   //                         └───>  Expression (operands passed)
typedef enum ExprType ExprType;                             //                                └───> Expression type
typedef union ExprValue ExprValue;                          //                                └───> Expression Value
typedef struct Funcall Funcall;                             //                                      └───> Compile Time Functions
typedef struct FuncallArg FuncallArg;                       //                                            └───> Arguments for Compile Time Functions (Linked List of Expressions)
typedef struct EvalResult EvalResult;                       // Evaluation Result
typedef enum EvalStatus EvalStatus;                         // └───> Evaluation Status
typedef struct Binding Binding;                             // └───> Binding Being Evaluated
typedef enum BindingType BindingType;                       //       └───> Binding Type
typedef enum BindingStatus BindingStatus;                   //       └───> Binding Status

typedef struct Sasm Sasm;
typedef struct StringLength StringLength;
typedef struct Scope Scope;
typedef struct UnresolvedOperand UnresolvedOperand;
typedef struct DeferredAssert DeferredAssert;
typedef struct DeferredEntry DeferredEntry;

typedef struct __attribute__((__packed__)) Metadata Metadata;

union ExprValue {
    String binding;
    uint64_t lit_int;
    double lit_float;
    char lit_char;
    String lit_str;
    Funcall* funcall;
    uint64_t reg_id;
};

struct Expr {
    ExprType type;
    ExprValue value;
};

struct FuncallArg {
    FuncallArg* next;
    Expr value;
};

struct Funcall {
    String name;
    FuncallArg* args;
};

struct SasmInstructionLine {
    String name;
    String operand;
    String operand2;
};

struct SasmLabelLine {
    String name;
};

struct SasmDirectiveLine {
    String name;
    String body;
};

union LineValue {
    SasmInstructionLine instruction;
    SasmLabelLine label;
    SasmDirectiveLine directive;
};

struct Line {
    LineType kind;
    LineValue value;
    FileLocation location;
};

struct SasmLexer {
    String source;
    FileLocation location;
    Line cachedToken;
    bool hasCachedToken;
};

struct InstStmt {
    Opcode type;
    Expr operand;
    Expr operand2;
};

struct LabelStmt {
    String name;
};

struct ConstStmt {
    String name;
    Expr value;
};

struct IncludeStmt {
    String path;
};

struct EntryStmt {
    Expr value;
};

union StmtValue {
    InstStmt inst;
    LabelStmt label;
    ConstStmt constant;
    IncludeStmt include;
    EntryStmt entry;
    StmtNode* block;
    StmtNode* scope;
};

struct Stmt {
    StmtType kind;
    StmtValue value;
    FileLocation location;
};

struct StmtNode {
    Stmt statement;
    StmtNode* next;
};

struct CodeBlock {
    StmtNode* begin;
    StmtNode* end;
};

struct Token {
    TokenType type;
    String text;
};

struct Tokenizer {
    String source;
    Token cachedToken;
    bool hasCachedToken;
};

struct Binding {
    BindingType type;
    String name;
    QuadWord value;
    Expr expr;
    BindingStatus status;
    FileLocation location;
};

struct StringLength {
    InstAddr addr;
    uint64_t length;
};

struct EvalResult {
    EvalStatus status;
    Binding* unresolvedBinding;
    QuadWord value;
    BindingType type;
};

struct Scope {
    Scope* previous;

    Binding bindings[BINDINGS_CAPACITY];
    size_t bindingsCnt;
};

struct UnresolvedOperand {
    InstAddr addr;
    Expr expr;
    FileLocation location;
    Scope* scope;
};

struct DeferredEntry {
    String bindingName;
    FileLocation location;
    Scope* scope;
};

struct Sasm {
    Scope* scope;
    Binding bindings[BINDINGS_CAPACITY];
    size_t bindingCount;

    UnresolvedOperand symbols[LABELS_CAPACITY];
    size_t symbolsCount;

    DeferredEntry deferredEntry;

    Program prog;

    bool hasEntry;
    InstAddr entry;
    FileLocation entryLocation;

    StringLength stringLens[STRING_LENGTHS_CAPACITY];
    size_t strLensCnt;

    Byte memory[MEMORY_CAPACITY];
    size_t memorySize;
    size_t memoryCapacity;

    Region region;

    size_t includeLevel;
    FileLocation includeLocation;

    String includePaths[INCLUDE_PATHS_CAPACITY];
    size_t includePathsCnt;
};

struct Metadata {
    DoubleWord magic;
    Word version;
    DataEntry programSize;
    DataEntry entry;
    DataEntry memorySize;
    DataEntry memoryCapacity;
    DataEntry externalsSize;
};

void translateSasmRootFile(Sasm* sasm, String inputFilePath);
void generateSmExecutable(Sasm* sasm, const char* filePath);
void translateSasmFile(Sasm* sasm, String inputFilePath);
void loadSmExecutableIntoSasm(Sasm* sasm, const char* filePath);

void pushUnresolvedOperand(Sasm* sasm, InstAddr addr, Expr expr, FileLocation location);
void pushIncludePath(Sasm* sasm, const char* path);
void bindUnresolvedLocalScope(Scope* scope, String name, BindingType type, FileLocation location);
void bindExprLocalScope(Scope* scope, String name, Expr expr, FileLocation location);
Binding* resolveBinding(Sasm* sasm, String name);
EvalResult evaluateBinding(Sasm* sasm, Binding* binding);

bool loadSasmFileIntoSasmLexer(SasmLexer* lineInterpreter, Region* arena, String filePath);
bool fetchCachedLineFromSasmLexer(SasmLexer* lineInterpreter, Line* output);
bool moveSasmLexerToNextLine(SasmLexer* lineInterpreter, Line* output);

Expr parsePrimaryOfSasmTokens(Region* arena, Tokenizer* tokenizer, FileLocation location);
CodeBlock getCodeBlockFromLines(Region* arena, SasmLexer* lineInterpreter);

bool fetchCachedSasmTokenFromSasmTokenizer(Tokenizer* tokenizer, Token* output, FileLocation location);
bool moveSasmTokenizerToNextToken(Tokenizer* tokenizer, Token* token, FileLocation location);
Tokenizer loadStringIntoTokenizer(String source);

const char* getNameOfBindType(BindingType type);
const char* getTokenName(TokenType type);

EvalResult resultOK(QuadWord value, BindingType type);
EvalResult resultUnresolved(Binding* unresolvedBinding);

FuncallArg* parseFuncallArgs(Region* region, Tokenizer* tokenizer, FileLocation location);
EvalResult evaluateExpression(Sasm* sasm, Expr expr, FileLocation location);

int dumpExprAsAST(FILE* stream, Expr expr, int* counter);
int dumpStmtNodeAsAST(FILE* stream, StmtNode* stmtNode, int* counter, int* blockNo);
int dumpStatementAsAST(FILE* stream, Stmt statement, int* counter, int* blockNo);
void generateASTPng(String inputFilePath, StmtNode* start);
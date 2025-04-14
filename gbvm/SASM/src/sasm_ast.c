#include "sasm_assembler.h"

static bool isPath(char x)
{
    return isalnum(x);
}

int dumpExprAsAST(FILE* stream, Expr expr, int* counter)
{
    int id = (*counter)++;

    switch (expr.type) {
    case EXPR_BINDING:
        fprintf(stream, "Expr_%d [shape=ellipse style=filled fillcolor=lightgoldenrod1 fontname=\"Courier\" label=\"" strFmt "\"]\n",
            id, strArg(expr.value.binding));
        break;
    case EXPR_LIT_INT:
        fprintf(stream, "Expr_%d [shape=note style=filled fillcolor=lightblue fontname=\"Courier\" label=\"%" PRIu64 "\"]\n",
            id, expr.value.lit_int);
        break;
    case EXPR_LIT_FLOAT:
        fprintf(stream, "Expr_%d [shape=note style=filled fillcolor=lightblue fontname=\"Courier\" label=\"%lf\"]\n",
            id, expr.value.lit_float);
        break;
    case EXPR_LIT_CHAR:
        fprintf(stream, "Expr_%d [shape=note style=filled fillcolor=lightblue fontname=\"Courier\" label=\"'%c'\"]\n",
            id, expr.value.lit_char);
        break;
    case EXPR_LIT_STR:
        fprintf(stream, "Expr_%d [shape=note style=filled fillcolor=lightblue fontname=\"Courier\" label=\"\\\"" strFmt "\\\"\"]\n",
            id, strArg(expr.value.lit_str));
        break;
    case EXPR_FUNCALL:
        fprintf(stream, "Expr_%d [shape=hexagon style=filled fillcolor=lightpink fontname=\"Courier\" label=\"" strFmt "\"]\n",
            id, strArg(expr.value.funcall->name));

        for (FuncallArg* arg = expr.value.funcall->args;
            arg != NULL;
            arg = arg->next) {
            int childId = dumpExprAsAST(stream, arg->value, counter);
            fprintf(stream, "Expr_%d -> Expr_%d\n", id, childId);
        }
        break;

    case EXPR_REG:
        fprintf(stream, "Expr_%d [shape=cylinder style=filled fillcolor=lightyellow fontname=\"Courier\" label=\"Reference( %ld )\"]\n",
            id, expr.value.reg_id);
        break;
    case EXPR_REG_INLINE:
        fprintf(stream, "Expr_%d [shape=cylinder style=filled fillcolor=lightyellow fontname=\"Courier\" label=\"Value( %ld )\"]\n",
            id, expr.value.reg_id);
        break;
    }

    return id;
}

int dumpStmtNodeAsAST(FILE* stream, StmtNode* stmtNode, int* counter)
{
    int id = (*counter)++;

    fprintf(stream, "subgraph cluster_%d {\n", id);
    fprintf(stream, "label=\"Code Block %d \";\n", id);
    fprintf(stream, "style=filled;\n");
    fprintf(stream, "color=gray;\n");
    fprintf(stream, "fillcolor=whitesmoke;\n");
    fprintf(stream, "fontname=\"Courier\";\n");

    int block_id = -1;

    while (stmtNode) {
        int next_id = dumpStatementAsAST(stream, stmtNode->statement, counter);

        if (block_id >= 0) {
            fprintf(stream, "Expr_%d -> Expr_%d;\n", block_id, next_id);
        }

        block_id = next_id;
        stmtNode = stmtNode->next;
    }

    fprintf(stream, "}\n");
    return id;
}

int dumpStatementAsAST(FILE* stream, Stmt statement, int* counter)
{
    switch (statement.kind) {
    case STMT_INST:
        {
            int id = (*counter)++;
            Opcode type = statement.value.inst.type;
            OpcodeDetails details = getOpcodeDetails(type);

            fprintf(stream, "Expr_%d [shape=component style=filled fillcolor=palegreen fontname=\"Courier\" label=\"%s\"]\n", id, details.name);

            if (details.has_operand) {
                int childId = dumpExprAsAST(stream, statement.value.inst.operand, counter);
                fprintf(stream, "Expr_%d -> Expr_%d [style=dotted]\n", id, childId);
            }

            if (details.has_operand2) {
                int childId = dumpExprAsAST(stream, statement.value.inst.operand2, counter);
                fprintf(stream, "Expr_%d -> Expr_%d [style=dotted]\n", id, childId);
            }
            return id;
        }
        break;

    case STMT_LABEL:
        {
            int id = (*counter)++;
            String name = statement.value.label.name;
            fprintf(stream, "Expr_%d [shape=trapezium style=filled fillcolor=lightgray fontname=\"Courier\" label=\"" strFmt "\"]\n",
                id, strArg(name));
            return id;
        }
        break;

    case STMT_CONST:
        {
            int id = (*counter)++;
            String name = statement.value.constant.name;
            Expr value = statement.value.constant.value;

            fprintf(stream, "Expr_%d [shape=note style=filled fillcolor=thistle fontname=\"Courier\" label=\"%%const " strFmt "\"]\n",
                id, strArg(name));
            int childId = dumpExprAsAST(stream, value, counter);
            fprintf(stream, "Expr_%d -> Expr_%d [style=dotted]\n", id, childId);
            return id;
        }
        break;

    case STMT_INCLUDE:
        {
            int id = (*counter)++;
            int childId = (*counter)++;

            String path = statement.value.include.path;
            fprintf(stream, "Expr_%d [shape=folder style=filled fillcolor=wheat fontname=\"Courier\" label=\"%%include\"]\n", id);
            fprintf(stream, "Expr_%d [shape=box style=filled fillcolor=wheat fontname=\"Courier\" label=\"" strFmt "\"]\n", childId, strArg(path));
            fprintf(stream, "Expr_%d -> Expr_%d [style=dotted]\n", id, childId);
            return id;
        }
        break;

    case STMT_ENTRY:
        {
            int id = (*counter)++;
            fprintf(stream, "Expr_%d [shape=pentagon style=filled fillcolor=plum1 fontname=\"Courier\" label=\"%%entry\"]\n", id);
            int childId = dumpExprAsAST(stream, statement.value.entry.value, counter);
            fprintf(stream, "Expr_%d -> Expr_%d [style=dotted]\n", id, childId);
            return id;
        }
        break;

    case STMT_BLOCK:
    case STMT_SCOPE:
        {
            int oldCnt = dumpStmtNodeAsAST(stream, statement.value.block, counter);
            fprintf(stream, "Expr_%d [shape=note style=filled fillcolor=lightpink fontname=\"Courier\" label=\" Goes To Code Block %d\"]\n", oldCnt, oldCnt);
            return oldCnt;
        }
        break;

    default:
        {
            assert(false && "dumpStatementAsAST: unreachable");
            exit(1);
        }
    }
}

void generateASTPng(String inputFilePath, StmtNode* start)
{
    String tmp = inputFilePath;
    tmp = splitStrByCondition(&tmp, isPath);

    char buffer[100];
    snprintf(buffer, sizeof(buffer), strFmt ".txt", strArg(tmp));
    FILE* out = openFile(buffer, "w");

    int ID = 0;
    fprintf(out, "digraph " strFmt " {\n", strArg(tmp));
    dumpStmtNodeAsAST(out, start, &ID);
    fprintf(out, "}\n");

    closeFile(out, buffer);

    snprintf(buffer, sizeof(buffer), "dot -Tpng " strFmt ".txt -o " strFmt ".png", strArg(tmp), strArg(tmp));
    if (system(buffer) != 0) {
        displayMsgWithExit("Disassembly Failed");
    }
}

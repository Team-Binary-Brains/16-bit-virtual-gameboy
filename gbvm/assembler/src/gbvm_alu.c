#include "global_definitions.h"
#include "gbvm_alu.h"
#include "gbvm_flags.h"

Error performArithmeticOperation(CPU* cpu, Memory* mem, ArithmeticOperations operation)
{
    if (cpu->registers.SP < 2) {
        return ERR_STACK_UNDERFLOW;
    }
    Word a = mem->stack[cpu->registers.SP - 2];
    Word b = mem->stack[cpu->registers.SP - 1];

    if (operation == SUBTRACT && b < 0) {
        operation = ADDITION;
        b = -b;
    }
    if (operation == ADDITION && (a < 0 || b < 0)) {
        operation = SUBTRACT;
        if (a < 0) {
            Word temp = -a;
            a = b;
            b = temp;
        }
    }
    switch (operation) {
    case ADDITION:
        mem->stack[cpu->registers.SP - 2] = a + b;
        bool addcndn = (b > 0 && a > MAX_WORD - b);
        setCarry(cpu, addcndn);
        setOverflow(cpu, addcndn);
        break;

    case SUBTRACT:
        mem->stack[cpu->registers.SP - 2] = a - b;
        setBorrow(cpu, (b > a));
        break;

    case MULTIPLY:
        mem->stack[cpu->registers.SP - 2] = a * b;
        setOverflow(cpu, (b != 0 && a > MAX_WORD / b));
        break;

    case DIVIDE:
        if (b == 0) {
            return ERR_DIV_BY_ZERO;
        }
        mem->stack[cpu->registers.SP - 2] = a / b;
        break;

    case MODULO:
        if (b == 0) {
            return ERR_DIV_BY_ZERO;
        }
        mem->stack[cpu->registers.SP - 2] = a % b;
        break;

    default:
        return ERR_ILLEGAL_ALU_OPERATION;
    }
    setZero(cpu, (mem->stack[cpu->registers.SP - 2] == 0));
    setSign(cpu, (mem->stack[cpu->registers.SP - 2] < 0));
    setParity(cpu, (mem->stack[cpu->registers.SP - 2] % 2 == 0));

    cpu->registers.SP -= 1;
    return ERR_OK;
}

/*
TODO: Implement the following cases
case LOGICAL
case SHIFT
case ROTATE
case COMPARE
*/
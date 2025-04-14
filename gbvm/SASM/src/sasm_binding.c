#include "sasm_assembler.h"

Binding* resolveBindingLocalScope(Scope* scope, String name)
{
    for (size_t i = 0; i < scope->bindingsCnt; ++i) {
        if (compareStr(scope->bindings[i].name, name)) {
            return &scope->bindings[i];
        }
    }

    return NULL;
}

void pushUnresolvedOperand(Sasm* sasm, InstAddr addr, Expr expr, FileLocation location)
{
    assert(sasm->symbolsCount < LABELS_CAPACITY);
    sasm->symbols[sasm->symbolsCount++] = (UnresolvedOperand) {
        .addr = addr,
        .expr = expr,
        .location = location,
        .scope = sasm->scope
    };
}

void bindUnresolvedLocalScope(Scope* scope, String name, BindingType type, FileLocation location)
{
    assert(scope->bindingsCnt < BINDINGS_CAPACITY);

    Binding* existing = resolveBindingLocalScope(scope, name);
    if (existing) {
        fprintf(stderr,
            FLFmt ": ERROR: name `" strFmt "` is already bound\n",
            FLArg(location),
            strArg(name));
        fprintf(stderr,
            FLFmt ": NOTE: first binding is located here\n",
            FLArg(existing->location));
        exit(1);
    }

    scope->bindings[scope->bindingsCnt++] = (Binding) {
        .name = name,
        .status = BIND_STATUS_DEFERRED,
        .type = type,
        .location = location,
        .value.u64 = 0
    };
}

void bindExprLocalScope(Scope* scope, String name, Expr expr, FileLocation location)
{
    assert(scope->bindingsCnt < BINDINGS_CAPACITY);

    Binding* existing = resolveBindingLocalScope(scope, name);
    if (existing) {
        fprintf(stderr,
            FLFmt ": ERROR: name `" strFmt "` is already bound\n",
            FLArg(location),
            strArg(name));
        fprintf(stderr,
            FLFmt ": NOTE: first binding is located here\n",
            FLArg(existing->location));
        exit(1);
    }

    scope->bindings[scope->bindingsCnt++] = (Binding) {
        .name = name,
        .expr = expr,
        .location = location,
    };
}

Binding* resolveBinding(Sasm* sasm, String name)
{
    for (Scope* scope = sasm->scope; scope != NULL; scope = scope->previous) {
        Binding* binding = resolveBindingLocalScope(scope, name);
        if (binding) {
            return binding;
        }
    }

    return NULL;
}

EvalResult evaluateBinding(Sasm* sasm, Binding* binding)
{
    switch (binding->status) {
    case BIND_STATUS_UNEVALUATED:
        binding->status = BIND_STATUS_EVALUATING;
        EvalResult result = evaluateExpression(sasm, binding->expr, binding->location);
        binding->status = BIND_STATUS_EVALUATED;

        if (result.status == EVAL_STATUS_OK) {
            binding->type = result.type;
            binding->value = result.value;
        }

        return result;
    case BIND_STATUS_EVALUATING:
        fprintf(stderr, FLFmt ": ERROR: cycling binding definition.\n",
            FLArg(binding->location));
        exit(1);
    case BIND_STATUS_EVALUATED:
        return resultOK(binding->value, binding->type);
    case BIND_STATUS_DEFERRED:
        return resultUnresolved(binding);

    default:
        assert(false && "evaluateBinding: unreachable");
        exit(1);
    }
}

const char* getNameOfBindType(BindingType type)
{
    switch (type) {
    case BIND_TYPE_FLOAT:
        return "Float";
    case BIND_TYPE_UINT:
        return "Unsigned_Int";
    case BIND_TYPE_MEM_ADDR:
        return "Mem_Addr";
    case BIND_TYPE_INST_ADDR:
        return "Inst_Addr";
    default:
        assert(false && "type_name: unreachable");
        exit(1);
    }
}

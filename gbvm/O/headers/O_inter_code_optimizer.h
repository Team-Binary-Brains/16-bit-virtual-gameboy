#pragma once
// Main driver: runs all optimization passes.
TACInstruction** optimizeCode(TACInstruction**);

// Local optimizations.
void peepholeOptimization(void);
void constantFoldingOptimization(void);
void constantPropagation(void);
void copyPropagation(void);
void algebraicSimplification(void);
void deadCodeElimination(void);
void strengthReduction(void);
void commonSubexpressionElimination(void);

// Global / advanced and machine-level (stubs or simplified implementations).
void loopInvariantCodeMotion(void);
void loopUnrolling(void);
void loopFusion(void);
void registerAllocation(void);
void functionInlining(void);
void interproceduralConstantPropagation(void);
void instructionScheduling(void);
void codeLayoutOptimization(void);
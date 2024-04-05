
#ifndef QUANTUM_TRANSFORMS_QUANTUMHX_TD
#define QUANTUM_TRANSFORMS_QUANTUMHX_TD

include "mlir/IR/OpBase.td"
include "mlir/Pass/PassBase.td"
include "mlir/Dialect/StandardOps/IR/Ops.td"
include "Dialect/Quantum/QuantumOps.td" // Include the QuantumOps file

// Header Guard
def : Pat<(RotateXOp $beta, (RotateXOp $alpha, $q)), (RotateXOp (AddFOp $alpha, $beta), $q)>;
def : Pat<(RotateYOp $beta, (RotateYOp $alpha, $q)), (RotateYOp (AddFOp $alpha, $beta), $q)>;
def : Pat<(RotateZOp $beta, (RotateZOp $alpha, $q)), (RotateZOp (AddFOp $alpha, $beta), $q)>;

#endif // QUANTUM_TRANSFORMS_QUANTUMHX_TD


#ifndef QUANTUM_TRANSFORMS_QUANTUMHX_TD
#define QUANTUM_TRANSFORMS_QUANTUMHX_TD

include "mlir/IR/OpBase.td"
include "mlir/Pass/PassBase.td"
include "mlir/Dialect/StandardOps/IR/Ops.td"
include "Dialect/Quantum/QuantumOps.td" // Include the QuantumOps file

// Header Guard
def : Pat<(PauliYGateOp $q), (PauliZGateOp $q)>;



#endif // QUANTUM_TRANSFORMS_QUANTUMHX_TD

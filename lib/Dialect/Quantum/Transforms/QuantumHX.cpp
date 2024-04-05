#include "mlir/Dialect/StandardOps/IR/Ops.h"
#include "mlir/IR/Builders.h"
#include "mlir/IR/BuiltinTypes.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/IR/PatternMatch.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Transforms/GreedyPatternRewriteDriver.h"
#include "../PassDetail.h"
#include "Dialect/Quantum/QuantumOps.h"

using namespace mlir;
using namespace mlir::quantum;

// two successive rotation gates can be combined together and their angles added
class FoldRotation : public OpRewritePattern<UniversalRotationGateOp> {
    // Constructor: benefit = "how much computation" the transformation saves
    using OpRewritePattern::OpRewritePattern;
    
    // match (rz - rz) pattern
    LogicalResult matchAndRewrite(UniversalRotationGateOp op2, PatternRewriter &rewriter) const final {
     auto op1 = op2.qinp().getDefiningOp<UniversalRotationGateOp>();
     if (!op1)
      return failure();

      /// Dummy implementation for testing only
      /// TODO: implement proper merge: U.U
       if (op1.getParentRegion() != op2.getParentRegion())
            return failure();

        rewriter.setInsertionPoint(op1);
        OperationState addState(op1.getLoc(), AddFOp::getOperationName());
        AddFOp::build(rewriter, addState, op1.phi(), op2.phi());
        Operation *addOp = rewriter.createOperation(addState);

        rewriter.startRootUpdate(op2);
        op2.setOperand(0, addOp->getResult(0)); // phi1 + phi2
        op2.setOperand(1, op1.qbs());
        rewriter.finalizeRootUpdate(op2);
        rewriter.eraseOp(op1);
       return success();
    }
};

class QuantumHXPass : public QuantumHXPassBase<QuantumHXPass> {
  void runOnFunction() override;
};

namespace {
#include "Dialect/Quantum/Transforms/QuantumHX.h.inc"
} // namespace

void QuantumHXPass::runOnFunction() {
  OwningRewritePatternList patterns(&getContext());
  populateWithGenerated(patterns);
  patterns.insert<FoldRotation>(&getContext());
  if (failed(
          applyPatternsAndFoldGreedily(getFunction(), std::move(patterns)))) {
    signalPassFailure();
  }
}

namespace mlir {

std::unique_ptr<FunctionPass> createQuantumHXPass() {
  return std::make_unique<QuantumHXPass>();
}

} // namespace mlir




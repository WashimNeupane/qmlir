add_mlir_dialect_library(QuantumTransforms
    QuantumRewrites.cpp
    Convert1QToU.cpp
    PrepareForZX.cpp
    QuantumHX.cpp

    ADDITIONAL_HEADER_DIRS
    ${PROJECT_SOURCE_DIR}/include/Dialect/Quantum/Transforms

    DEPENDS
    MLIRQuantumPassIncGen
    MLIRQuantumTransformsRewritesPassIncGen
    MLIRQuantumConvert1QToUPassIncGen
    MLIRQuantumPrepareForZXPassIncGen
    MLIRZXTransformsRewritesPassIncGen
    MLIRQuantumHXPassIncGen
    )

target_link_libraries(QuantumTransforms PUBLIC MLIRIR)

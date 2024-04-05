LLVM MLIR INSTALL (takes a while)
-----------------------------------------------
FROM ubuntu:20.04
WORKDIR /artifact

### base
RUN apt-get update -y
ARG DEBIAN_FRONTEND=noninteractive
RUN DEBIAN_FRONTEND="noninteractive" apt-get install -y tzdata
RUN apt-get install -y build-essential
RUN apt-get install -y cmake
RUN apt-get install -y git
RUN apt-get install -y ninja-build
RUN apt-get install -y python3.8 python3-pip
RUN apt-get install -y lld
RUN apt-get install -y vim
RUN apt-get install -y wget curl
RUN apt-get install -y lsb-release software-properties-common
RUN apt-get install -y zip unzip
RUN wget https://apt.llvm.org/llvm.sh && \
    chmod +x llvm.sh && ./llvm.sh 13 && rm llvm.sh

### project dependencies
# 1. LLVM
git clone https://github.com/llvm/llvm-project
cd /artifact/llvm-project && git checkout 9816d43cff5ad7abb94eab94dcd205285675972a
cd /artifact/llvm-project && \
    mkdir build && \
    cd build && \
    cmake ../llvm -G Ninja         \
      -DCMAKE_BUILD_TYPE=Release    \
      -DCMAKE_C_COMPILER=clang-13    \
      -DCMAKE_CXX_COMPILER=clang++-13 \
      -DLLVM_ENABLE_ASSERTIONS=ON      \
      -DLLVM_BUILD_EXAMPLES=OFF         \
      -DLLVM_TARGETS_TO_BUILD="host"     \
      -DLLVM_ENABLE_PROJECTS='mlir'       \
      -DLLVM_ENABLE_OCAMLDOC=OFF           \
      -DLLVM_ENABLE_BINDINGS=OFF            \
      -DLLVM_INSTALL_UTILS=ON                \
      -DLLVM_ENABLE_LLD=ON
cd /artifact/llvm-project/build && ninja check-mlir

# 2. Python packages
RUN pip3 install matplotlib
RUN pip3 install qiskit
RUN pip3 install numpy
RUN pip3 install sh

### Project
echo "bump-git-11"
git clone https://github.com/opencompl/QMLIR
cd /artifact/QMLIR && git checkout cc-artifact
cd /artifact/QMLIR && git submodule init && git submodule update
cd /artifact/QMLIR && \
    mkdir build && \
    cd build && \
    cmake .. -G Ninja                                          \
      -DCMAKE_BUILD_TYPE=Release                                \
      -DCMAKE_LINKER=lld                                         \
      -DCMAKE_C_COMPILER=clang-13                                 \
      -DCMAKE_CXX_COMPILER=clang++-13                              \
      -DMLIR_DIR=/artifact/llvm-project/build/lib/cmake/mlir/       \
      -DLLVM_DIR=/artifact/llvm-project/build/lib/cmake/llvm/        \
      -DLLVM_EXTERNAL_LIT=/artifact/llvm-project/build/bin/llvm-lit &&\
    ninja && \
    ninja check-quantum-opt
export PATH=$PATH:/artifact/llvm-project/build/bin:/artifact/QMLIR/build/bin
export OPENQASM_TO_MLIR_PY=/artifact/QMLIR/tools/openqasm-to-mlir.py

### Benchmarks
# 1. QASMBench
git clone https://github.com/anurudhp/QASMBench.git -b cc-artifact
cd /artifact/QASMBench && \
    mkdir circuit_qasm circuit_mlir && \
    cp small/*/*.qasm medium/*/*.qasm large/*/*.qasm circuit_qasm/
cp /artifact/QMLIR/data/scripts/Makefile \
       /artifact/QMLIR/data/scripts/run-opts-bench.py \
       /artifact/QMLIR/data/scripts/plot-dataset-gate-stats.py \
       /artifact/QMLIR/data/scripts/plot-bench-gate-stats.py \
       /artifact/QMLIR/data/scripts/plot-bench-runtimes.py \
       /artifact/QASMBench/

# 2. IBM Challenge
mkdir /artifact/IBMChallenge && \
    cd /artifact/IBMChallenge && \
    cp /artifact/QMLIR/data/circuit_qasm.zip . && \
    unzip circuit_qasm.zip && \
    mkdir circuit_mlir
cp /artifact/QMLIR/data/scripts/Makefile \
       /artifact/QMLIR/data/scripts/run-opts-bench.py \
       /artifact/QMLIR/data/scripts/plot-dataset-gate-stats.py \
       /artifact/QMLIR/data/scripts/plot-bench-gate-stats.py \
       /artifact/QMLIR/data/scripts/plot-bench-runtimes.py \
       /artifact/QMLIR/data/scripts/plot-bench-runtimes-ibmchallenge.py \
       /artifact/IBMChallenge/

### Execute and plot
cd /artifact/QASMBench && make plots
cd /artifact/IBMChallenge && make plots

git clone https://github.com/dime10/QIRO.git
mkdir QIRO/build && cd QIRO/build
cmake .. -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DLLVM_ENABLE_ASSERTIONS=On \
    -DLLVM_ENABLE_LLD=ON
    -DCMAKE_C_COMPILER=clang-13                                 \
    -DCMAKE_CXX_COMPILER=clang++-13                              \
    -DMLIR_DIR=/artifact/llvm-project/build/lib/cmake/mlir/       \
    -DLLVM_DIR=/artifact/llvm-project/build/lib/cmake/llvm/        \
    -DLLVM_EXTERNAL_LIT=/artifact/llvm-project/build/bin/llvm-lit &&\
cmake --build .
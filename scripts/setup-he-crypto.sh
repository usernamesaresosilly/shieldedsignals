#!/bin/bash
# included packages:
# git             – Version control system (required to clone SEAL, GSL, Crypto++)
# cmake           – Build system generator (used to build SEAL and GSL)
# build-essential – GCC, g++, make, etc.
# libssl-dev      – SSL/TLS headers (used by some crypto libs/tools)
# pkg-config      – Helps locate libraries during compilation
# valgrind        – Runtime memory checker and leak detector
# gdb             – GNU Debugger
# lldb            – LLVM Debugger
# clang-format    – Formatter for C++/C
# python3-dev     – Python headers (for pybind11 & building Python modules)
# python3-pip     – Python package manager
# pybind11-dev    – C++ ↔ Python interop library (header-only)
# libcrypto++-dev – Crypto++ development headers
# libcrypto++-doc – Docs for Crypto++
# libcrypto++-utils – Tools for testing Crypto++
# sudo            – In case your container doesn’t have sudo installed
# tree            – Displays directory structures in a tree-like format (used to verify installs)
set -e
apt update && apt full-upgrade -y
sudo apt install -y \
  git cmake build-essential libssl-dev pkg-config \
  valgrind gdb lldb clang-format \
  python3-dev python3-pip pybind11-dev \
  libcrypto++-dev libcrypto++-doc libcrypto++-utils \
  sudo tree
# ----------GSL---------
cd ~
git clone https://github.com/microsoft/GSL.git
cd GSL && mkdir build && cd build
cmake .. && sudo make install
sudo ln -s /usr/local/include/gsl /usr/include/gsl
tree -L 2 /usr/include/gsl
# ----------SEAL----------
cd ~
git clone https://github.com/microsoft/SEAL.git
cd SEAL && mkdir build && cd build
cmake .. && cmake --build . -j$(nproc)
sudo cmake --install . && sudo ldconfig
sudo ln -s /usr/local/include/SEAL-4.1/seal /usr/local/include/seal
tree -L 2 /usr/local/include/seal
ls -lh /usr/local/lib/libseal*.a
GREEN='\033[0;32m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color
echo -e "\n${GREEN}✔ Installed headers:${NC}"
echo -e "${CYAN}/usr/local/include/seal:${NC}"
ls /usr/local/include/seal/{seal.h,encryptor.h,decryptor.h,batchencoder.h} 2>/dev/null | xargs -n 1 basename
echo -e "${CYAN}/usr/include/gsl:${NC}"
ls /usr/include/gsl/{span,string_span,narrow} 2>/dev/null | xargs -n 1 basename
echo -e "\n${GREEN}✅ Capstone setup complete.${NC}"

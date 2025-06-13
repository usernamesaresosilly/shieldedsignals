# ShieldedSignals - Installation Guide

This document provides step-by-step instructions for setting up **ShieldedSignals**, including all necessary dependencies for NS3 simulations involving advanced cryptography.

---

## System Requirements

- **OS:** Ubuntu 22.04+ or Debian-based distributions
- **Privileges:** Sudo privileges
- **Resources:** At least 4 CPU cores and 8 GB RAM recommended

---

## Step 1 — Install System Packages

Open a terminal and execute:

```bash
sudo apt update && sudo apt full-upgrade -y

sudo apt install -y \
  git cmake build-essential libssl-dev pkg-config \
  valgrind gdb lldb clang-format \
  python3-dev python3-pip pybind11-dev \
  libcrypto++-dev libcrypto++-doc libcrypto++-utils \
  sudo tree
```

### Package Summary

| Package           | Purpose                                          |
|-------------------|--------------------------------------------------|
| `git`             | Version control (repository cloning)             |
| `cmake`           | Build system generator                           |
| `build-essential` | Compiler toolchain (gcc, g++, make)              |
| `libssl-dev`      | SSL/TLS development headers                      |
| `pkg-config`      | Library discovery helper                         |
| `valgrind`        | Memory checker                                   |
| `gdb`, `lldb`     | Debugging tools                                  |
| `clang-format`    | Source code formatter                            |
| `python3-dev`     | Python headers (for pybind11 integration)        |
| `python3-pip`     | Python package manager                           |
| `pybind11-dev`    | Python ↔ C++ interoperability                    |
| `libcrypto++-*`   | Crypto++ library for cryptography                |
| `sudo`            | Administrative privileges                        |
| `tree`            | Display directory structure                      |

---

## Step 2 — Install NS3 (`v2x-lte-dev` Branch)

Clone and set up NS3:

```bash
git clone https://gitlab.com/cttc-lena/ns-3-dev/-/tree/v2x-lte-dev?ref_type=heads
cd ns-3-dev
```

---

## Step 3 — Install NR Module

Clone the NR module within NS3’s `contrib` directory:

```bash
cd contrib
git clone https://gitlab.com/cttc-lena/nr/-/blob/nr-v2x-dev/README.md?ref_type=heads
```

---

## Step 4 — Install Microsoft GSL

```bash
cd ~
git clone https://github.com/microsoft/GSL.git
cd GSL
mkdir build && cd build
cmake ..
sudo make install
sudo ln -s /usr/local/include/gsl /usr/include/gsl
tree -L 2 /usr/include/gsl
```

---

## Step 5 — Install Microsoft SEAL

```bash
cd ~
git clone https://github.com/microsoft/SEAL.git
cd SEAL
mkdir build && cd build
cmake ..
cmake --build . -j$(nproc)
sudo cmake --install .
sudo ldconfig
sudo ln -s /usr/local/include/SEAL-4.1/seal /usr/local/include/seal
tree -L 2 /usr/local/include/seal
ls -lh /usr/local/lib/libseal*.a
```

---

## Verify Installation

Confirm that GSL and SEAL are correctly installed:

```bash
ls /usr/include/gsl
ls /usr/local/include/seal
```

---

## Installation Complete

Your system is now fully configured for building and running **ShieldedSignals** simulations.

If you encounter any issues, please open an issue in this repository.

---

## Notes

- SEAL and GSL versions are the latest stable at the time of writing.
- Ensure correct NS3 branch usage for V2X and LTE simulations.
- Instructions assume a bare-metal or virtual machine environment; Docker instructions may be provided separately.

---

## Next Steps

Proceed to build ShieldedSignals modules, configure NS3 scenarios, and run simulations.

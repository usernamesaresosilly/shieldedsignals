# ShieldedSignals

## Setting up NS3

### 1. Install NS3 (`ns-3-dev`, `v2x-lte-dev` branch)

Clone the NS3 repository using the following link:  
[https://gitlab.com/cttc-lena/ns-3-dev/-/tree/v2x-lte-dev?ref_type=heads](https://gitlab.com/cttc-lena/ns-3-dev/-/tree/v2x-lte-dev?ref_type=heads)

### 2. Install the NR module

Clone the NR module into the `contrib` folder inside your NS3 directory.  
Be sure to name the folder `nr`:

[https://gitlab.com/cttc-lena/nr/-/blob/nr-v2x-dev/README.md?ref_type=heads](https://gitlab.com/cttc-lena/nr/-/blob/nr-v2x-dev/README.md?ref_type=heads)

Follow the instructions in the NR module README to properly configure the NS3 distribution and integrate the NR module.

### 3. Install dependencies

There is a setup script provided to install all required dependencies for SEAL and Crypto++.  
Please run the script to ensure proper configuration of the libraries.

### 4. Setup project in `scratch`

Move the files from this repository's `project` folder into a new folder under the `scratch` directory of NS3.

Next, add the following lines to the `CMakeLists.txt` file inside the `scratch` directory:

```cmake
include_directories(/usr/include ~/)
link_directories(/usr/local/lib)
link_libraries(cryptopp seal-4.1)
```

> **Note:** If the dependencies were installed in a different location, update the `include_directories` and `link_directories` paths accordingly to point to the correct installation directories. Also install.md goes a bit farther in depth on setup

# ShieldedSignals

## Setting up NS3

### 1. Install NS3 (ns-3-dev, `v2x-lte-dev` branch)

Clone the NS3 repository using the following link:  
[https://gitlab.com/cttc-lena/ns-3-dev/-/tree/v2x-lte-dev?ref_type=heads](https://gitlab.com/cttc-lena/ns-3-dev/-/tree/v2x-lte-dev?ref_type=heads)

### 2. Install the NR module

Clone the NR module into the `contrib` folder inside your NS3 directory.  
Be sure to name the folder `nr`:

[https://gitlab.com/cttc-lena/nr/-/blob/nr-v2x-dev/README.md?ref_type=heads](https://gitlab.com/cttc-lena/nr/-/blob/nr-v2x-dev/README.md?ref_type=heads)

Follow the instructions in the nr module README fro how to set up the NS3 distribution and the nr module

### 3. Install dependencies

There is a setup script available to install dependencies for SEAL and Crypto++.  
Please use the provided script to ensure proper configuration.

### 4. Setup project in scratch

Move the files in this respositroy's project folder over to a folder in the scracth directory of NS3.

Add the following three line of code to the CmakeList file in scratch

include_directories(/usr/include ~/)
link_directories(/usr/local/lib)
link_libraries(cryptopp seal-4.1)

Use the script form #3 then the dependencies will work, if installed ina different location then add the directory where it is stored to the include directories line


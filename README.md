# Generic Computed Tomography (GCT)

GCT (Generic Computed Tomography) is a GPU-accelerated image reconstruction and processing toolkit for CT systems of various detector and projection geometries.
GCT can currently reconstruct cross-sectional images from the following axial scan (circular X-ray source trajectory) projections using FDK (Feldkamp, Davis, and Kress) algorithm:

1. 1D parallel projection.
2. 1D fan-beam projection (line detector).
3. 1D fan-beam projection (arc detector).
4. 2D parallel projection.
5. 2D rebinned or cone-parallel projections.
6. 2D cone-beam projection (cylindrical detector).
7. 2D cone-beam projection (flat detector).

GCT presents a novel software design to describe the CT geometry by utilizing the generic 
programming techniques of modern (C++ 20) and CUDA C++. Our aim is to design scalable 
and maintainable reconstruction software by following the SOLID principles (https://en.wikipedia.org/wiki/SOLID).

Here are the images reconstructed from the real projections with the GCT FDK algorithm.

<img src="doc/pictures/recon_img.png">

Micro-CT: The full-scan projections were acquired using the micro-CT scanner with flat
detector (Carl Zeiss Metrotom). Images courtesy of Fraunhofer IPK, Berlin, Germany. <br/>

C-arm CBCT: The short-scan projections were acquired using the interventional C-arm CBCT scanner 
with flat
detector (Siemens Artis Q). Images courtesy of Magdeburg university hospital, Magdeburg, Germany. <br/>


MSCT: The full-scan projections were acquired using the Multi-Slice CT scanner with cylindrical
detector.<br/>

Synchrotron-CT: The half-scan parallel projections were acquired using the Synchrotron-CT scanner.
Images courtesy of Australian Nuclear Science and Technology Organisation. <br/>





## Documentation
The paper describing the design of GCT has been published in the conference proceedings
of the 17th International Meeting on Fully Three-Dimensional
Image Reconstruction in Radiology and Nuclear Medicine.
The title of the paper is 'A generic software design for computed tomography in modern C++' and it 
can be accessed from the link: https://arxiv.org/abs/2310.16846


## Installation and software build
The current version of GCT requires a C++ compiler that supports C++ 23 features and an NVIDIA CUDA compiler and toolkit that supports CUDA 13.3. Please check the official CUDA installation instructions for your operating system. Note that only NVIDIA GPUs of Turing, Ampere, Ada Lovelace, Hopper, and Blackwell architectures support CUDA 13.3. Unfortunately, GCT cannot be run on GPUs of Tesla, Fermi, Kepler, Maxwell, Pascal and Volta architectures.

Currently, GCT has been installed and tested only in Ubuntu 26.04 LTS. We will add installation instructions for the Windows operating system after testing.

### Ubuntu 26.04 LTS

Here are the installation instructions we followed for a laptop with an RTX 3070 GPU (Ampere architecture) running [Ubuntu 26.04 LTS](https://ubuntu.com/download/desktop). The following instructions assume that you have access to a computer with a freshly installed Ubuntu 26.04 LTS. Before executing the commands in the terminal, read all installation instructions to determine which packages you need to install (some may already be installed) and the best option for installation and program compilation where alternatives are available.

#### 1. Installing the g++ Compiler and Other Build Tools

Verify whether the g++ compiler is already installed:

```console
g++ --version
```

In Ubuntu 26.04 LTS, the following commands will install g++ 15.2 and other packages for building C/C++ programs, such as `make`:

```console
sudo apt update
sudo apt install build-essential
```

#### 2. Installing CMake (4.2.3)

```console
sudo apt-get install cmake
```

#### 3. Installing the NVIDIA Driver

The minimum driver version required for CUDA 13.x is R580. You can install the driver with the following commands:

```console
ubuntu-drivers devices
# Make sure that the recommended driver version is greater than 580.0
sudo ubuntu-drivers install
```

Please reboot after installing the driver. For more information, see the [NVIDIA driver installation guide](https://docs.nvidia.com/datacenter/tesla/driver-installation-guide/).

#### 4. Installing gedit for Text Editing

```console
sudo apt update
sudo apt install gedit
```

#### 5. Installing CUDA Toolkit 13.3

Follow the installation instructions on the
[CUDA Downloads page](https://developer.nvidia.com/cuda-downloads?target_os=Linux&target_arch=x86_64&Distribution=Ubuntu&target_version=26.04&target_type=deb_network).

After installation, verify with the following command:

```console
nvcc --version
```

If the `nvcc` version is not displayed, the following environment variables need to be set by editing the `.bashrc` file located in the `$HOME` directory:

```
export CUDA_HOME=/usr/local/cuda-13.3
export PATH=$CUDA_HOME/bin:$PATH
export LD_LIBRARY_PATH=$CUDA_HOME/lib64:$LD_LIBRARY_PATH
```

```console
cd $HOME
gedit .bashrc
```

Add the above three lines to set the environment variables and close the file. To apply the changes in the current session:

```console
source ~/.bashrc
```

Then verify the `nvcc` version again:

```console
nvcc --version
```

You should see output similar to the following:

```console
nvcc: NVIDIA (R) Cuda compiler driver
Copyright (c) 2005-2026 NVIDIA Corporation
Built on Fri_Apr_24_07:22:02_PM_PDT_2026
Cuda compilation tools, release 13.3, V13.3.33
Build cuda_13.3.r13.3/compiler.37862127_0
```

After the above successful configuration, make sure the following commands print the full path to the `nvcc` executable:

```console
which nvcc
echo $CUDACXX
```

If not, set the `CUDACXX` environment variable by adding the following line to `/etc/environment`:

```
CUDACXX=/usr/local/cuda-13.3/bin/nvcc
```

```console
sudo gedit /etc/environment
```

Add the above line, then save and close the file.

#### 6. Installing Git

```console
sudo apt-get install git-all
```

#### 7 Building GCT Program
##### 7.a Downloading Source Files and Building the Program Individually

```console
git clone --recursive https://github.com/satidev/gct.git
cd gct
mkdir build
cd build
cmake ..
make
```

##### 7.b Using an IDE
Alternatively (and ideally), a popular IDE such as [Visual Studio Code](https://code.visualstudio.com/) can be used to edit source files and build the program.

###### Installing VS Code and Required/Optional Extensions

1. Install [Visual Studio Code](https://code.visualstudio.com/) via Ubuntu Software or directly from the website.
2. Launch VS Code.
3. Install the **C/C++** extension from Microsoft (click **Extensions** and search for `C/C++`).
4. Install the **CMake Tools** extension from Microsoft.
5. Optional: install the **Nsight Visual Studio Code Edition** extension from NVIDIA.

###### Compilation and Program Build in Visual Studio Code

1. Clone the git repository recursively to a local directory:

```console
mkdir gct_vs
cd gct_vs
git clone --recursive https://github.com/satidev/cuda-cpp-tutorial.git
```

2. Launch VS Code.
3. Click **File → Open Folder** and select the directory containing the main `CMakeLists.txt` (`gct_vs`).
4. Select the host compiler (e.g., GCC 15.2).
5. Confirm that the configuration is successful and that build files have been written to the `build` directory.
6. Build the library and program: click **Terminal → Run Build Task → CMake: build**.



### Test data and example reconstructions

To run the example reconstructions listed example_recon.h file, 
please download the test data from the following link:
https://drive.google.com/file/d/10HiyYO8T32fDTYqEoh9RPe_I1__YbzYH/view?usp=drive_link

The projection data was generated using the CT simulation toolkit
[CTL](https://gitlab.com/tpfeiffe/ctl).

After the successful compilation of GCT programs, the example reconstructions
can be performed as given below:

```console
# Change to directory where the gct executable lies.
cd build
# Unzip the downloaded file contaning test data.
# Pass the directory name as a command line arugment to gct executable.
./gct /path/gct_test_data
```

















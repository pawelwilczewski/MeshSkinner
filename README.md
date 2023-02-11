# MeshSkinner

3D mesh skinning software. COMP3931 Individual Project by Pawel Wilczewski, University of Leeds.

# Getting started

## Windows

Clone this repository, run `GenerateProjectsWindows.bat` and build the project using `Visual Studio 2022`.

## Linux

Clone this repository, run `GenerateProjectsLinux.sh` and build the project using `make`.

Quickstart commands on Linux:

`chmod +x GenerateProjectsLinux.sh && ./GenerateProjectsLinux.sh && make && ./bin/Debug-linux-x86_64/MeshSkinner/MeshSkinner`

On FENG Linux, you will need to load `GCC` first:

`module load gcc && chmod +x GenerateProjectsLinux.sh && ./GenerateProjectsLinux.sh && make && ./bin/Debug-linux-x86_64/MeshSkinner/MeshSkinner`

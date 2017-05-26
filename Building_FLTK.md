# Building FLTK
Since only source code of FLTK is provided from its official site, we need to build the source to create necesarry library files (*.lib or *.a or *.so) for using.  
### Requirements
- CMake: Download cmake from its official site. (https://cmake.org)
- MinGW: Download MinGW from its official site (http://www.mingw.org). If you downloaded Code::Blocks setup which already has mingw then you don't need this. Also not needed for non-windows platform.  
While this guide assumes the user is on windows (for simplicity), linux users can follow the exact same steps with little modifications (which would be obvious). If you face some problem while building the library, then feel free to open a new issue on this repo and we will try to help you resolve the problem.  
  
### Steps to build
- Download and extract the source code of fltk. Lets say in 'D:/fltk-1.3.4-1'.
- Create a include folder in fltk folder that is 'D:/fltk-1.3.4-1/include' and copy the FL and GL folders from fltk directory to the include directory. This structure is required for cmake.
- Open cmake-gui.exe
- In the `source code` field select the fltk folder.
- In the `where to build` field write another folder say 'D:/fltk-1.3.4-1/build'. If the folder doesn't exist, cmake will create it.
- Press `Configure`. It will ask to create the build folder if it doesn't exist, select yes.
- Select `CodeBlocks - MinGW Makefiles` from the generator combobox. Also Select `Specify Native Compilers` below.
- On next screen, browse for `gcc.exe` and `g++.exe` for C and C++ respectively. These will either be in `C:/MinGW/bin` for external MinGW or in `C:/Program Files (x86)/CodeBlocks/MinGW/bin` for mingw with codeblocks.  
- Click Finish. Cmake will take some time to check the compilers and generate some listing. Everything will be red.
- Click on `Configure` again. The red color will go away. Everything is verified.
- Click on `Generate` to generate the proper project and makefiles.
- Close CMake and open a command prompt window in the build directory of fltk, that is, 'D:/fltk-1.3.4-1/build'.
- Run `mingw32-make` in fltk build directory which comes with MinGW. To do this, once you have cd..ed into fltk's build directory, issue either `C:/MinGW/bin/mingw32-make` or `C:/Program Files (x86)/CodeBlocks/MinGW/bin/mingw32-make` depending on where your MinGW is.
- Every project will build succesfully. The `FLUID` project will fail and you will get an error in the end. But we are not going to use FLUID and the files required for us are there.
- Copy the `abi-version.h` files from 'D:/fltk-1.3.4-1/build/FL/abi-version.h' to 'D:/fltk-1.3.4-1/include/FL/abi-version.h'
- Copy all the *.a files from 'D:/fltk-1.3.4-1/build/lib' to 'D:/fltk-1.3.4-1/lib'
  
Congratulation! You have successfully built the FLTK library from source! :)  

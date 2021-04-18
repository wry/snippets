# c++ conan
a tiny example using conan

use :

* mkdir ./build ; cd build
* conan install ..
* cmake ..
* make

use (ios x86\_64) :

* mkdir ./build ; cd build
* conan install .. --build=missing --profile:host=../profiles/ios --profile:build=default 
* cmake .. -GXcode -DCMAKE\_TOOLCHAIN\_FILE=conan\_toolchain.cmake
* open get.xcodeproj

use (android x86\_64) :

* mkdir ./build ; cd build
* conan install .. --build=missing --profile:host=../profiles/android --profile:build=default 
* cmake .. -DCMAKE\_TOOLCHAIN\_FILE=conan\_toolchain.cmake
    * or cmake .. -DCMAKE\_TOOLCHAIN\_FILE=../cmake/conan\_android\_toolchain.cmake
* make


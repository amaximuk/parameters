mkdir build_conan
cd build_conan
conan install .. --build=missing --settings=build_type=Release
cmake -DPACKAGE_MANAGER=CONAN -DBUILD_PARAMETERS_COMPILER=ON -G "Visual Studio 17 2022" -A x64 ..
cd ..
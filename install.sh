#!/bin/bash

set -e  # Exit on error


# 2. Build R3D
echo "Building r3d..."
mkdir -p external/r3d/build
cd external/r3d/build
cmake .. -DR3D_RAYLIB_VENDORED=ON -DASSIMP_VENDORED=ON -DCMAKE_BUILD_TYPE=Release
cmake --build .

cd ../../../
echo "r3d built successfully!"

# 3. Done
echo "You can now link your project with:"
echo "    -Iexternal/r3d/include"
echo "    external/r3d/build/libr3d.a"

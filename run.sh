export LD_LIBRARY_PATH="$HOME"/.local/lib/:"$GCC_DIR"/lib64/:"$LD_LIBRARY_PATH"

cmake -S . -B ./build -DCMAKE_CXX_COMPILER="$GCC_DIR"/bin/g++
make -C ./build -j"$(nproc)" --silent
./build/stl_algorithms_and_ranges

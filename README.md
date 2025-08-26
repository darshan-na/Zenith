# Zenith

A C++ static site generator for a personal site.

## Prerequisites
- C++20 compatible compiler
- CMake 3.10+
- cmark-gfm (Markdown parser)
- tinyxml2 (RSS generation)

## Setup
1. Clone the repository.
2. Install dependencies:
   - cmark-gfm: `sudo apt-get install libcmark-gfm-dev` (Ubuntu) or equivalent.
   - tinyxml2: `sudo apt-get install libtinyxml2-dev` (Ubuntu) or equivalent.
   - yamp-cpp  `sudo apt install libyaml-cpp-dev`    (Ubuntu) or equivalent.
3. Set the file paths in the `config.yaml` file appropriately.
3. Build the project:
   ```bash
   mkdir build && cd build
   cmake ..
   make
4. cd into the project directory:
```bash
   cd ..
```
5. Execute the binary
```bash
   ./build/zenith
```
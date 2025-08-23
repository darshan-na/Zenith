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
3. Create a `lib/` directory and add cmark-gfm and tinyxml2 (or use git submodules).
4. Build the project:
   ```bash
   mkdir build && cd build
   cmake ..
   make
5. ./zenith
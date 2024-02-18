# mm.minidbg

Toy project - creating a mini debugger, based entirely on repo:
https://github.com/TartanLlama/minidbg/tree/master


## How to run cmake

In root folder:
  - cd build; cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 .. # this will generate the build files (Makefiles for linux) and compile_commands.json for clang
  - cd build; cmake --build . # this will build the actual program. For this project files go into build/apps/minidbg



## How to fetch submodules (you will need this to build)
git submodule update --init --recursive

## How to "revert" files not added into git
git clean -d -f -x
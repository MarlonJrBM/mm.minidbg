add_library(minidbglib minidbglib.cpp ../ext/linenoise/linenoise.c ../ext/linenoise/linenoise.h ../include/minidbg/minidbglib.hh)

target_include_directories(minidbglib PUBLIC ../include)

target_compile_features(minidbglib PUBLIC cxx_std_17)

target_link_libraries(minidbglib PUBLIC 
                     ${PROJECT_SOURCE_DIR}/ext/libelfin/dwarf/libdwarf++.so
                     ${PROJECT_SOURCE_DIR}/ext/libelfin/elf/libelf++.so)


add_dependencies(minidbglib libelfin)
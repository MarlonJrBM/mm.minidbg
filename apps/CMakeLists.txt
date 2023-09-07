add_executable(minidbg minidbg.cpp ../ext/linenoise/linenoise.c)
target_link_libraries(minidbg PUBLIC minidbglib)

target_compile_features(minidbg PRIVATE cxx_std_17)
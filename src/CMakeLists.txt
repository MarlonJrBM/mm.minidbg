add_library(minidbglib STATIC minidbglib.cpp ../include/minidbg/minidbglib.hh)
target_include_directories(minidbglib PUBLIC ../include)

target_compile_features(minidbglib PUBLIC cxx_std_11)

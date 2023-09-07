add_executable(minidbg minidbg.cpp)

target_link_libraries(minidbg PUBLIC minidbglib)

target_compile_features(minidbg PRIVATE cxx_std_17)
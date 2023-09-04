#include "minidbg/minidbglib.hh"

#include <sys/ptrace.h>
#include <unistd.h>

#include <iostream>

int minidbg::Application::run(int argc, const char* argv[]) {
    if (argc < 2) {
        std::cerr << "Program name not specified.\n";
        return -1;
    }

    auto prog = argv[1];

    auto pid = fork();
    if (pid == 0) {
        // we're in the child process
        // execute debugee
        ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);
        execl(prog, prog, nullptr);

    } else if (pid >= 1) {
        // we're in the parent process
        // execute debugger
        std::cout << "Started debugging process " << pid << '\n';
        minidbg::Debugger debugger(prog, pid);
    }

    return 0;
}
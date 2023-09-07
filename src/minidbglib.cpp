#include "minidbg/minidbglib.hh"

#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <vector>

#include "linenoise.h"

namespace {
std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> out{};
    std::stringstream ss{s};
    std::string item;

    while (std::getline(ss, item, delimiter)) {
        out.push_back(item);
    }

    return out;
}

bool isPrefix(const std::string& s, const std::string& of) {
    if (s.size() > of.size()) return false;
    return std::equal(s.begin(), s.end(), of.begin());
}
}  // namespace

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
        debugger.run();
    }

    return 0;
}

void minidbg::Debugger::run() {
    int wait_status;
    auto options = 0;
    waitpid(_pid, &wait_status, options);

    char* line = nullptr;
    while ((line = linenoise("minidbg> ")) != nullptr) {
        handleCommand(line);
        linenoiseHistoryAdd(line);
        linenoiseFree(line);
    }
}

void minidbg::Debugger::handleCommand(const std::string& line_) {
    auto args = split(line_, ' ');
    auto command = args[0];

    if (isPrefix(command, "continue")) {
        continueExecution();
    } else {
        std::cerr << "Unknown command\n";
    }
}

void minidbg::Debugger::continueExecution() {
    ptrace(PTRACE_CONT, _pid, nullptr, nullptr);

    int wait_status;
    auto options = 0;
    waitpid(_pid, &wait_status, options);
}
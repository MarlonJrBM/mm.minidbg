#include "minidbg/minidbglib.hh"

#include <sys/personality.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <vector>

#include "linenoise.h"

namespace {
std::vector<std::string> split(const std::string &s, char delimiter) {
  std::vector<std::string> out{};
  std::stringstream ss{s};
  std::string item;

  while (std::getline(ss, item, delimiter)) {
    out.push_back(item);
  }

  return out;
}

bool isPrefix(const std::string &s, const std::string &of) {
  if (s.size() > of.size()) return false;
  return std::equal(s.begin(), s.end(), of.begin());
}
}  // namespace

namespace minidbg {
int minidbg::Application::run(int argc, const char *argv[]) {
  if (argc < 2) {
    std::cerr << "Program name not specified.\n";
    return -1;
  }

  auto prog = argv[1];

  auto pid = fork();
  if (pid == 0) {
    // we're in the child process
    // execute debugee
    personality(ADDR_NO_RANDOMIZE);
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

  char *line = nullptr;
  while ((line = linenoise("minidbg> ")) != nullptr) {
    handleCommand(line);
    linenoiseHistoryAdd(line);
    linenoiseFree(line);
  }
}

void minidbg::Debugger::handleCommand(const std::string &line_) {
  auto args = split(line_, ' ');
  auto command = args[0];

  if (isPrefix(command, "continue")) {
    continueExecution();
  } else if (isPrefix(command, "breakpoint")) {
    std::string addr{args[1],
                     2};  // naively assume that the user has written 0xADDRESS
    setBreakpointAtAddress(std::stol(addr, 0, 16));
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

void minidbg::Breakpoint::enable() {
  auto data = ptrace(PTRACE_PEEKDATA, _pid, _addr, nullptr);
  _savedData = static_cast<uint8_t>(data & 0xff);  // save bottom byte
  const uint64_t INT3 = 0xcc;
  uint64_t dataWithInt3 = ((data & ~0xff) | INT3);  // set bottom byte to 0xcc
  ptrace(PTRACE_POKEDATA, _pid, _addr, dataWithInt3);

  _enabled = true;
}

void minidbg::Breakpoint::disable() {
  auto data = ptrace(PTRACE_PEEKDATA, _pid, _addr, nullptr);
  const auto restoredData = ((data & ~0xff) | _savedData);
  ptrace(PTRACE_POKEDATA, _pid, _addr, restoredData);
}

void Debugger::setBreakpointAtAddress(std::intptr_t addr_) {
  std::cout << "Set breakpoint at address 0x" << std::hex << addr_ << std::endl;
  Breakpoint bp(_pid, addr_);
  bp.enable();
  _breakpoints[addr_] = bp;
}

}  // namespace minidbg

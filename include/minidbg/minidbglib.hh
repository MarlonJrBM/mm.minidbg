#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

namespace minidbg {

class Application {
 public:
  int run(int argc, const char* argv[]);
};

class Breakpoint {
 public:

  Breakpoint(): _pid(0), _addr(0), _enabled(false), _savedData{} {}

  Breakpoint(pid_t pid, std::intptr_t addr)
      : _pid{pid}, _addr{addr}, _enabled{false}, _savedData{} {}

  void enable();
  void disable();

  auto isEnabled() const -> bool { return _enabled; }
  auto getAddress() const -> std::intptr_t { return _addr; }

 private:
  pid_t _pid;
  std::intptr_t _addr;
  bool _enabled;
  uint8_t _savedData;  // data which used to be at the breakpoint address
};

class Debugger {
 public:
  Debugger(const std::string& name_, pid_t pid_)
      : _programName(name_), _pid(pid_){};
  void run();

  void handleCommand(const std::string& line_);

  void continueExecution();

  void setBreakpointAtAddress(std::intptr_t addr_);

 private:
  std::string _programName;
  pid_t _pid;
  std::unordered_map<std::intptr_t, Breakpoint> _breakpoints;
};

}  // namespace minidbg
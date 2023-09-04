#pragma once

#include <string>

namespace minidbg {

class Application {
   public:
    int run(int argc, const char* argv[]);
};

class Debugger {
   public:
    Debugger(const std::string& name_, pid_t pid_) : _programName(name_), _pid(pid_){};
    void run();

   private:
    std::string _programName;
    pid_t _pid;
};

}  // namespace minidbg
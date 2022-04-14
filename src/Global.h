#pragma once
#include "DependencyManager.h"

class Global {
  public: 
    int cycle = 0; // tracks the cycle the program is on
    bool traceEnded = false; // indicates if the trace has ended
    size_t W = 1; // indicates the size of the pipleline
    bool hault = false; // indicates if reading has haulted
    size_t totalInstCount = 0; // indicates how many instructions are currently in the system
    int total_inst = 0; //Overall instructions executed 
    DependencyManager * dm; // Pointer to dependency manager
    
    bool DEBUG = false; // used to toggle debug logs

    Global(size_t W, DependencyManager * dm) {
      this->W = W;
      this->dm = dm;
    }

    ~Global() {
      delete dm;
    }

    std::string file_name = ""; // Holds the file name
    int START_INSTRUCTION = 0; // The instruction the program should start at 
    int INSTRUCTION_COUNT = 0; // The number of instructions the program should read

};

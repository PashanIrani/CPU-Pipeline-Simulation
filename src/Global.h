#pragma once
#include "DependencyManager.h"

class Global {
  public: 
    int cycle = 0; // tracks the cycle the program is on
    bool traceEnded = false; // indicates if the trace has ended
    size_t W = 2; // indicates the size of the pipleline
    bool hault = false; // indicates if reading has haulted
    size_t totalInstCount = 0; // indicates how many instructions are currently in the system
    DependencyManager * dm; // Pointer to dependency manager
    
    bool DEBUG = false; // used to toggle debug logs

    Global(size_t W, DependencyManager * dm) {
      this->W = W;
      this->dm = dm;
    }
};

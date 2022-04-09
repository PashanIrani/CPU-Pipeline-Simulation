#pragma once

#include "Instruction.h"
#include "TraceReader.h"

class ExecuteStep {
  public:
  // TODO: add queue
  Instruction * current;
  Global * global;
  TraceReader * tr;
  
  ExecuteStep(Global * global, TraceReader * tr) {
      this->global = global;
      this->tr = tr;
    }

    /*
    * Performs actions for this step
    * Returns: Instruction that is ready to move to next stage
    */
    Instruction * performStep() {
       if (current != NULL) { // Simply Print for now TODO: add proper logic
        std::cout << "Performing EX..." << std::endl;
        current->print();
      }

      Instruction * leavingInst = current;
      current = NULL; // mark current as null, to indicate this processor is now idle
      return leavingInst;
    }

    /*
    * Intakes next instruction that it will be processing
    */
    void recieve(Instruction * incomingInst) {
      current = incomingInst;
    }
};
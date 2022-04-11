#pragma once

#include "Instruction.h"
#include "TraceReader.h"
#include "Global.h"

class WriteBackStep {
  public:
    Instruction * current = NULL;
    Global * global;

     // Unused members added to make cpp compiler happy
    TraceReader * tr;
    INSTRUCTION_TYPE currentInstType;

    WriteBackStep(Global * global, TraceReader * tr) {
      this->global = global;
      this->tr = tr;
    }

    /*
    * Performs actions for this step
    * Returns: Instruction that is ready to move to next stage
    */
    Instruction * performStep() {      
      if (current != NULL) { // Simply Print for now TODO: add proper logic
        if (global->DEBUG) {
          std::cout << "Performing WB..." << std::endl;
          current->print();
        }
      }

      Instruction * leavingInst = current;
      current = NULL; // mark current as null, to indicate this processor is now idle

      // Instruction will be leaving the system here:
      if (leavingInst != NULL) {
        global->totalInstCount--;
        delete leavingInst;
      }

      return NULL;
    }

    /*
    * Intakes next instruction that it will be processing, returns true if accepted, false if not accepted.
    */
    bool recieve(Instruction * incomingInst) {
      current = incomingInst;
      return true;
    }
};
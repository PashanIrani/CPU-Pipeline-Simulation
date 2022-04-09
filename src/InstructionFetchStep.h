#pragma once

#include "Instruction.h"
#include "TraceReader.h"
// #include "Step.h"

class InstructionFetchStep {
  TraceReader * tr;
  Instruction * current;
  Global * global;

  public:
    // constructor
    InstructionFetchStep(Global * global, TraceReader * tr) {
      this->global = global;
      this->tr = tr;
    }

    /*
    * Performs actions for this step
    * Returns: Instruction that is ready to move to next stage
    */
    Instruction * performStep() {
      
      current = tr->getNextInst(); // grabs next instruction

      // Will be null if trace has ended
      if (current == NULL) {
        std::cout << "!!! TRACE ENDED !!!" << std::endl;
        global->traceEnded = true;
      } else { // Simply Print for now TODO: add proper logic
        std::cout << "Performing IF..." << std::endl;
        current->print(); 
      }

      // Return instruction to indicate it is ready to move to next stage
      return current;
    }

};
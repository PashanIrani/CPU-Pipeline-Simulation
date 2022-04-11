#pragma once

#include "Instruction.h"
#include "TraceReader.h"
// #include "Step.h"

class InstructionFetchStep {
  TraceReader * tr;
  Instruction * current;
  Global * global;

  // Unused members added to make cpp compiler happy
  INSTRUCTION_TYPE currentInstType;

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
      if(!global->hault){
        current = tr->getNextInst(); // grabs next instruction

        // Will be null if trace has ended
        if (current == NULL) {
          if (global->DEBUG) std::cout << "!!! TRACE ENDED !!!" << std::endl;
          global->traceEnded = true;
          return NULL;
        } else {
          global->totalInstCount++;

          if (global->DEBUG) {
            std::cout << "Performing IF..." << std::endl;
            current->print(); 
          }
        }

        if(current->type == INST_BRANCH) //branch instruction halts future instructions
        global->hault = true;

        // Add newly fetched instruction to the dependency manager
        global->dm->add(current);
        
        // Return instruction to indicate it is ready to move to next stage
        return current;
      }
      return NULL;
    }

};
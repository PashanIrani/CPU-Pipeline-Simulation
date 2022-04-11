#pragma once

#include "Instruction.h"
#include "TraceReader.h"
#include "Queue.h"

class ExecuteStep {
  public:
  Instruction * current;
  Global * global;
  INSTRUCTION_TYPE currentInstType;
  struct Queue * queue; // holds instructions that need to be processed next

  // Unsed members added to make cpp compiler happy
  TraceReader * tr;


  ExecuteStep(Global * global, TraceReader * tr) {
      this->global = global;
      this->tr = tr;
      this->queue = (Queue *) malloc(sizeof(Queue));
      queue->head = NULL;
      this->currentInstType = INST_UNDEFINED;
      this->current = NULL;
    }

    /*
    * Performs actions for this step
    * Returns: Instruction that is ready to move to next stage
    */
    Instruction * performStep() {
      if (current == NULL) return NULL;

      // If branch instruction, hault reading in more instruction in IF step
      if(current->type == INST_BRANCH) global->hault = false;

      if (global->DEBUG) {
        std::cout << "Performing EX..." << std::endl;
        current->print();
      }
      // Mark dependency if of Type INT or FLOAT
      if (current->type == INST_INT || current->type == INST_FLOAT) {
        global->dm->markComplete(current);
      }
      
      Instruction * leavingInst = current;
      current = Delete(queue); // start next node (will be set to null if queue is empty)

      return leavingInst;
    }

    /*
    * Intakes next instruction that it will be processing, returns true if accepted, false if not accepted.
    */
    bool recieve(Instruction * incomingInst) {
      // check dependency
      if (!global->dm->dependenciesMet(incomingInst)) {
        return false;
      }

      if (current == NULL) {
        current = incomingInst;
      } else {
        return false;
      }

      currentInstType = current == NULL ? INST_UNDEFINED : current->type;
      return true;
    }
};
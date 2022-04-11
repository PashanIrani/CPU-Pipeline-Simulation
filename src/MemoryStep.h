#pragma once

#include "Instruction.h"
#include "TraceReader.h"
#include "Global.h"
#include "Queue.h"

class MemoryStep {
  public:
  Instruction * current;
  Global * global;
  INSTRUCTION_TYPE currentInstType;
  struct Queue * queue; // holds instructions that need to be processed next

  // Unsed members added to make cpp compiler happy
  TraceReader * tr;


  MemoryStep(Global * global, TraceReader * tr) {
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

      std::cout << "Performing MEM..." << std::endl;
      current->print();
      

      Instruction * leavingInst = current;
      current = Delete(queue); // start next node (will be set to null if queue is empty)

      return leavingInst;
    }

    /*
    * Intakes next instruction that it will be processing, returns true if accepted, false if not accepted.
    */
    bool recieve(Instruction * incomingInst) {
      if (current == NULL) {
        current = incomingInst;
      } else {
        return false;
      }

      currentInstType = current == NULL ? INST_UNDEFINED : current->type;
      return true;
    }
};
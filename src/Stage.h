#pragma once

#include "Instruction.h"
#include "TraceReader.h"
#include "InstructionFetchStep.h"
#include "InstructionDecodeStep.h"
#include "ExecuteStep.h"

template <class T>
class Stage {
  
  // Array of Processors that will perform their respective steps on an instruction
  T ** processors;
  
  // Just a label to use for debugging.
  std::string LABEL = "";

  // Array of instructions that need to be sent to the next stage. 
  Instruction ** outgoingInsts;

  Global * global; // global object used to access variables used throughout the application

  bool lastStage; // marks this stage as being the last stage, if true it will perform the final action of "handling instructions"
  
  struct Queue * pendingInsts; // holds the instruction that still need to be sent

  public:
    Stage(Global * global, TraceReader * tr, std::string LABEL, bool lastStage) {
      this->global = global;
      this->LABEL = LABEL;
      this->lastStage = lastStage;

      processors = (T **) malloc(sizeof(T *) * global->W);

      this->pendingInsts = (Queue *) malloc(sizeof(Queue));

      // Initialize all processors
      for (size_t i = 0; i < global->W; ++i) {
          processors[i] = new T(global, tr); 
      }

      outgoingInsts = (Instruction **) malloc(sizeof(Instruction *) * global->W);
    }

    /*
    * Triggers each processor to perform their duties
    */
    void run() {
      for (size_t i = 0; i < global->W; ++i) {
        std::cout << "["<< LABEL << "]" << std::endl;        
        outgoingInsts[i] = processors[i]->performStep();
        if (lastStage && outgoingInsts[i] != NULL) {
          global->totalInstCount--;
        }
      }
    }

    /*
    * Triggers next stage to recieve the outgoing instructions from this stage
    */
    template <class U>
    void send(Stage<U> * nextPipeline) {
      
      // Add all the outgoing insts from this stage at the end of the "pendingInsts" to ensure they get sent after the pending instructions
      for (size_t i = 0; i < global->W; ++i) {
        Insert(pendingInsts, outgoingInsts[i]);
      }

      struct Queue * temp = (Queue *) malloc(sizeof(Queue)); // temp queue to help "trevase the queue" without mutating the queue during traversal
      bool instWasSent = true;

      while(CountNodes(pendingInsts) > 0) {
        Instruction * inst = Delete(pendingInsts);
        
        // once instWasSet becomes false, stop the sending of all other instructions
        if (instWasSent)
        instWasSent = nextPipeline->recieve(inst);

        // if instruction was not sent, add to temp queue, so this instruction can be re-sent next cycle
        if (!instWasSent) {
          if (inst != NULL)
          std::cout << inst->id << " is pending" << std::endl;
          Insert(temp, inst);
        }
      }

      pendingInsts = temp;
    }

    /*
    * Recieves an instruction that is being added to this stage, and apporiatly adds them to the correct processor.
    */
    bool recieve(Instruction * inst) {
      if (std::is_same<T, InstructionFetchStep>::value) {
        // Do nothing, as nothing will send instructions to the "Instruction Fetch" Stage
      } else if (std::is_same<T, InstructionDecodeStep>::value) {

        // Simply sends instruction to first available processor
        for (size_t i = 0; i < global->W; ++i) {
          if (processors[i]->current == NULL) {
            return processors[i]->recieve(inst);
          }    
        }
      } else if (std::is_same<T, ExecuteStep>::value) {
        for (size_t i = 0; i < global->W; ++i) {

          // if processor is idle OR it is performing an instruction same action as this instruction being added (ie. using the same unit, therefore it needs to 
          // wait for the current instruction to finish) and not perform a MEM operation, then add it to that processor. The processor itself
          // will handle it's own queue and track the instructions it needs to process. Over here we simply determine which processor should 
          // deal with this instruction
          if (processors[i]->current == NULL 
          || (inst != NULL && inst->type != INST_LOAD && inst->type != INST_STORE && processors[i]->currentInstType == inst->type)) {
            return processors[i]->recieve(inst);
          }
        }
      }

      if (inst != NULL)
      std::cerr << "[" << inst->id << "] Instruction failed to send" << std::endl;
   

      return false;
    }
};

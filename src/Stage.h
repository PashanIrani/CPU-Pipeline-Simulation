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

  public:
    Stage(Global * global, TraceReader * tr, std::string LABEL) {
      this->global = global;
      this->LABEL = LABEL;
      
      processors = (T **) malloc(sizeof(T *) * global->W);

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
        std::cout << "["<< LABEL << "] Running Processor: " << i << std::endl;        
        outgoingInsts[i] = processors[i]->performStep();
      }
    }

    /*
    * Triggers next stage to recieve the outgoing instructions from this stage
    */
    template <class U>
    void send(Stage<U> * nextPipeline) {
      for (int i = 0; i < global->W; ++i) {
        nextPipeline->recieve(outgoingInsts[i]);
      }
    }

    /*
    * Recieves an instruction that is being added to this stage, and apporiatly adds them to the correct processor.
    */
    void recieve(Instruction * inst) {
      bool instWasSent = false; // indicates if this instruction was sent to it's next processor

      if (std::is_same<T, InstructionFetchStep>::value) {
        // Do nothing, as nothing will send instructions to the "Instruction Fetch" Stage
      } else if (std::is_same<T, InstructionDecodeStep>::value) {

        // Simply sends instruction to first available processor
        for (size_t i = 0; i < global->W; ++i) {
          if (processors[i]->current == NULL) {
            processors[i]->recieve(inst);
            instWasSent = true;
            break;
          }    
        }
      } else if (std::is_same<T, ExecuteStep>::value) {
        // TODO: add proper logic here
        for (size_t i = 0; i < global->W; ++i) {
          if (processors[i]->current == NULL) {
            processors[i]->recieve(inst);
            instWasSent = true;
            break;
          }
        }
      }

      if (!instWasSent) {
        std::cerr << "[" << LABEL << "] Instruction failed to send" << std::endl;
      }
    }
};
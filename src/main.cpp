#include <iostream>
#include "Global.h"
#include "TraceReader.h"
#include "InstructionFetchStep.h"
#include "Stage.h"
#include "ExecuteStep.h"
#include "MemoryStep.h"
#include "WriteBackStep.h"

int main(int argc, char const *argv[]) {
    
    Global * global = new Global();
    if (argc >= 5) {
      global->file_name = argv[1];
      global->START_INSTRUCTION = atoi(argv[2]);
      global->INSTRUCTION_COUNT = atoi(argv[3]);
      global->W = atoi(argv[4]);    // W-wide superscalar processor

      if (global->file_name == "") {
        std::cout << "File name cannot be empty" << std::endl;
        return -1;
      }
      
      TraceReader tr(global->file_name);
      
      if (global->START_INSTRUCTION < 0) {
        std::cout << "Start instruction cannot be negative" << std::endl;
        return -1;
      }

      // incrementing instruction from the file
      for (int i = 0; i < global->START_INSTRUCTION - 1; i++) {
        if (tr.getNextInst() == nullptr) {
          std::cout << "Start Instruction should be less than total instructions" << std::endl;
          return -1;
        }
      }

      if (global->INSTRUCTION_COUNT < 0) {
        std::cout << "Instruction Count cannot be negative" << std::endl;
        return -1;
      }

      if (global->W <= 0 ) {
        std::cout << "Cannot have less than 1-wide superscalar processor" << std::endl;
        return -1;
      }

      // Create Pipeline Handlers for each steps
      Stage<InstructionFetchStep> ifp(global, &tr, "IF", false);   
      Stage<InstructionDecodeStep> idp(global, &tr, "ID", false);   
      Stage<ExecuteStep> exp(global, &tr, "EX", true);  
      Stage<MemoryStep> mem(global, &tr, "MEM", false);
      Stage<WriteBackStep> wb(global, &tr, "WB", false);  

      do {
        std::cout << "\nCycle " << global->cycle << std::endl;
        // Perform steps for each stage for the cycle, and perpare their "sends" (instructions that will goto the next stage after this cycle)
        ifp.run();
        idp.run();
        exp.run();
        mem.run();
        wb.run();

        // Move instructions to the next stage
        ifp.send(&idp);
        idp.send(&exp);
        exp.send(&mem);
        mem.send(&wb);

        global->cycle++;
      } while (global->totalInstCount > 0 || !global->traceEnded);
      
      return 0;
    }
    else {
      std::cout << "Insufficient number of arguments" << std::endl;
      return -1;
    }
      
}

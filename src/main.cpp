#include <iostream>
#include "Global.h"
#include "TraceReader.h"
#include "InstructionFetchStep.h"
#include "Stage.h"
#include "ExecuteStep.h"
#include "MemoryStep.h"
#include "WriteBackStep.h"
#include "DependencyManager.h"

int main(int argc, char const *argv[]) {
    Global * global = new Global(2, new DependencyManager());
    
    if (argc < 5) {
      std::cout << "Insufficient number of arguments" << std::endl;
      return -1;
    }
  

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
    Stage<InstructionFetchStep> * ifs = new Stage<InstructionFetchStep>(global, &tr, "IF");   
    Stage<InstructionDecodeStep> * id = new Stage<InstructionDecodeStep>(global, &tr, "id");   
    Stage<ExecuteStep> * ex = new Stage<ExecuteStep>(global, &tr, "ex");  
    Stage<MemoryStep> * mem = new Stage<MemoryStep>(global, &tr, "mem");
    Stage<WriteBackStep> * wb = new Stage<WriteBackStep>(global, &tr, "wb");  

    do {
      
      // Perform steps for each stage for the cycle, and perpare their "sends" (instructions that will goto the next stage after this cycle)
      ifs->run();
      id->run();
      ex->run();
      mem->run();
      wb->run();

      // Move instructions to the next stage
      ifs->send(id);
      id->send(ex);
      ex->send(mem);
      mem->send(wb);

      std::cout << "\nCycle: " << global->cycle << ", Instruction In System: " << global->totalInstCount << std::endl;
      global->cycle++;
    } while (global->totalInstCount > 0 || !global->traceEnded);

    delete ifs;
    delete id;
    delete ex;
    delete mem;
    delete wb;
    delete global;
    
    return 0;      
}

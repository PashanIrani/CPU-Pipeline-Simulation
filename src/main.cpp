#include <iostream>
#include <iomanip>
#include "Global.h"
#include "TraceReader.h"
#include "InstructionFetchStep.h"
#include "Stage.h"
#include "ExecuteStep.h"
#include "MemoryStep.h"
#include "WriteBackStep.h"
#include "DependencyManager.h"

void outputHistogram(Global *global) {
  double instructionPercent[6];
  instructionPercent[0] = ((double) global->numOfInstructions[0] / global->total_inst) * 100;
  instructionPercent[1] = ((double) global->numOfInstructions[1] / global->total_inst) * 100;
  instructionPercent[2] = ((double) global->numOfInstructions[2] / global->total_inst) * 100;
  instructionPercent[3] = ((double) global->numOfInstructions[3] / global->total_inst) * 100;
  instructionPercent[4] = ((double) global->numOfInstructions[4] / global->total_inst) * 100;
  instructionPercent[5] = ((double) global->numOfInstructions[5] / global->total_inst) * 100;
  std::cout << " ========== Histogram ========== " << std::endl;
  std::cout << std::showpoint << std::fixed;
  std::cout << std::setprecision(1) << "Total Integer Instructions: " << instructionPercent[0] << "%" << std::endl;
  std::cout << "Total Floating Point Instructions: " << instructionPercent[1] << "%" << std::endl;
  std::cout << "Total branch Instructions: " << instructionPercent[2] << "%" << std::endl;
  std::cout << "Total load Instructions: " << instructionPercent[3] << "%" << std::endl;
  std::cout << "Total store Instructions: " << instructionPercent[4] << "%" << std::endl;
  // std::cout << "Total undefined Instructions: " << instructionPercent[5] << "%" << std::endl;
}

int main(int argc, char const *argv[]) {
    DependencyManager * dm = new DependencyManager();
    Global * global = new Global(2, dm);
    
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
      
     TraceReader tr(global->file_name, global);
      
    if (global->START_INSTRUCTION < 0) {
      std::cout << "Start instruction cannot be negative" << std::endl;
      return -1;
    }

    // incrementing instruction from the file
    for (int i = 0; i < global->START_INSTRUCTION - 1; i++) {
      Instruction * nextInst = tr.getNextInst(true);

      // This will ignore the skipped instructions
      dm->add(nextInst);
      dm->markComplete(nextInst);

      if (i % 100000 == 0) {
        std::cout << "Getting Ready, please wait... " << ((double)i/global->START_INSTRUCTION) * 100.0 << "% complete" << std::endl;
      }

      if (nextInst == nullptr) {
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
      global->cycle++;
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

      if (global->cycle % 10000 == 0 || global->DEBUG) {
        std::cout << "Cycle: " << global->cycle << std::endl;
        std::cout << "Instruction In System: " << global->totalInstCount << std::endl;
        std::cout<< "Total Instructions: " << global->total_inst<<std::endl;
      }

    } while (global->totalInstCount > 0);

    std::cout << "Cycle: " << global->cycle << std::endl;
    std::cout << "Instruction In System: " << global->totalInstCount << std::endl;
    std::cout<< "Total Instructions: " << global->total_inst<<std::endl;

    outputHistogram(global);

    delete ifs;
    delete id;
    delete ex;
    delete mem;
    delete wb;
    delete global;
    
    return 0;      
}

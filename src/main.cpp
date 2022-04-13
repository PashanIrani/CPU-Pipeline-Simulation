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
  
    //TraceReader tr("./input/srv_0");
    TraceReader tr("./input/srv_subset_10_simplified");

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
      if(global->cycle > 7940) break;
    } while (global->totalInstCount > 0 || !global->traceEnded);

    delete ifs;
    delete id;
    delete ex;
    delete mem;
    delete wb;
    delete global;

    return 0;
}

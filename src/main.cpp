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
    global->W = 2;

    TraceReader tr("./input/srv_subset_10_simplified");

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

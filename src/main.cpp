#include <iostream>
#include "Global.h"
#include "TraceReader.h"

int main(int argc, char const *argv[]) {
    Global * global = new Global();
    TraceReader tr("./input/srv_subset_10");

    Instruction * next_inst;
    while((next_inst = tr.getNextInst()) != NULL) {
      next_inst->print();
    }
    return 0;
}

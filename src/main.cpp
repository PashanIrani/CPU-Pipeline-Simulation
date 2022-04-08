#include <iostream>
#include "Global.h"
#include "TraceReader.h"

int main(int argc, char const *argv[]) {
    Global * global = new Global();
    TraceReader tr("./input/srv_subset_10");
    tr.getNextInst()->print();
    tr.getNextInst()->print();
    tr.getNextInst()->print();
    tr.getNextInst()->print();
    tr.getNextInst()->print();
    tr.getNextInst()->print();
    tr.getNextInst()->print();
    tr.getNextInst()->print();
    tr.getNextInst()->print();
    tr.getNextInst()->print();
    tr.getNextInst()->print();
    tr.getNextInst()->print();
    tr.getNextInst()->print();
    return 0;
}

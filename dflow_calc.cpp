/* 046267 Computer Architecture - Winter 20/21 - HW #3               */
/* Implementation (skeleton)  for the dataflow statistics calculator */

#include "dflow_calc.h"
#include <vector>
using namespace std;
int cmd_count = 0;

class Node{
    int cmd_num;
    int opsLatency;
    InstInfo progTrace;
    Node* left_dep;
    Node* right_dep;
public:
    Node(int opLatency, InstInfo progTrace, Node* left_dep = nullptr, Node* right_dep = nullptr): opsLatency(opLatency), progTrace(progTrace), left_dep(left_dep), right_dep(right_dep){
        cmd_num = cmd_count;
        cmd_count++;
    }
    ~Node() = default;
};

class ProgCtx{
    vector<Node> graph;
    unsigned int numOfInsts;
public:
    ProgCtx();
    ~ProgCtx() = default;
};

ProgCtx analyzeProg(const unsigned int opsLatency[], const InstInfo progTrace[], unsigned int numOfInsts){
    ProgCtx ctx;
    for (int i = 0; i < numOfInsts; i++){}
    return PROG_CTX_NULL;
}

void freeProgCtx(ProgCtx ctx) {
}

int getInstDepth(ProgCtx ctx, unsigned int theInst) {
    return -1;
}

int getInstDeps(ProgCtx ctx, unsigned int theInst, int *src1DepInst, int *src2DepInst) {
    return -1;
}

int getProgDepth(ProgCtx ctx) {
    return 0;
}



/* 046267 Computer Architecture - Winter 20/21 - HW #3               */
/* Implementation (skeleton)  for the dataflow statistics calculator */

#include "dflow_calc.h"
#include <vector>
using namespace std;
#include <iostream>
int instCount = 0;

class Node{
public:
    int opsLatency;
    InstInfo progTrace;
    Node* left_dep;
    Node* right_dep;
    int instNum;
    Node(InstInfo progTrace, int opLatency = 0, Node* left_dep = nullptr, Node* right_dep = nullptr): 
        opsLatency(opLatency), progTrace(progTrace), left_dep(left_dep), right_dep(right_dep){
            instNum = instCount;
            instCount++;
        }
    ~Node() = default;
};

class Graph{
public:
    vector<Node> graph;
    unsigned int numOfInsts;
    Graph(const unsigned int opsLatency[] = nullptr, const InstInfo progTrace[] = nullptr, unsigned int numOfInsts = 0):
    numOfInsts(numOfInsts){
        // InstInfo entry;
        // Node entry_node = Node(entry);
        // graph.push_back(entry_node);

        for(int i = 0 ; i < numOfInsts ; i++){
            int dst1_index = findDstInCtx(progTrace[i].src1Idx);
            int dst2_index = findDstInCtx(progTrace[i].src2Idx);
            if(dst1_index == -1 && dst2_index == -1){
                graph.push_back(Node(progTrace[i], opsLatency[progTrace[i].opcode], nullptr, nullptr));

            }
            else if(dst1_index >= 0 && dst2_index >= 0){
                graph.push_back(Node(progTrace[i], opsLatency[progTrace[i].opcode], &graph[dst1_index], &graph[dst2_index]));
            }
            else{
                if(dst1_index >= 0){
                    graph.push_back(Node(progTrace[i], opsLatency[progTrace[i].opcode], &graph[dst1_index], nullptr));
                }
                else{
                    graph.push_back(Node(progTrace[i], opsLatency[progTrace[i].opcode], nullptr, &graph[dst2_index]));
                }
            } 
        }
        // InstInfo exit;
        // graph.push_back(Node(exit, 0, &graph[graph.size()-1]));
    }
    ~Graph() = default;
    int findDstInCtx(int src_idx){
        int graph_size = graph.size() - 1; 
        for(int i = graph_size; i >= 0 ; i--){
            if(graph[i].progTrace.dstIdx == src_idx) return i;
        }
        return -1;
    }
};

ProgCtx analyzeProg(const unsigned int opsLatency[], const InstInfo progTrace[], unsigned int numOfInsts){
    if (opsLatency == nullptr || progTrace == nullptr || numOfInsts <= 0) return PROG_CTX_NULL;
    Graph* prog = new Graph(opsLatency, progTrace, numOfInsts);
    return (ProgCtx)(prog);
}

void freeProgCtx(ProgCtx ctx) {
    delete (Graph*)ctx;
}

int getMaxPathAux (Graph* ctx, unsigned int inst, int max_path, int current_path){
    if (ctx->graph[inst].left_dep == nullptr && ctx->graph[inst].left_dep == nullptr){
        if (current_path > max_path) return current_path + ctx->graph[inst].opsLatency;
        else return max_path;
    }
    if (ctx->graph[inst].left_dep != nullptr){
        max_path= getMaxPathAux(ctx, ctx->graph[inst].left_dep->instNum, max_path, current_path + ctx->graph[inst].left_dep->opsLatency);
    }
    if (ctx->graph[inst].right_dep != nullptr){
        max_path= getMaxPathAux(ctx, ctx->graph[inst].right_dep->instNum, max_path, current_path + ctx->graph[inst].right_dep->opsLatency);
    }
    return max_path;
}


int getInstDepth(ProgCtx ctx, unsigned int theInst) {
    int max_path = 0;
    max_path = getMaxPathAux((Graph*)ctx, theInst, max_path, 0);
    return max_path;
}

int findDstInFinalCtx(Graph* prog, int src_idx){
    int graph_size = prog->graph.size() - 2; // avoid last instruction
    for(int i = graph_size; i >= 0 ; i--){
        if(prog->graph[i].progTrace.dstIdx == src_idx) return i;
    }
    return -1;
}

int getInstDeps(ProgCtx ctx, unsigned int theInst, int *src1DepInst, int *src2DepInst) {
    if(theInst == -1) return -1;
    Graph* prog = (Graph*)ctx;
    int src1 = findDstInFinalCtx(prog, prog->graph[theInst].progTrace.src1Idx);
    int src2 = findDstInFinalCtx(prog, prog->graph[theInst].progTrace.src2Idx);
    if(src1 >= 0){
        *src1DepInst = prog->graph[src1].instNum;
    }
    else *src1DepInst = src1;
    if(src2 >= 0){
        *src2DepInst = prog->graph[src2].instNum;
    }
    else *src2DepInst = src2;

    return 0;
}

int getProgDepth(ProgCtx ctx) {
    Graph* prog = (Graph*)ctx;
    int graph_size = prog->graph.size() - 1;
    return getInstDepth(ctx, graph_size) + prog->graph[graph_size].opsLatency;
}



/* 046267 Computer Architecture - Winter 20/21 - HW #3               */
/* Implementation (skeleton)  for the dataflow statistics calculator */

#include "dflow_calc.h"
#include <vector>
using namespace std;
#include <iostream>

int instCount = 0;


/**
 * Node Class: represents an instruction in the dependancies graph.
 * @arg opsLatency- the instruction's operation latency.
 * @arg progTrace- the instrucion info.
 * @arg left_dep- a pointer to a previous instruction, which the current instrucion depends on.
 * @arg right_dep- a pointer to a previous instruction, which the current instrucion depends on.
 * @arg instNum- the instruction number.
 * @arg longestpath- the greatest latency untill the current instruction can starts to run.
 * */
class Node{
public:
    int opsLatency;
    InstInfo progTrace;
    Node* left_dep;
    Node* right_dep;
    int instNum;
    int longestpath = 0;
    Node(InstInfo progTrace, int opLatency = 0, Node* left_dep = nullptr, Node* right_dep = nullptr): 
        opsLatency(opLatency), progTrace(progTrace), left_dep(left_dep), right_dep(right_dep){
            instNum = instCount;
            instCount++;
            if(right_dep && left_dep){
                longestpath += max(left_dep->longestpath + left_dep->opsLatency, right_dep->longestpath + right_dep->opsLatency);
            }
            else if (right_dep){
                longestpath += right_dep->longestpath + right_dep->opsLatency;
            }
            else if(left_dep){
                longestpath += left_dep->longestpath + left_dep->opsLatency;
            }
            else longestpath = 0;
        }
    ~Node() = default;
};

/**
 * Graph Class: represents dependancies graph.
 * @arg graph- vector of the instructions which represented in the dependencies graph.
 * @arg numOfInsts- vector size.
  * */
class Graph{
public:
    vector<Node> graph;
    unsigned int numOfInsts;
    Graph(const unsigned int opsLatency[] = nullptr, const InstInfo progTrace[] = nullptr, unsigned int numOfInsts = 0):
    numOfInsts(numOfInsts){
        graph.reserve(numOfInsts);
        //insert the instructions to the vector
        for(unsigned int i = 0 ; i < numOfInsts ; i++){
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
    }
    ~Graph() = default;
    //helping function for finding src_idx dependency's nodes.
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

int getInstDepth(ProgCtx ctx, unsigned int theInst) {
    Graph* prog = (Graph*)ctx;
    return prog->graph[theInst].longestpath;
}

int getInstDeps(ProgCtx ctx, unsigned int theInst, int *src1DepInst, int *src2DepInst) {
    Graph* prog = (Graph*)ctx;
    if(prog->graph[theInst].left_dep){
        *src1DepInst = prog->graph[theInst].left_dep->instNum;
    }
    else *src1DepInst = -1;
    if(prog->graph[theInst].right_dep){
        *src2DepInst = prog->graph[theInst].right_dep->instNum;
    }
    else *src2DepInst = -1;
    return 0;
}

int getProgDepth(ProgCtx ctx) {
    Graph* prog = (Graph*)ctx;
    int graph_size = prog->graph.size(), max = 0, index = 0;
    for(int i = 0 ; i < graph_size ; i++){
        if(max <= prog->graph[i].longestpath + prog->graph[i].opsLatency){
            max = prog->graph[i].longestpath + prog->graph[i].opsLatency;
            index = i;
        }
    }
    return prog->graph[index].longestpath + prog->graph[index].opsLatency;
}



/* 046267 Computer Architecture - Winter 20/21 - HW #3               */
/* Implementation (skeleton)  for the dataflow statistics calculator */

#include "dflow_calc.h"
#include <vector>
using namespace std;

class Node{
    int opsLatency;
    InstInfo progTrace;
    Node* left_dep;
    Node* right_dep;
public:
    Node(InstInfo progTrace, int opLatency = 0, Node* left_dep = nullptr, Node* right_dep = nullptr): 
        opsLatency(opLatency), progTrace(progTrace), left_dep(left_dep), right_dep(right_dep){}
    ~Node() = default;
};

class Graph{
    vector<Node> graph;
    unsigned int numOfInsts;
public:
    Graph(const unsigned int opsLatency[] = nullptr, const InstInfo progTrace[] = nullptr, unsigned int numOfInsts = 0):
            numOfInsts(numOfInsts){
        InstInfo entry;
        Node entry_node = Node(entry);
        graph.push_back(entry_node);
        for(int i = 0 ; i > numOfInsts ; i++){
            int dst1_index = findDstInCtx(progTrace[i].src1Idx);
            int dst2_index = findDstInCtx(progTrace[i].src2Idx);
            if(dst1_index == 0 && dst2_index == 0){
                graph.push_back(Node(progTrace[i], opsLatency[i], &entry_node, nullptr));
            }
            else if(dst1_index > 0 && dst2_index > 0){
                int index1 = getDstIndex(dst1_index);
                int index2 = getDstIndex(dst2_index);
                graph.push_back(Node(progTrace[i], opsLatency[i], &graph[index1], &graph[index2]));
            }
            else{
                if(dst1_index > 0){
                    int index1 = getDstIndex(dst1_index);
                    graph.push_back(Node(progTrace[i], opsLatency[i], &graph[index1], nullptr));
                }
                else{
                    int index2 = getDstIndex(dst2_index);
                    graph.push_back(Node(progTrace[i], opsLatency[i], nullptr, &graph[index2]));
                }
            } 
        }
        InstInfo exit;
        graph.push_back(Node(exit));
    }
    ~Graph() = default;
    bool findDstInCtx(int index){ return true; } // implementation
    int getDstIndex(int dst_index){ return 0; }  // implementation
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



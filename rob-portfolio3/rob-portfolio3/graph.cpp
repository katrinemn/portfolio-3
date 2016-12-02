#include "Graph.h"

Graph::Graph()
{

}

void Graph::addNode(Vertex v)
{
    nodes.push_back(v);
}

void Graph::addEdge(Vertex* parent, Vertex* v, int w)
{
    parent->weigt.push_back(w); // parent contains the information of edge weights
    parent->adj.push_back(v);
}

vector<Vertex>* Graph::getNodesPointer()
{
    return &nodes;
}

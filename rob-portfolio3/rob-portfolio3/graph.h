#pragma once
#include<iostream>
#include <list>
#include <stack>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
// Class to represent a graph
using namespace std;

struct Pixel
{
    int x;
    int y;

    Pixel(int x, int y) : x(x), y(y) {}

    bool operator==(const Pixel& rhs)
    {
        return this->x == rhs.x && this->y == rhs.y;
    }
};

struct Vertex
{
    Pixel data;
    Vertex* path; // parent
    bool known;
    int dist;
    vector<int> weigt;    
    list<Vertex*> adj; // Pointer to an array containing adjacency lists

    Vertex(Pixel n) : data(n){}
};

class Graph
{
    friend class Dijkstras;

public:
    Graph();   // Constructor

    void addNode(Vertex v);
    void addEdge(Vertex* parent, Vertex* v, int weight);
    vector<Vertex>* getNodesPointer();

    vector<Vertex> nodes;
protected:
    int V; // Number of vertices'


};

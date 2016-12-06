#pragma once
#include "Image.hpp"
#include "graph.h"

using namespace rw::sensor;

#define VERTEX 255
#define EDGE 255
#define PATH 0

class Robot
{
public:
	Robot(Image* map, int startX, int startY);
    void mapEnviroment();
    void saveInternMaps();
	void colorPath(vector<Vertex*> path); // to color path
	void listNodes(); // list index and x,y position

	vector<Vertex*> findReturnPath(Vertex& start, Vertex& goal, Graph& g);
	void find();
	~Robot();

private:
    void makeDrivingPath();
    Vertex* findClosestVertexTo(int x, int y);
    bool unvisitedVertex();
    Vertex* firstUnvistedVertex(Vertex* startPoint);
    void markDrivePath(queue<Vertex*> path);
    void markGraphMap();

    Pixel startPoint;
    int sensorLength = 8; //sensor reaches 8x8 around robot
	int xPos, yPos;
	Image* img;
    Image* graphMap;
    Image* drivingMap;
    Graph graph;
    queue<Vertex*> drivePath;
};


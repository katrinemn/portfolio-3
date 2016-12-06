#pragma once
#include "Image.hpp"
#include "graph.h"
#include "Sensor.hpp"
#include "Defines.h"

using namespace rw::sensor;

class Robot
{
public:
	Robot(Image* map, Image* tMap, int startX, int startY);
    void mapEnviroment();
    void saveInternMaps();
	void colorPath(vector<Vertex*> path); // to color path
	void listNodes(); // list index and x,y position
	void runRobot();

	vector<Vertex*> findReturnPath(Vertex* start, Vertex* goal);
	void find();
	~Robot();

private:
    void makeDrivingPath();
    Vertex* findClosestVertexTo(int x, int y);
    bool unvisitedVertex();
    Vertex* firstUnvistedVertex(Vertex* startPoint);
    void markDrivePath(queue<Vertex*> path);
    void markGraphMap();
	void colorPath(Pixel start, Pixel slut, Image* img);

	Sensor robSensor;
    Pixel startPoint;
    int gridSize = 8; //sensor reaches 8x8 around robot
	Pixel currPos;
	Image* img;
    Image* graphMap;
    Image* drivingMap;
	Image* sensorMap;
	Image* moveMap;
	Image* astarMap;
    Graph graph;
    queue<Vertex*> drivePath;
};


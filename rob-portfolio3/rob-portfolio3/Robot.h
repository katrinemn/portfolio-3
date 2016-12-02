#pragma once
#include "Image.hpp"
#include "graph.h"

using rw::sensor::Image;

#define VERTEX 100
#define EDGE 70

class Robot
{
public:
	Robot(Image* map);
    void mapEnviroment();
    void saveInternMaps();
	~Robot();

private:
    int sensorLength = 8; //sensor reaches 8x8 around robot
	int xPos, yPos;
	Image* img;
    Image* graphMap;
    Graph graph;
};


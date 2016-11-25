#pragma once
#include "Image.hpp"

using rw::sensor::Image;

class Robot
{
public:
	Robot();
	~Robot();

private:
	int xPos, yPos;
	Image* img;
};


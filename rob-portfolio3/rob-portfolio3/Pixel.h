#pragma once
#include "Image.hpp"

using rw::sensor::Image;
class Pixel
{
public:
	Pixel();
	int x, y;
	int getValue(Image* img );
	~Pixel();
};


#pragma once
#include "Image.hpp"

using rw::sensor::Image;
class Pixel
{
public:
	Pixel();
    Pixel(int X, int Y);
	int x, y;
	int getValue(Image* img );
	~Pixel();
};


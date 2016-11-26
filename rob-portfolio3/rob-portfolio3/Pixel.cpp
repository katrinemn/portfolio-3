#include "Pixel.h"



Pixel::Pixel()
{
}


Pixel::Pixel(int X, int Y)
{
    x =X;
    y = Y;
}

int Pixel::getValue(Image* img)
{
	return img->getPixelValuei(x, y, 0);
}



Pixel::~Pixel()
{
}

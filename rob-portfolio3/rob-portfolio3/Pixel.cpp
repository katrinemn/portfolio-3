#include "Pixel.h"



Pixel::Pixel()
{
}

int Pixel::getValue(Image* img)
{
	return img->getPixelValuei(x, y, 0);
}



Pixel::~Pixel()
{
}

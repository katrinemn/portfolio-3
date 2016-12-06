#include <iostream>
#include "Image.hpp"
#include "PPMLoader.hpp"
#include <vector>
#include <cmath>
#include <math.h>



using namespace rw::sensor;
using namespace rw::loaders;


int main(int argc) { 
    std::string filename("inputImage.pgm");
    std::cout << "loading image..." << std::endl;
    Image* img = PPMLoader::load(filename);

    // do stuff here
	int channel = 0; // allways 0 on grayscale image

	img->saveAsPGM("testout.pgm");

    system("pause");
}
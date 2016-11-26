#include <iostream>
#include "Image.hpp"
#include "PPMLoader.hpp"
#include <vector>
#include <cmath>
#include <math.h>
#include <string>


using namespace std;
using namespace rw::sensor;
using namespace rw::loaders;


int main(int argc) {
    std::string filename("inputImage.pgm");
    std::cout << "loading image..." << std::endl;
    Image* img = PPMLoader::load(filename);
    for( int i = 0; i < img->getWidth();i++)
    {
        for (int j = 0; j < img->getHeight();j++)
        {
            if(img->getPixelValuei(i, j, 0) == 255)
                img->setPixel8S(i, j, 0);
            else
                img->setPixel8S(i, j, 255);
        }
    }
    // do stuff here
	int channel = 0; // allways 0 on grayscale image

    std::cout << "Image loaded" << std::endl;

    std::cout << std::endl << "saving image..." << std::endl;
    //Save maps
	img->saveAsPGM("testout.pgm");

    system("pause");
}

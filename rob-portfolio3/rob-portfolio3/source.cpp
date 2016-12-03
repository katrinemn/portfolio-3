#include <iostream>
#include "Image.hpp"
#include "PPMLoader.hpp"
#include <vector>
#include <cmath>
#include <math.h>
#include "Robot.h"
#include "graph.h"
#include "Astar.h"


using namespace rw::sensor;
using namespace rw::loaders;


int main(int argc) {
	std::string filename("inputImage.pgm");
	std::cout << "loading image..." << std::endl;
	Image* img = PPMLoader::load(filename);

	// do stuff here
	int channel = 0; // allways 0 on grayscale image

	std::cout << "Image loaded" << std::endl;

	//make robot
	Robot roboKat(img);

	roboKat.mapEnviroment();
	
	// find() skal have nogen argumenter
	// men det afhænger lidt af om Katrine udstikker
	// koordinater eller Vertex pointer - eller noget helt 3.
	roboKat.find();

    //Save maps
    std::cout << std::endl << "saving image..." << std::endl;
    roboKat.saveInternMaps();

	return 0;
}

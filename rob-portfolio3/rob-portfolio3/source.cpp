#include <iostream>
#include "Image.hpp"
#include "PPMLoader.hpp"
#include <vector>
#include <cmath>
#include <math.h>
#include <string>
#include "Sensor.hpp"
#include "Robot.h"
#include "graph.h"
#include "Astar.h"

using namespace std;
using namespace rw::sensor;
using namespace rw::loaders;


int main(int argc) {
	std::string filename("inputImage.pgm");
	string inputTarget("inputTargets.pgm");
	std::cout << "loading image..." << std::endl;
	Image* img = PPMLoader::load(filename);
	Image* tImg = PPMLoader::load(inputTarget);

	// do stuff here
	int channel = 0; // allways 0 on grayscale image

	std::cout << "Image loaded" << std::endl;

    //make robot
    Robot roboKat(img, tImg, 13,13);

	roboKat.mapEnviroment();

	//run robot
	roboKat.runRobot();

    //Save maps
    std::cout << std::endl << "saving image..." << std::endl;
    roboKat.saveInternMaps();

	system("pause");

	return 0;
}

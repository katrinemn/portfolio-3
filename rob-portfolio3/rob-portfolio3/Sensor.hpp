//
//  Sensor.hpp
//  rob-portfolio3
//
//  Created by Katrine Maria Nielsen on 26/11/16.
//  Copyright © 2016 Katrine Maria Nielsen. All rights reserved.
//

#ifndef Sensor_hpp
#define Sensor_hpp
#include <stdio.h>
#include <vector>
#include <iostream>
#include "graph.h"
#include <math.h>
#include "Defines.h"
using namespace std;
class Sensor
{
public:
    Sensor();
    Pixel getSurroundings(Image* img, Image* imgTarget, Pixel begin, int distance);
    vector<Pixel> getRoute(Image* loadedImg, Pixel from, Pixel to);
    ~Sensor();
private:
    int Sign(double x);
    Pixel getObject(Pixel begin, Pixel current, Image* img, Image* imgTarget);
    
};
#endif /* Sensor_hpp */

//
//  Sensor.hpp
//  rob-portfolio3
//
//  Created by Katrine Maria Nielsen on 26/11/16.
//  Copyright © 2016 Katrine Maria Nielsen. All rights reserved.
//

#ifndef Sensor_hpp
#define Sensor_hpp
#include "Pixel.h"
#include <stdio.h>
#include <vector>
#include <iostream>
#include <tgmath.h>
#include "Defines.h"
using namespace std;
class Sensor
{
public:
    Sensor();
    Pixel getSurroundings(Image* img, Pixel begin, int distance);
    vector<Pixel> getRoute(Image* loadedImg, Pixel from, Pixel to);
    ~Sensor();
private:
    int Sign(double x);
    Pixel getObject(Pixel begin, Pixel current, Image* img);
    
};
#endif /* Sensor_hpp */

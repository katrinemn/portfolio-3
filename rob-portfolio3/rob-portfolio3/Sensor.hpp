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
class Sensor
{
public:
    Sensor();
    Pixel getSurroundings(Pixel begin, int distance);
    ~Sensor();
private:
};
#endif /* Sensor_hpp */

//
//  Sensor.cpp
//  rob-portfolio3
//
//  Created by Katrine Maria Nielsen on 26/11/16.
//  Copyright © 2016 Katrine Maria Nielsen. All rights reserved.
//

#include "Sensor.hpp"

Sensor::Sensor()
{
    
}

Pixel Sensor::getSurroundings(Image* img, Image* imgTargets, Pixel begin, int distance)
{
    Pixel target(0,0);
    for (int x = begin.x - distance; x <= begin.x + distance; x++)
    {
        Pixel current(x,begin.y-distance);
        target = getObject(begin, current, img, imgTargets);
        if(target.x && target.y)
            return target;
        Pixel current1(x,begin.y+distance);
        target = getObject(begin, current1, img, imgTargets);
        if(target.x && target.y)
            return target;
    }
    for (int y = begin.y - distance; y <= begin.y + distance; y++)
    {
        Pixel current(begin.x-distance,y);
        target = getObject(begin, current, img, imgTargets);
        if(target.x && target.y)
            return target;
        Pixel current1(begin.x+distance,y);
        target = getObject(begin, current1, img, imgTargets);
        if(target.x && target.y)
            return target;
    }
    img->setPixel8S(begin.x,begin.y , ROBOT_PIXEL);
    Pixel null(-1,-1);
    return null;
}


Pixel Sensor::getObject(Pixel begin, Pixel current, Image* img, Image* imgTarget)
{
    vector<Pixel> route = getRoute(img, begin, current);
    Pixel target(0,0);
    for (int i = 0 ; i < route.size() ; i++)
    {
        int index;
        
        if (route[0].x == begin.x && route[0].y == begin.y)
            index = i;
        else
            index = route.size()-1-i;
        
        if (img->getPixelValuei(route[index].x,route[index].y , 0) == BLACK_PIXEL)
            break;
        if (imgTarget->getPixelValuei(route[index].x,route[index].y , 0) == TARGET_PIXEL)
        {
            Pixel target(route[index].x,route[index].y);
            return target;
        }
        else
            img->setPixel8S(route[index].x,route[index].y , VISITED_PIXEL);
    }
    Pixel null(0,0);
    return null;
}

vector<Pixel> Sensor::getRoute(Image* loadedImg, Pixel from, Pixel to)
{
    vector<Pixel> route;
    double x1 = from.x;
    double y1 = from.y;
    double x2 = to.x;
    double y2 = to.y;
    
    const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
    if(steep)
    {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }
    
    if(x1 > x2)
    {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }
    
    const float dx = x2 - x1;
    const float dy = fabs(y2 - y1);
    
    float error = dx / 2.0;
    const int ystep = (y1 < y2) ? 1 : -1;
    int y = (int)y1;
    
    const int maxX = (int)x2;
    
    for(int x=(int)x1; x<maxX; x++)
    {
        if(steep)
        {
            Pixel pixel(y,x);
            route.push_back(pixel);
        }
        else
        {
            Pixel pixel(x,y);
            route.push_back(pixel);
        }
        
        error -= dy;
        if(error < 0)
        {
            y += ystep;
            error += dx;
        }
    }
    return route;
}

Sensor::~Sensor()
{
    
}
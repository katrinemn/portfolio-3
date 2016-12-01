#include "Robot.h"



Robot::Robot(Image* map)
{
    img = map;
    graphMap = map->copyFlip(false, false);    
}

void Robot::mapEnviroment()
{
    //run though map in sensor length bloks
    for (int y = 5; y < img->getHeight()-5; y += sensorLength*2)
    {
        for (int x = 5; x < img->getWidth()-5; x += sensorLength*2)
        {          
            //boundary check
            if (x + sensorLength >= img->getWidth() || y + sensorLength >= img->getHeight())
                break;

            Pixel center(x + sensorLength, y + sensorLength);

            if (img->getPixelValuei(center.x, center.y, 0) == 0)
                continue;

            Vertex newV(center);
            graph.addNode(newV);
        }
    }

    //Color vertexes
    for (auto i : graph.nodes)
    {
        graphMap->setPixel8U(i.data.x, i.data.y, VERTEX);
    }
}

void Robot::saveInternMaps()
{
    img->saveAsPGM("testout.pgm");
    graphMap->saveAsPGM("grafMap.pgm");
}

Robot::~Robot()
{
}

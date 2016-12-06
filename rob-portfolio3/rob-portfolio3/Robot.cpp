#include "Robot.h"



Robot::Robot(Image* map)
{
    img = map;
    graphMap = map->copyFlip(false, false);    
}

void Robot::mapEnviroment()
{
    //run though map in sensor length bloks
	// +- 5 is for black border around image
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

    //Make edges to adj vertexes
    for (int iIndex = 0; iIndex < graph.nodes.size(); iIndex++)
    {
        Vertex* i = &graph.nodes[iIndex];
        Pixel leftAdj(i->data.x - sensorLength*2, i->data.y);
        Pixel rightAdj(i->data.x + sensorLength*2, i->data.y);
        Pixel topAdj(i->data.x, i->data.y - sensorLength*2);
        Pixel bottomAdj(i->data.x, i->data.y + sensorLength*2);

        for (int vIndex = 0; vIndex < graph.nodes.size(); vIndex++)
        {
            //Got to use this way to get the right pointer so data gets saved 
            Vertex* v = &graph.nodes[vIndex];
            if (v->data == leftAdj || v->data == rightAdj || v->data == topAdj || v->data == bottomAdj)
            {
                bool clearPath = true;
                //Check for black between them too
                //if edge is vertical
                for (int y = i->data.y; y < v->data.y; y++)
                {
                    if (graphMap->getPixelValuei(i->data.x, y, 0) == 0)
                        clearPath = false;
                }
                //if edge is horizontal
                for (int x = i->data.x; x < v->data.x; x++)
                {
                    if (graphMap->getPixelValuei(x, i->data.y, 0) == 0)
                        clearPath = false;
                }

                //if no black was found the path is clear and edge can be added
                if(clearPath)
                    graph.addEdge(i, v, 8);
            }

            //If sentence for diagonal edges. forloop needs to be nested as both x and y moves each time. No straight lines

        }
    }


    //Color vertexes
    for (auto i : graph.nodes)
    {
        graphMap->setPixel8U(i.data.x, i.data.y, VERTEX);

        //color edges.
        for (auto e : i.adj)
        {
            //if edge is vertical
            for (int y = i.data.y; y < e->data.y; y++)            
                graphMap->setPixel8U(i.data.x , y, EDGE);
            //if edge is horizontal
            for (int x = i.data.x; x < e->data.x; x++)
                graphMap->setPixel8U(x, i.data.y, EDGE);
        }
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

#include "Robot.h"



Robot::Robot(Image* map, int startX, int startY)
{
    Pixel newP(10,10);
    this->startPoint = newP;

    img = map;
    graphMap = map->copyFlip(false, false);    
    drivingMap = map->copyFlip(false, false);
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
                for (int y = i->data.y; y > v->data.y; y--)
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
                for (int x = i->data.x; x > v->data.x; x--)
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

    //make path for robot. When made it can just pop the queue from start to finish to cover entire map
    makeDrivingPath();

    //mark driving path
    markDrivePath(drivePath);

    //Color vertexes
    markGraphMap();
}

void Robot::markGraphMap()
{
    //Color vertexes
    for (auto i : graph.nodes)
    {
        graphMap->setPixel8U(i.data.x, i.data.y, VERTEX);

        //color edges.
        for (auto e : i.adj)
        {
            //if edge is vertical
            for (int y = i.data.y; y < e->data.y; y++)
                graphMap->setPixel8U(i.data.x, y, EDGE);
            for (int y = i.data.y; y > e->data.y; y--)
                graphMap->setPixel8U(i.data.x, y, EDGE);
            //if edge is horizontal
            for (int x = i.data.x; x < e->data.x; x++)
                graphMap->setPixel8U(x, i.data.y, EDGE);
            for (int x = i.data.x; x > e->data.x; x--)
                graphMap->setPixel8U(x, i.data.y, EDGE);
        }
    }
}

void Robot::markDrivePath(queue<Vertex*> path)
{
    int color = 10;
    while (!path.empty())
    {
        Vertex* curr = path.front();
        path.pop();

        //Vertex
        drivingMap->setPixel8U(curr->data.x, curr->data.y, color);

        //Edge to next vertex
        if (!path.empty())
        {
            Vertex* next = path.front();
            //if edge is vertical
            for (int y = curr->data.y; y < next->data.y; y++)
                drivingMap->setPixel8U(curr->data.x, y, color);
            for (int y = curr->data.y; y > next->data.y; y--)
                drivingMap->setPixel8U(curr->data.x, y, color);
            //if edge is horizontal
            for (int x = curr->data.x; x < next->data.x; x++)
                drivingMap->setPixel8U(x, curr->data.y, color);
            for (int x = curr->data.x; x > next->data.x; x--)
                drivingMap->setPixel8U(x, curr->data.y, color);
        }

        //Increment color
        color += 2;
    }
}

void Robot::makeDrivingPath()
{
    //first point is start point. closest vertex to start point
    Vertex* currNode = findClosestVertexTo(startPoint.x, startPoint.y);
    drivePath.push(currNode);

    currNode->visited = true;

    while (unvisitedVertex())
    {
        //select first unvisited node
        bool noUnVisited = true;

        for (auto v : currNode->adj)
        {
            if (!v->visited)
            {
                //select node and mark node visited
                currNode = v;
                currNode->visited = true;
                drivePath.push(currNode);

                //set flag. If there was a node that wasn't visited set false
                noUnVisited = false;
            }
        }
        //if no nodes available that is unvisited find first/closest node in graph that is unvisited
        if (noUnVisited)
        {
            currNode = firstUnvistedVertex(currNode);
            currNode->visited = true;

            //Before Push next point, push the points it takes to get there to the queue. Use A* to find optimal path
            TODO;

            drivePath.push(currNode);            
        }
    }
}

Vertex* Robot::firstUnvistedVertex(Vertex* startPoint)
{
    //reset known on all nodes
    for (int i = 0; i < graph.nodes.size(); i++)
        graph.nodes[i].known = false;

    queue<Vertex*> search;    
    
    search.push(startPoint);
    startPoint->known = true;

    //Brushfire'ish approch to find closest connected node
    while (!search.empty())
    {
        Vertex* currV = search.front();
        search.pop();

        for (auto v : currV->adj)
        {
            if (!v->visited)
                return v;
            else if (!v->known)
            {
                search.push(v);
                v->known = true;
            }
        }
    }

}

bool Robot::unvisitedVertex()
{
    for (auto v : graph.nodes)
    {
        if (!v.visited)
            return true;
    }
    return false;
}

Vertex* Robot::findClosestVertexTo(int x, int y)
{
    Vertex* closest = &graph.nodes[0]; //first node
    double distance = 50000; //large number to init
    for (auto i = 0; i < graph.nodes.size(); i++)
    {
        Vertex v = graph.nodes[i];
        double newD = sqrt(pow(v.data.x - closest->data.x, 2) + pow(v.data.y - closest->data.y, 2));
        if (newD < distance)
        {
            distance = newD;
            closest = &graph.nodes[i];
        }
    }

    return closest;
}

void Robot::saveInternMaps()
{
    img->saveAsPGM("testout.pgm");
    graphMap->saveAsPGM("grafMap.pgm");
    drivingMap->saveAsPGM("drivingMap.pgm");
}

Robot::~Robot()
{
}

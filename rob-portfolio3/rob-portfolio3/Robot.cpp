#include "Robot.h"
#include "Astar.h"



Robot::Robot(Image* map, Image* tMap, int startX, int startY)
{
    Pixel newP(10,10);
    this->startPoint = newP;
	this->currPos = newP;

    img = map;
	sensorMap = tMap;
    graphMap = map->copyFlip(false, false);    
    drivingMap = map->copyFlip(false, false);
	moveMap = map->copyFlip(false, false);
	astarMap = map->copyFlip(false, false);
}

void Robot::runRobot()
{
	int targetsFound = 0;
	while(!drivePath.empty())
	{
		//Check sensor
		Pixel target = robSensor.getSurroundings(img, sensorMap, currPos, 16);

		if (target.x != -1 && target.y != -1)
		{
			targetsFound++;
			//color target as visited
			sensorMap->setPixel8U(target.x, target.y, VISITED_PIXEL);

			//target found. Move directly to target as sensor means it is within sight
			colorPath(currPos, target, moveMap, &searchPixels);
			currPos = target;

			//move to dropoff point
			Vertex* pos = findClosestVertexTo(currPos.x, currPos.y);
			Vertex* home = findClosestVertexTo(startPoint.x, startPoint.y);
			auto wayHome = findReturnPath(pos, home);

			//color and move to closest vertex
			colorPath(currPos, pos->data, astarMap, &transportPixel);
			currPos = pos->data;
			
			//move back to dropoff
			for (int i = 0; i < wayHome.size()-1; i++)
			{
				colorPath(wayHome[i]->data, wayHome[i + 1]->data, astarMap, &transportPixel);
			}

			//color from end vertex to dropoff
			currPos = wayHome[wayHome.size()-1]->data;
			colorPath(currPos, startPoint, astarMap, &transportPixel);


			//dropped off banana, now gotta get back
			//move to closest vertex
			pos = findClosestVertexTo(startPoint.x, startPoint.y);
			colorPath(startPoint, pos->data, astarMap, &transportPixel);
			currPos = pos->data;

			//find path back to next pos
			auto backToPoint = findReturnPath(pos, drivePath.front());

			//get back to where it saw the target, and keep searching
			for (int i = 0; i < backToPoint.size()-1; i++)
			{
				colorPath(backToPoint[i]->data, backToPoint[i + 1]->data, astarMap, &transportPixel);
			}
			currPos = backToPoint.back()->data;
			continue;
		}
		//go to next point in queue
		Vertex* next = drivePath.front();
		drivePath.pop();

		colorPath(currPos, next->data, moveMap, &searchPixels);
		currPos = next->data;
	}

	//number of targets found
	cout << "Targets found and dropped off: " << targetsFound << endl;
	cout << "Pixel traversed during search: " << searchPixels << endl;
	cout << "Pixel traversed during transport: " << transportPixel << endl;
	cout << "Pixel traversed during Total: " << searchPixels + transportPixel << endl;
}

void Robot::colorPath(Pixel from, Pixel to, Image* img, int* counter)
{
	double x1 = from.x;
	double y1 = from.y;
	double x2 = to.x;
	double y2 = to.y;

	const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
	if (steep)
	{
		std::swap(x1, y1);
		std::swap(x2, y2);
	}

	if (x1 > x2)
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

	for (int x = (int)x1; x<maxX; x++)
	{
		if (steep)
		{
			img->setPixel8U(y, x, VISITED_PIXEL);
			*counter = *counter + 1;
		}
		else
		{
			img->setPixel8U(x, y, VISITED_PIXEL);
			*counter = *counter + 1;
		}

		error -= dy;
		if (error < 0)
		{
			y += ystep;
			error += dx;
		}
	}
}

void Robot::mapEnviroment()
{
    //run though map in sensor length bloks
	// +- 5 is for black border around image
    for (int y = 5; y < img->getHeight()-5; y += gridSize*2)
    {
        for (int x = 5; x < img->getWidth()-5; x += gridSize*2)
        {          
            //boundary check
            if (x + gridSize >= img->getWidth() || y + gridSize >= img->getHeight())
                break;

            Pixel center(x + gridSize, y + gridSize);

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
        Pixel leftAdj(i->data.x - gridSize*2, i->data.y);
        Pixel rightAdj(i->data.x + gridSize*2, i->data.y);
        Pixel topAdj(i->data.x, i->data.y - gridSize*2);
        Pixel bottomAdj(i->data.x, i->data.y + gridSize*2);

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
    int color = 0;
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
        color += 1;
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
				break;
            }
        }
        //if no nodes available that is unvisited find first/closest node in graph that is unvisited
        if (noUnVisited)
        {
			Vertex* tempNode = currNode;
            currNode = firstUnvistedVertex(currNode);
            currNode->visited = true;

            //Before Push next point, push the points it takes to get there to the queue. Use A* to find optimal path
			auto extraPath = findReturnPath(tempNode, currNode);
			
			for (auto eV : extraPath)
			{
				drivePath.push(eV);
				eV->visited = true;
			}

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
	return nullptr;
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
        double newD = sqrt(pow(v.data.x - x, 2) + pow(v.data.y - y, 2));
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
	moveMap->saveAsPGM("moveMap.pgm");
	sensorMap->saveAsPGM("sensorMap.pgm");
	astarMap->saveAsPGM("astarmap.pgm");
}
void Robot::find()
{
	cout << "Start A* search" << endl;
	int start = 0;
	int goal = graph.nodes.size()-1;
	cout << "I USE START VERTEX: "<< start << " with (x,y) = (" << graph.nodes[start].data.x << "," << graph.nodes[start].data.y << ")"<< endl;
	cout << "I USE GOAL VERTEX: "<< goal << " with (x,y) = (" << graph.nodes[goal].data.x << "," << graph.nodes[goal].data.y<< ")" << endl;
	findReturnPath(&graph.nodes[start], &graph.nodes[goal]);
}
vector<Vertex*> Robot::findReturnPath(Vertex* start, Vertex* goal)
{	

	AStar as;
	vector<Vertex*> dummy;
	dummy = as.searchAStar(start, goal);
	//colorPath(dummy);
	
	return dummy;
}

void Robot::listNodes()
{
	int counter = 0;
	for (auto v : graph.nodes)
	{
		cout << "VERTEX: " << counter << " has (x,y) = (" << v.data.x << "," << v.data.y << ")" << endl;
		counter++;
	}
}

void Robot::colorPath(vector<Vertex*> path)
{
	//cout << path.size() << endl;

	for (auto v : path)
		graphMap->setPixel8U(v->data.x, v->data.y, PATH);
		//cout << v->data.x << "," << v->data.y << endl;
}

Robot::~Robot()
{
}

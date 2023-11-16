#include "gv.h"
#include "my_header.h"

#include "resource.h"

#include <math.h>
#include <stdint.h>

#include "clipper2/clipper.h"

#define my_min(a,b) ((a)<(b)?(a):(b))
#define my_max(a,b) ((a)>(b)?(a):(b))

using namespace std;

void init_cuts(void)
{
	numOfCuts = sizeof(cuts) / sizeof(cuts[0]);

	for (size_t i = 0; i < numOfCuts; i++)
	{
		if ((int)(cuts[i].p[0].x*1e6) == (int)(cuts[i].p[1].x*1e6))
		{
			if ((int)(cuts[i].p[0].y*1e6) == (int)(cuts[i].p[1].y*1e6))
				cuts[i].type = CUT_ONE_POINT;
			else
				cuts[i].type = CUT_VER;
		}
		else if((int)(cuts[i].p[0].y*1e6) == (int)(cuts[i].p[1].y*1e6))
			cuts[i].type = CUT_HOR;
		else
			cuts[i].type = CUT_DEFAULT;
	}
}

void init_poligons(void)
{
	numOfPolys = sizeof(polygons) / sizeof(polygons[0]);

	for (size_t i = 0; i < numOfPolys; i++)
	{
		for (size_t j = 0; j < numOfCuts; j++)
			polygons[i].edge[j] = cuts[j];	
	}

	for (uint8_t i = 0; i < numOfCuts; i++)
	{
		for (uint8_t j = 0; j < 2; j++)
		{
			polygons[0].edge[i].p[j].x -= 3.0;
			polygons[0].edge[i].p[j].y -= 2.0;

			polygons[1].edge[i].p[j].x += 0.0;
			polygons[1].edge[i].p[j].y += 1.0;

			polygons[2].edge[i].p[j].x += 3.0;
			polygons[2].edge[i].p[j].y -= 2.0;
		}
	}
}

void find_area(void)
{
	start.x = polygons[0].edge[0].p[0].x;
	start.y = polygons[0].edge[0].p[0].y;

	stop.x = start.x;
	stop.y = start.y;

	for (size_t i = 0; i < numOfPolys; i++)
	{
		for (size_t j = 0; j < numOfCuts; j++)
		{
			for (uint8_t k = 0; k < 2; k++)
			{
				if (polygons[i].edge[j].p[k].x < start.x)
					start.x = polygons[i].edge[j].p[k].x;

				if (polygons[i].edge[j].p[k].y < start.y)
					start.y = polygons[i].edge[j].p[k].y;

				if (polygons[i].edge[j].p[k].x > stop.x)
					stop.x = polygons[i].edge[j].p[k].x;

				if (polygons[i].edge[j].p[k].y > stop.y)
					stop.y = polygons[i].edge[j].p[k].y;
			}
		}
	}

	stepX = (stop.x - start.x) / 100.0;
	stepY = (stop.y - start.y) / 100.0;

	scaleDraw = (my_min(drawArea.Width(), drawArea.Height()) / my_max((stop.x - start.x), (stop.y - start.y))) * 0.8;
}

void find_optimum(void)
{
	PointD currentPoint = { start.x, start.y };
	position currentPosition = { 0.0 };
	//double tmpPos[2] = { 0.0 };

	//uint32_t posCnt = 0;
	//uint32_t optPosCnt = 0;

	optimalPosition.distances = new myDistance[numOfPolys];
	optimalPosition.maxDistance.dist = count_simple_distance(start, stop); // диагональ квадрата

	for (currentPoint.x = start.x; currentPoint.x <= stop.x; currentPoint.x += stepX)
	{
		for (currentPoint.y = start.y; currentPoint.y <= stop.y; currentPoint.y += stepY)
		{
			//posCnt++;

			//if (posCnt == 130)
			//	int b = 0;

			currentPosition = find_all_distances(currentPoint);

			//tmpPos[0] = currentPosition.distances[0].dist;
			//tmpPos[1] = currentPosition.distances[1].dist;

			if (currentPosition.maxDistance.dist < optimalPosition.maxDistance.dist)
			{
				//optPosCnt = posCnt;
				optimalPosition = currentPosition;
			}
		}
	}

	isOptimalFound = true;
}

bool isPointOutOfPoly(PointD point, poly thePoly)
{
	Clipper2Lib::PointD currentPoint;
	Clipper2Lib::PathD currentPoly;

	for (size_t i = 0; i < numOfCuts; i++)
	{
		currentPoint.x = thePoly.edge[i].p[0].x;
		currentPoint.y = thePoly.edge[i].p[0].y;

		currentPoly.push_back(currentPoint);
	}

	currentPoint.x = point.x;
	currentPoint.y = point.y;

	if (Clipper2Lib::PointInPolygon(currentPoint, currentPoly) == Clipper2Lib::PointInPolygonResult::IsOutside)
		return true;
	else
		return false;
}

myDistance find_distance_to_poly(PointD point, poly polygone)
{
	position thisPosition = { 0 };
	double simpleDistances[2] = { 0.0 };

	if (isPointOutOfPoly(point, polygone))
	{
		thisPosition.maxDistance.dist = count_simple_distance(start, stop); // диагональ квадрата
		thisPosition.distances = new myDistance[numOfCuts];

		for (size_t i = 0; i < numOfCuts; i++)
		{
			thisPosition.distances[i].intsPoint = find_ints_point(point, polygone.edge[i]);

			if ((my_min(polygone.edge[i].p[0].x, polygone.edge[i].p[1].x) <= thisPosition.distances[i].intsPoint.x) && (thisPosition.distances[i].intsPoint.x <= my_max(polygone.edge[i].p[0].x, polygone.edge[i].p[1].x)) &&
				(my_min(polygone.edge[i].p[0].y, polygone.edge[i].p[1].y) <= thisPosition.distances[i].intsPoint.y) && (thisPosition.distances[i].intsPoint.y <= my_max(polygone.edge[i].p[0].y, polygone.edge[i].p[1].y)))
			{
				thisPosition.distances[i].dist = count_simple_distance(point, thisPosition.distances[i].intsPoint);
			}
			else
			{
				for (uint8_t j = 0; j < 2; j++)
				{
					simpleDistances[j] = count_simple_distance(point, polygone.edge[i].p[j]);
				}

				if (simpleDistances[0] < simpleDistances[1])
				{
					thisPosition.distances[i].dist = simpleDistances[0];
					thisPosition.distances[i].intsPoint = polygone.edge[i].p[0];
				}
				else
				{
					thisPosition.distances[i].dist = simpleDistances[1];
					thisPosition.distances[i].intsPoint = polygone.edge[i].p[1];
				}
			}

			if (thisPosition.distances[i].dist < thisPosition.maxDistance.dist)
				thisPosition.maxDistance = thisPosition.distances[i];
		}
	}
	else
	{
		thisPosition.maxDistance.dist = 0.0;
		thisPosition.maxDistance.intsPoint = point;
	}

	thisPosition.point = point;

	return thisPosition.maxDistance;
}

position find_all_distances(PointD point)
{
	position thisPosition = { 0 };

	thisPosition.maxDistance.dist = 0.0;
	thisPosition.distances = new myDistance[numOfPolys];

	for (size_t i = 0; i < numOfPolys; i++)
	{
		thisPosition.distances[i] = find_distance_to_poly(point, polygons[i]);

		if (thisPosition.distances[i].dist > thisPosition.maxDistance.dist)
			thisPosition.maxDistance = thisPosition.distances[i];
	}

	thisPosition.point = point;

	return thisPosition;
}

PointD find_ints_point(PointD point, cut theCut)
{
	PointD intsPoint = { 0.0 };

	switch (theCut.type)
	{
	case CUT_DEFAULT:
		intsPoint = find_default_line(point, theCut); break;
	case CUT_HOR:
		intsPoint = find_hor_line(point, theCut); break;
	case CUT_VER:
		intsPoint = find_ver_line(point, theCut); break;
	case CUT_ONE_POINT:
		intsPoint = point; break;
	default: break;
	}

	return intsPoint;
}

PointD find_default_line(PointD point, cut theCut)
{
	double h_replace = 0.0;
	PointD intsPoint = { 0.0 };

	h_replace = (theCut.p[1].y - theCut.p[0].y) / (theCut.p[1].x - theCut.p[0].x);
	intsPoint.x = (theCut.p[0].x*h_replace + (point.x / h_replace) + point.y - theCut.p[0].y) / (h_replace + (1 / h_replace));
	intsPoint.y = ((intsPoint.x - point.x)*(theCut.p[0].x - theCut.p[1].x)) / (theCut.p[1].y - theCut.p[0].y) + point.y;
	
	return intsPoint;
}

PointD find_hor_line(PointD point, cut theCut)
{
	PointD intsPoint = { 0.0 };

	intsPoint.x = point.x;
	intsPoint.y = theCut.p[0].y;

	return intsPoint;
}

PointD find_ver_line(PointD point, cut theCut)
{
	PointD intsPoint = { 0.0 };

	intsPoint.x = theCut.p[0].x;
	intsPoint.y = point.y;

	return intsPoint;
}

double count_simple_distance(PointD p1, PointD p2)
{
	return sqrt((p2.x - p1.x)*(p2.x - p1.x) + (p2.y - p1.y)*(p2.y - p1.y));
}

void init_draw_dialog(void)
{
	drawDialog = new MyDialog();
	
	drawDialog->Create(IDD_DIALOG1, drawDialog);

	drawDialog->GetClientRect(drawArea);
	drawAreaCenter.x = drawArea.Width() / 2;
	drawAreaCenter.y = drawArea.Height() / 2;

	drawDialog->ShowWindow(SW_SHOW);
}

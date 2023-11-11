#include "gv.h"
#include "my_header.h"

#include "resource.h"

#include <math.h>
#include <stdint.h>

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

void find_area(void)
{
	start.x = cuts[0].p[0].x;
	start.y = cuts[0].p[0].y;

	stop.x = start.x;
	stop.y = start.y;

	for (size_t i = 0; i < numOfCuts; i++)
	{
		for (uint8_t j = 0; j < 2; j++)
		{
			if(cuts[i].p[j].x < start.x)
				start.x = cuts[i].p[j].x;

			if (cuts[i].p[j].y < start.y)
				start.y = cuts[i].p[j].y;

			if (cuts[i].p[j].x > stop.x)
				stop.x = cuts[i].p[j].x;

			if (cuts[i].p[j].y > stop.y)
				stop.y = cuts[i].p[j].y;
		}
	}

	stepX = (stop.x - start.x) / 100.0;
	stepY = (stop.y - start.y) / 100.0;

	scaleDraw = (my_min(drawArea.Width(), drawArea.Height()) / my_max((stop.x - start.x), (stop.y - start.y))) * 0.8;
}

void find_optimum(void)
{
	PointD currentPoint = { start.x, start.y };
	position currentPosition = {0.0};

	optimalPosition.maxDistance.dist = count_simple_distance(start, stop); // диагональ квадрата

	for (currentPoint.x = start.x; currentPoint.x <= stop.x; currentPoint.x += stepX)
	{
		for (currentPoint.y = start.y; currentPoint.y <= stop.y; currentPoint.y += stepY)
		{
			currentPosition = find_all_distances(currentPoint);

			if (currentPosition.maxDistance.dist < optimalPosition.maxDistance.dist)
				optimalPosition = currentPosition;
		}
	}

	isOptimalFound = true;
}

position find_all_distances(PointD point)
{
	position thisPosition = {0};

	double simpleDistances[2] = { 0.0 };

	thisPosition.maxDistance.dist = 0.0;

	for (size_t i = 0; i < numOfCuts; i++)
	{
		thisPosition.distances[i].intsPoint = find_ints_point(point, cuts[i]);

		if ((my_min(cuts[i].p[0].x, cuts[i].p[1].x) <= thisPosition.distances[i].intsPoint.x) && (thisPosition.distances[i].intsPoint.x <= my_max(cuts[i].p[0].x, cuts[i].p[1].x)) &&
			(my_min(cuts[i].p[0].y, cuts[i].p[1].y) <= thisPosition.distances[i].intsPoint.y) && (thisPosition.distances[i].intsPoint.y <= my_max(cuts[i].p[0].y, cuts[i].p[1].y)))
		{
			thisPosition.distances[i].dist = count_simple_distance(point, thisPosition.distances[i].intsPoint);
		}
		else
		{
			for (uint8_t j = 0; j < 2; j++)
			{
				simpleDistances[j] = count_simple_distance(point, cuts[i].p[j]);
			}

			if (simpleDistances[0] < simpleDistances[1])
			{
				thisPosition.distances[i].dist = simpleDistances[0];
				thisPosition.distances[i].intsPoint = cuts[i].p[0];
			}
			else
			{
				thisPosition.distances[i].dist = simpleDistances[1];
				thisPosition.distances[i].intsPoint = cuts[i].p[1];
			}
		}

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

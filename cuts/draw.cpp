#include "my_header.h"
#include <stdint.h>

void draw_cuts(void)
{
	POINT currentPoints[2] = { 0 };

	for (size_t i = 0; i < numOfCuts; i++)
	{
		for (uint8_t j = 0; j < 2; j++)
		{
			currentPoints[j].x = drawAreaCenter.x + (cuts[i].p[j].x * scaleDraw);
			currentPoints[j].y = drawAreaCenter.y - (cuts[i].p[j].y * scaleDraw);
		}

		mydc->MoveTo(currentPoints[0]);
		mydc->LineTo(currentPoints[1]);
	}
}

void draw_optimal_position(void)
{
	POINT currentPoints[2] = { 0 };

	currentPoints[0].x = drawAreaCenter.x + (optimalPosition.point.x * scaleDraw);
	currentPoints[0].y = drawAreaCenter.y - (optimalPosition.point.y * scaleDraw);

	for (size_t i = 0; i < numOfCuts; i++)
	{
		currentPoints[1].x = drawAreaCenter.x + (optimalPosition.distances[i].intsPoint.x * scaleDraw);
		currentPoints[1].y = drawAreaCenter.y - (optimalPosition.distances[i].intsPoint.y * scaleDraw);

		mydc->MoveTo(currentPoints[0]);
		mydc->LineTo(currentPoints[1]);
	}
}

void draw_x0(void)
{
	POINT currentPoint = { 0 };

	if (isOptimalFound)
	{
		currentPoint.x = drawAreaCenter.x + (optimalPosition.point.x * scaleDraw);
		currentPoint.y = drawAreaCenter.y - (optimalPosition.point.y * scaleDraw);

		mydc->Ellipse(currentPoint.x - 3, currentPoint.y - 3, currentPoint.x + 3, currentPoint.y + 3);
	}
}
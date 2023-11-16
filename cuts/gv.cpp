#include "gv.h"
//#include "MyDialog.h"

cut cuts[4] = {
	//{{-1.0, -2.0}, {-2.0, 5.0}},
	//{{2.0, 2.0}, {-1.0, 6.0}},
	//{{7.0, -1.0}, {8.0, -8.0}},
	//{{-8.0, -6.0}, {0.0, -5.0}},

	//{{{-2.0, 0.0}, {0.0, 2.0}}, CUT_DEFAULT},
	//{{{0.0, 2.0}, {2.0, 0.0}}, CUT_DEFAULT},
	//{{{2.0, 0.0}, {0.0, -2.0}}, CUT_DEFAULT},
	//{{{0.0, -2.0}, {-2.0, 0.0}}, CUT_DEFAULT},

	{{{-2.0, -2.0}, {-2.0, 2.0}}, CUT_DEFAULT},
	{{{-2.0, 2.0}, {2.0, 2.0}}, CUT_DEFAULT},
	{{{2.0, 2.0}, {2.0, -2.0}}, CUT_DEFAULT},
	{{{2.0, -2.0}, {-2.0, -2.0}}, CUT_DEFAULT},

	//{{{-2.0, 0}, {-2.0, 0}}, CUT_DEFAULT},
	//{{{0, 2.0}, {0, 2.0}}, CUT_DEFAULT},
	//{{{2.0, 0}, {2.0, 0}}, CUT_DEFAULT},
	//{{{0, -2.0}, {0, -2.0}}, CUT_DEFAULT},
};

poly polygons[3] = { 0.0 };

PointD start = {0.0}, stop = { 0.0 };
double stepX = 0.0, stepY = 0.0;
position currentPosition = { 0.0 };
position optimalPosition = { 0.0 };
size_t numOfCuts = 0;
size_t numOfPolys = 0;

MyDialog *drawDialog;
POINT* myPoints;
CPaintDC* mydc;

CPen blackpen(PS_SOLID, 1, RGB(0, 0, 0));
CPen redpen(PS_SOLID, 1, RGB(255, 0, 0));
CPen bluepen(PS_SOLID, 1, RGB(0, 0, 255));
CPen greenpen(PS_SOLID, 1, RGB(0, 255, 0));

CRect drawArea;
POINT drawAreaCenter;

double scaleDraw = 0.0;
bool isOptimalFound = false;
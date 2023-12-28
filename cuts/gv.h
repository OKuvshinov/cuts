#pragma once

#include "afxdialogex.h"
#include "MyDialog.h"

#ifndef GLOBAL_VARIABLES_HEADER

typedef enum
{
	CUT_DEFAULT = 0,
	CUT_HOR,
	CUT_VER,
	CUT_ONE_POINT
} cutType;

typedef struct
{
	double x;
	double y;
} PointD;

typedef struct
{
	PointD p[2];
	cutType type;
} cut;

typedef struct
{
	double dist;
	PointD intsPoint;
} myDistance;

typedef struct
{
	PointD point;
	myDistance *distances;
	myDistance maxDistance;
} position;

typedef struct
{
	cut edge[4];
	bool isOnPoint;
} poly;

//////////

extern cut cuts[4];
extern poly polygons[4];
extern poly basePolygone;

extern PointD start, stop;
extern double stepX, stepY;
extern position optimalPosition;
extern size_t numOfCuts;
extern size_t numOfPolys;

extern MyDialog *drawDialog;
extern POINT* myPoints;
extern CPaintDC* mydc;

extern CPen blackpen;
extern CPen redpen;
extern CPen bluepen;
extern CPen greenpen;

extern CRect drawArea;
extern POINT drawAreaCenter;

extern double scaleDraw;
extern bool isOptimalFound;

extern PointD shifts[4];

extern double PI;

#define GLOBAL_VARIABLES_HEADER

#endif

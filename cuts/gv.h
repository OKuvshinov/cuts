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
} distance;

typedef struct
{
	PointD point;
	distance distances[4];
	distance maxDistance;
} position;

typedef struct
{
	cut edge[5];
	bool isOnPoint;
} poly;

//////////

extern cut cuts[4];
extern PointD start, stop;
extern double stepX, stepY;
extern position optimalPosition;
extern size_t numOfCuts;

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

#define GLOBAL_VARIABLES_HEADER

#endif

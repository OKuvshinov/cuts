#pragma once

#include "gv.h"

void init_cuts(void);
void init_poligons(void);
void find_area(void);
void find_optimum(void);

position find_all_distances(PointD point);
PointD find_ints_point(PointD point, cut theCut);
PointD find_default_line(PointD point, cut theCut);
PointD find_hor_line(PointD point, cut theCut);
PointD find_ver_line(PointD point, cut theCut);
double count_simple_distance(PointD p1, PointD p2);

myDistance find_distance_to_poly(PointD point, poly polygone);
bool isPointOutOfPoly(PointD point, poly thePoly);

void init_draw_dialog(void);
void draw_polygons(void);
void draw_cuts(poly thePolygone);
void draw_optimal_position(void);
void draw_x0(void);
void draw_base(void);
void draw_axis(void);
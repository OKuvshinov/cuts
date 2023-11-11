#pragma once

#include "gv.h"

void init_cuts(void);
void find_area(void);
void find_optimum(void);

position find_all_distances(PointD point);
PointD find_ints_point(PointD point, cut theCut);
PointD find_default_line(PointD point, cut theCut);
PointD find_hor_line(PointD point, cut theCut);
PointD find_ver_line(PointD point, cut theCut);
double count_simple_distance(PointD p1, PointD p2);

void init_draw_dialog(void);
void draw_cuts(void);
void draw_optimal_position(void);
void draw_x0(void);
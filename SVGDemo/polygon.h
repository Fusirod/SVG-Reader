#pragma once
#ifndef POLYGON_H
#define POLYGON_H

#include "Shape.h"

class polygon : public Shape
{
public:
	vector<point> poly;
	void draw(Graphics& gra, List lst) override;
};

class polyline : public Shape
{
public:
	vector<point> poly;
	void draw(Graphics& gra, List lst);
};

void readPolygon(string name, string val, polygon* gon);
void readPolyline(string name, string val, polyline* line);

#endif

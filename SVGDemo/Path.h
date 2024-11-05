#pragma once
#ifndef PATH_H
#define PATH_H

#include "Shape.h"

class Path : public Shape
{
public:
	string data;
	string linecap, linejoin;
	Path() {
		linecap = "butt";
		linejoin = "miter";
	}
	void readSinglePoint(string data, int& idx, point& p);
	void readSinglePoint(string data, int& idx);
	void draw(Graphics& gra, List lst) override;
};

void readPath(string name, string val, Path* path);

#endif



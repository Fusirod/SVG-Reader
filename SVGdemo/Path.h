#pragma once
#ifndef PATH_H
#define PATH_H

#include "Shape.h"

class Path : public shape
{
public:
	string data;
	string linecap, linejoin;
	Path() {
		linecap = "butt";
		linejoin = "miter";
	}
	void readSinglePointFirst(string data, int& idx, point& p);
	float readSinglePointSecond(string data, int& idx);
	void draw(Graphics& gra, List lst) override;
};

void readPath(string name, string val, Path* path);

#endif



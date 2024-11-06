#pragma once
#ifndef GRADIENT_H
#define GRADIENT_H
#include "View.h"

class stop {
public:
	color colorStop;
	float offset;
	float opacityStop;
	stop() {
		offset = 0;
		opacityStop = 1;
	}
};

class Gradient {
public:
	vector<stop> stopList;
	Change changes;
	float* getPointList();
	Color* getColorList();
};

class linearGradient : public Gradient {
public:
	string id;
	point start, end;
	string units;
	bool percent;
};

class radialGradient : public Gradient {
public:
	string id;
	point center;
	float r, rx, ry;
	string units, link_href;
	radialGradient() {
		r = rx = ry = 0;
		link_href = "";
	}
};

class List {
public:	
	vector<linearGradient> linearList;
	vector<radialGradient> radialList;
};

void readStop(string name, string val, stop& st);
void readLinear(string name, string val, linearGradient& linear);
void readRadial(string name, string val, radialGradient& radial);
#endif
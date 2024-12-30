#pragma once
#ifndef GRADIENT_H
#define GRADIENT_H

#include "viewBox.h"

class stopPoint {
public:
	colour stopColor;
	float offset, stopOpacity;
	stopPoint()
		: offset(0), stopOpacity(1) {}
};

class Gradient
{
public:
	vector<stopPoint> stopList;
	transformShape trs;
	float* getPointList();
	Color* getColorList();
};

class linearGradient : public Gradient {
public:
	string id;
	point start, end;
	string units;
	bool percentage = false;
};

class radialGradient : public Gradient {
public:
	string id;
	point center;
	float r = 0;
	float fx = 0;
	float fy = 0;
	string units;
	string xlink_href = "";
};

class defs {
public:
	vector<linearGradient> lgList;
	vector<radialGradient> rgList;
};

template <typename T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
	return (v < lo) ? lo : (v > hi) ? hi : v;
}

void readStop(string name, string val, stopPoint& stop);
void readLinearGradient(string name, string val, linearGradient& lg);
void readRadialGradient(string name, string val, radialGradient& rg);
#endif

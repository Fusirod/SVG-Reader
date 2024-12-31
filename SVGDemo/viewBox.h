#pragma once
#ifndef VIEWBOX_H
#define VIEWBOX_H

#include "Transform.h"

class viewBox {
public:
	float width, height, minX, minY;
	float boxWidth, boxHeight;
	viewBox() 
		: width(1000), height(500), minX(0), minY(0) {}

	void setAttributes(const string& attri) {
		istringstream iss(attri);
		iss >> minX >> minY >> boxWidth >> boxHeight;
	}
};

#endif
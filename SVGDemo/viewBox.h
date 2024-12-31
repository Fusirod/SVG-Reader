#pragma once
#ifndef VIEWBOX_H
#define VIEWBOX_H

#include "Transform.h"

class viewBox {
public:
	float width, height, minX, minY, boxWidth, boxHeight;
	viewBox() 
		: width(0), height(0), minX(0), minY(0) {}
	viewBox(float w, float h, float x, float y, float bw, float bh)
		: width(w), height(h), minX(x), minY(y), boxWidth(bw), boxHeight(bh) {}

	void setAttributes(const string& attri) {
		istringstream iss(attri);
		iss >> minX >> minY >> boxWidth >> boxHeight;
	}
};

#endif

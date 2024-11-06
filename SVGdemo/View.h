#pragma once
#ifndef VIEW_H
#define VIEW_H
#include "Change.h"

class View
{
public:
	float width, height, minX, minY;
	float widthView, heightView;
	View() {
		width = 800;
		height = 600;
		minX = 0;
		minY = 0;
	}

	void setAttributes(const string& viewStr) {
		stringstream ss(viewStr);
		ss >> minX >> minY >> widthView >> heightView;
	}
};

#endif


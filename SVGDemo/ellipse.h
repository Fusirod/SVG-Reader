#pragma once
#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "Shape.h"

class ellipse : public Shape
{
public:
	float rx, ry;
	ellipse() {
		rx = rx = 0;
	}
	void draw(Graphics& gra, List lst) override;
};

void readEllipse(string name, string val, ellipse* ell);

#endif



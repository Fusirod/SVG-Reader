#pragma once
#ifndef CIRCLE_H
#define CIRCLE_H

#include "Shape.h"

class circle : public Shape 
{
public:
	point center;
	float r;
	circle() {
		r = 0;
	}
	void draw(Graphics& gra, List lst) override;
};

void readCircle(string name, string val, circle* cir);

#endif


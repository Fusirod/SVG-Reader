#pragma once
#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Shape.h"

class rectangle : public Shape
{
public:
	float width, height;
	void draw(Graphics& gra, List lst) override;
};

void readRectangle(string name, string val, rectangle* rec);

#endif

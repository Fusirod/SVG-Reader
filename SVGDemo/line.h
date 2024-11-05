#pragma once
#ifndef LINE_H
#define LINE_H

#include "Shape.h"

class line : public Shape
{
	point end;
	void draw(Graphics& gra, List lst) override;
};

void readLine(string name, string val, line* l);
#endif



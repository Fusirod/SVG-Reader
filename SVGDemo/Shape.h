#pragma once
#ifndef SHAPE_H
#define SHAPE_H

#include "Gradient.h"

class Shape
{
public:
	point start;
	color strokeColor, fillColor;
	float strokeWidth;
	float strokeOpacity, fillOpacity;
	string strokeStr, fillStr;
	Change changes;

	Shape() {
		strokeStr = fillStr = "";
		strokeWidth = 0;
		strokeOpacity = fillOpacity = 1;
	}

	virtual void draw(Graphics& gra, List lst) = 0;
};

class text : public Shape
{
public:
	float fontSize;
	string textFile;
	string fontFamily;
	string textAnchor;
	bool italic;
	float dx, dy;
	text() {
		textFile = "";
		textAnchor = "start";
		fontFamily = "Arial";
		italic = false;
		dx = dy = 0;
	}

	void draw(Graphics& gra, List lst) override;
};

void readText(string name, string val, text* t);

#endif



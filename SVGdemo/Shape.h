#pragma once
#ifndef SHAPE_H
#define SHAPE_H

#include "Gradient.h"

class shape
{
public:
	point start;
	color strokeColor, fillColor;
	float strokeWidth;
	float strokeOpacity, fillOpacity;
	string strokeID, fillID;
	Change changes;

	shape() {
		strokeID = fillID = "";
		strokeWidth = 0;
		strokeOpacity = fillOpacity = 1;
	}

	virtual void draw(Graphics& gra, List lst) = 0;
};

class circle : public shape
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

class rectangle : public shape
{
public:
	float width, height;
	void draw(Graphics& gra, List lst) override;
};

void readRectangle(string name, string val, rectangle* rec);

class ellipse : public shape
{
public:
	float rx, ry;
	ellipse() {
		rx = rx = 0;
	}
	void draw(Graphics& gra, List lst) override;
};

void readEllipse(string name, string val, ellipse* ell);

class line : public shape
{
public:
	point end;
	void draw(Graphics& gra, List lst) override;
};

void readLine(string name, string val, line* l);

class polygon : public shape
{
public:
	vector<point> poly;
	void draw(Graphics& gra, List lst) override;
};

class polyline : public shape
{
public:
	vector<point> poly;
	void draw(Graphics& gra, List lst);
};

void readPolygon(string name, string val, polygon* gon);
void readPolyline(string name, string val, polyline* line);

class text : public shape
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



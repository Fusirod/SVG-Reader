#pragma once
#ifndef SHAPES_H
#define SHAPES_H

#include "Gradient.h"

class shapes {
public:
    point start;
    colour strokeColor, fillColor;
    float strokeWidth;
    float strokeOpacity, fillOpacity;
    string strokeId, fillId; 
    transformShape trs;

    shapes() 
        : strokeWidth(0), strokeOpacity(1), fillOpacity(1), strokeId(""), fillId("") {}

    virtual void draw(Graphics& graphics, defs def) = 0;
    virtual ~shapes() {}
};


class line : public shapes {
public:
	point end;
	void draw(Graphics& graphics, defs def) override;
};

class rectangle : public shapes {
public:
	float width, height;
    rectangle()
        : width(0), height(0) {}

	void draw(Graphics& graphics, defs def) override;
};

class circle : public shapes {
public:
    float r;
    circle()
        : r(0) {}

    void draw(Graphics& graphics, defs def) override;
};

class ellipse : public shapes {
public:
    float rx, ry;
    ellipse()   
        : rx(0), ry(0) {}

    void draw(Graphics& graphics, defs def) override;
};

class text : public shapes {
public:
    float fontSize = 12;
    string content = "";
    string fontFamily = "Times New Roman";
    string textAnchor = "start";
    bool italic = false;
    float dx = 0, dy = 0;

    void draw(Graphics& graphics, defs def) override;
};

class polygon : public shapes {
public:
    vector<point> points;
    polygon() {}
    void draw(Graphics& graphics, defs def) override;
};

class polyline : public shapes {
public:
    vector<point> points;
    polyline() {}
    void draw(Graphics& graphics, defs def) override;
};

void readLine(string name, string value, line* line);
void readCircle(string name, string value, circle* cir);
void readRectangle(string name, string value, rectangle* rect);
void readEllipse(string name, string value, ellipse* elli);
void readPolygon(string name, string value, polygon* polygon);
void readPolyline(string name, string value, polyline* polyline);
void readText(string name, string value, text* text);

#endif // !SHAPES_H





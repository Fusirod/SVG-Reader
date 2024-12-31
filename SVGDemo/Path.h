#pragma once
#ifndef PATH_H
#define PATH_H

#include "Shapes.h"

const float M_PI = 3.14159265358979323846;

class Path : public shapes {
public:
	string data;
	string linecap = "butt";
	string linejoin = "miter";
	point previous_control;

	void readSinglePointFirst(const string& data, int& idx, point& p);
	float readSinglePointSecond(const string& data, int& idx);
	void draw(Graphics& graphic, defs def) override;
	void handleClosePath(GraphicsPath& path, point& currentPoint, point& startPoint, char& lastCommand);
	void handleArcTo(GraphicsPath& path, const string& data, int& idx, point& currentPoint, char command);
	void handleQuadraticBezier(const string& data, int& idx, char command, GraphicsPath& path, point& currentPoint, char& lastCommand);
	void handleSmoothCubicBezier(const string& data, int& idx, char command, GraphicsPath& path, point& currentPoint, point& d2, char& lastCommand);
	void handleMoveTo(const string& data, int& idx, char command, GraphicsPath& path, point& currentPoint, point& startPoint, bool& firstPoint, char& lastCommand);
	void handleLineTo(const string& data, int& idx, char command, GraphicsPath& path, point& currentPoint, char& lastCommand);
	void handleHorizontalLineTo(const string& data, int& idx, char command, GraphicsPath& path, point& currentPoint, char& lastCommand);
	void handleVerticalLineTo(const string& data, int& index, char command, GraphicsPath& path, point& current_point, char& last_command);
	void handleCubicBezier(const string& data, int& index, char command, GraphicsPath& path, point& current_point, point& d2, char& last_command);
	void handleSmoothQuadraticBezier(const string& data, int& idx, char command, GraphicsPath& path, point& currentPoint, point& lastControlPoint, char& lastCommand);
	void configurePen(Pen& pen, const string& linecap, const string& linejoin);
};

void readPath(const string& name, const string& value, Path* path);
#endif 





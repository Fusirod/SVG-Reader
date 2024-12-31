#pragma once
#ifndef SVGELEMENT_H
#define SVGELEMENT_H

#include <iostream>
#include "rapidxml.hpp" 
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h> 
#include <locale>  
#include <codecvt> 
#include <algorithm>
#include <regex>
#include <unordered_map>
#include "stdafx.h"

using namespace std;
using namespace Gdiplus;
using namespace rapidxml;
#pragma comment (lib,"Gdiplus.lib")


class colour
{
public:
	int red, green, blue;
	colour()
		: red(0), green(0), blue(0) {}
};

class point
{
public:
	float x, y;
	point()
		: x(0), y(0) {}
	point(float xVal, float yVal) 
		: x(xVal), y(yVal) {}
};

string trim(const string& str);
bool check(char c);
void removeSpace(string& str);
float clarify(string str);

colour readRGB(string val);
vector<point> readPoint(string value);

#endif 





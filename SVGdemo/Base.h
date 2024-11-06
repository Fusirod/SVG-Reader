#ifndef BASE_H
#define BASE_H

#include <iostream>
#include "rapidxml.hpp"
#include "stdafx.h"
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <windows.h>
#include <shellapi.h>
#include <objidl.h>
#include <gdiplus.h> 
#include <locale>  
#include <codecvt> 
#include <algorithm>
#include <regex>
#include <unordered_map>

using namespace std;
using namespace rapidxml;
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

const double M_PI = 3.14159265358979323846;

class color {
public:
	int red, green, blue;
	color() {
		red = green = blue = 0;
	}
};

class point {
public:
	float x, y;
	point() {
		x = 0, y = 0;
	}
};

string trim(string s);
bool check(char a);
void removeSpace(string& s);
float clarify(string s);

color readRGB(string val);
vector<point> readPoints(string value);


#endif

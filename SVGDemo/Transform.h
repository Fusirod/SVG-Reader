#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "SVGElement.h"

class transformShape {
public:
	vector<string> type;
	vector<float> value;
};

void transformImage(Graphics& graphic, float angle, float width, float height, float scrollX, float scrollY, float scale);
void readTransform(string val, transformShape& trs);
void applyTransform(LinearGradientBrush& lgbrush, transformShape& trs);

#endif 

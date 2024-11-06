#pragma once
#ifndef CHANGE_H
#define CHANGE_H

#include "Base.h"
class Change
{
public:
	vector<string> types;
	vector<float> values;
};

void image(Graphics& grap, float width, float height, float scrollX, float scrollY, float scale, float angle);
void readTransform(string value, Change& ch);
void apply(LinearGradientBrush& lgbrush, Change& ch);

#endif

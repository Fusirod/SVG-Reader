#pragma once
#ifndef GROUP_H
#define GROUP_H

#include "Path.h"

class Group
{
	unordered_map<string, string> attributes;
	void traversalGroup(rapidxml::xml_node<>* root, float& maxHeight, vector<Shape*> shapes, List& lst);
};

vector<Shape*> readFile(string fileName, float& maxWidth, float& maxHeight, List& lst, View& vb);

#endif
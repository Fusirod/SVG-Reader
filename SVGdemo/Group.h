#pragma once
#ifndef GROUP_H
#define GROUP_H

#include "Path.h"

class group {
public:
	unordered_map<string, string> attributes;
	//void traversalGroup(rapidxml::xml_node<>* root, float& maxWidth, float& maxHeight, vector<shape*>& shapes, List& lst);
	void traversalGroup(xml_node<>* root, float& maxWidth, float& maxHeight, vector<shape*> shapes, List& lst);
};

vector<shape*> readFile(string fileName, float& maxWidth, float& maxHeight, List& lst, View& vb);

#endif
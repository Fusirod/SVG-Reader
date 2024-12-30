#pragma once
#ifndef GROUP_H
#define GROUP_H

#include "Path.h"

class Group
{
public:
	unordered_map<string, string> attributes;
	void groupTraversal(rapidxml::xml_node<>* root, float& maxWidth, float& maxHeight, vector<shapes*>& shape, defs& def);
	void processLine(xml_node<>* node, vector<shapes*>& shape);
	void processRectangle(xml_node<>* node, vector<shapes*>& shape);
	void processEllipse(xml_node<>* node, vector<shapes*>& shape);
	void processCircle(xml_node<>* node, vector<shapes*>& shape);
	void processPolygon(xml_node<>* node, vector<shapes*>& shape);
	void processPolyline(xml_node<>* node, vector<shapes*>& shape);
	void processText(xml_node<>* node, vector<shapes*>& shape);
	void processPath(xml_node<>* node, vector<shapes*>& shape);
	void processGroup(xml_node<>* node, float& maxWidth, float& maxHeight, vector<shapes*>& shape, defs& def);
	void processDefs(xml_node<>* node, defs& def);
	void processLinearGradient(xml_node<>* node, defs& def);
	void processRadialGradient(xml_node<>* node, defs& def);
};

vector<shapes*> readFile(const string& fileName, float& maxWidth, float& maxHeight, defs& def, viewBox& vb);

#endif 



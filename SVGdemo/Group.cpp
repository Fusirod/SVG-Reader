#include "Group.h"

vector<shape*> readFile(string fileName, float& maxWidth, float& maxHeight, List& lst, View& v) {
	vector<shape*> shapes;
	ifstream fin(fileName);

	vector<char> buffer((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
	buffer.push_back('\0');

	xml_document<> doc;
	doc.parse<0>(&buffer[0]);
	xml_node<>* root = doc.first_node("svg");
	for (xml_attribute<>* attri = root->first_attribute(); attri; attri = attri->next_attribute()) {
		string attributeName = attri->name();
		string attributeValue = attri->value();
		if (attributeName == "viewBox")
			v.setAttributes(attributeValue);

		else if (attributeName == "width") {
			if (attributeValue[attributeValue.length() - 1] == 'x')
				attributeValue = attributeValue.substr(0, attributeValue.length() - 2);
			v.width = stof(attributeValue);
		}

		else if (attributeName == "height") {
			if (attributeValue[attributeValue.length() - 1] == 'x')
				attributeValue = attributeValue.substr(0, attributeValue.length() - 2);
			v.height = stof(attributeValue);
		}
	}

	maxWidth = 0, maxHeight = 0;
	group gr;
	gr.traversalGroup(root, maxWidth, maxHeight, shapes, lst);
	for (int i = 0; i < lst.radialList.size(); i++) {
		if (lst.radialList[i].link_href != "") {
			string id = lst.radialList[i].link_href.substr(1);
			for (int j = 0; j < lst.linearList.size(); i++) {
				if (lst.linearList[j].id == id) {
					lst.radialList[i].stopList = lst.linearList[i].stopList;
					break;
				}
			}
		}
	}
	return shapes;
}

void group::traversalGroup(xml_node<>* root, float& maxWidth, float& maxHeight, vector<shape*> shapes, List& lst) {
	if (!root) return;
	for (xml_node<>* node = root->first_node(); node; node = node->next_sibling()) {
		string name = node->name();
		if (name == "line") {
			line* tmpLine = new line();
			for (const auto& attri : attributes) {
				readLine(attri.first, attri.second, tmpLine);
			}

			for (xml_attribute<>* attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute()) {
				readLine(attribute->name(), attribute->value(), tmpLine);
			}
			shapes.push_back(tmpLine);
		}
		else if (name == "rect") {
			rectangle* tmpRect = new rectangle();
			for (const auto& attri : attributes) {
				readRectangle(attri.first, attri.second, tmpRect);
			}

			for (xml_attribute<>* attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute()) {
				string attributeName = attribute->name();
				string attributeValue = attribute->value();
				readRectangle(attributeName, attributeValue, tmpRect);
			}
			shapes.push_back(tmpRect);
		}
		else if (name == "ellipse") {
			ellipse* tmpEllip = new ellipse();
			for (const auto& attri : attributes) {
				readEllipse(attri.first, attri.second, tmpEllip);
			}

			for (xml_attribute<>* attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute()) {
				readEllipse(attribute->name(), attribute->value(), tmpEllip);
			}
			shapes.push_back(tmpEllip);
		}
		else if (name == "circle") {
			circle* tmpCir = new circle();
			for (const auto& attri : attributes) {
				readCircle(attri.first, attri.second, tmpCir);
			}

			for (xml_attribute<>* attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute()) {
				readCircle(attribute->name(), attribute->value(), tmpCir);
			}
			shapes.push_back(tmpCir);
		}
		else if (name == "polygon") {
			polygon* polyg = new polygon();
			for (const auto& attri : attributes) {
				readPolygon(attri.first, attri.second, polyg);
			}

			for (xml_attribute<>* attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute()) {
				readPolygon(attribute->name(), attribute->value(), polyg);
			}
			shapes.push_back(polyg);
		}
		else if (name == "polyline") {
			polyline* polyl = new polyline();
			for (const auto& attri : attributes) {
				readPolyline(attri.first, attri.second, polyl);
			}

			for (xml_attribute<>* attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute()) {
				readPolyline(attribute->name(), attribute->value(), polyl);
			}
			shapes.push_back(polyl);
		}
		else if (name == "text") {
			text* tmpText = new text();
			for (const auto& attri : attributes) {
				readText(attri.first, attri.second, tmpText);
			}

			for (xml_attribute<>* attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute()) {
				readText(attribute->name(), attribute->value(), tmpText);
			}
			tmpText->textFile = node->value();
			shapes.push_back(tmpText);
		}
		else if (name == "path") {
			Path* tmpPath = new Path();
			for (const auto& attri : attributes) {
				readPath(attri.first, attri.second, tmpPath);
			}

			for (xml_attribute<>* attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute()) {
				readPath(attribute->name(), attribute->value(), tmpPath);
			}
			shapes.push_back(tmpPath);
		}
		else if (name == "g") {
			group newGroup;
			for (const auto& attri : this->attributes	) {
				newGroup.attributes[attri.first] = attri.second;
			}

			string tran = "transform";
			for (xml_attribute<>* attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute()) {
				auto it = newGroup.attributes.find(attribute->name());
				if (it != newGroup.attributes.end()) {
					if (attribute->name() == tran) {
						newGroup.attributes[attribute->name()] = it->second + " " + attribute->value();
					}
					else {
						newGroup.attributes[attribute->name()] = attribute->value();
					}
				}
				else {
					newGroup.attributes[attribute->name()] = attribute->value();
				}
			}
			newGroup.traversalGroup(node, maxWidth, maxHeight, shapes, lst);
		}
		else if (name == "defs") {
			for (xml_node<>* child = node->first_node(); child; child = child->next_sibling()) {
				string childName = child->name();
				if (childName == "linearGradient") {
					linearGradient lg;
					for (xml_attribute<>* attribute = child->first_attribute(); attribute; attribute = attribute->next_attribute()) {
						readLinear(attribute->name(), attribute->value(), lg);
					}

					for (xml_node<>* grandChild = child->first_node(); grandChild; grandChild = grandChild = grandChild->next_sibling()) {
						string s = grandChild->name();
						if (s == "stop") {
							stop newStop;
							for (xml_attribute<>* attribute = grandChild->first_attribute(); attribute; attribute = attribute->next_attribute()) {
								readStop(attribute->name(), attribute->value(), newStop);
							}
							lg.stopList.push_back(newStop);
						}
					}
					lst.linearList.push_back(lg);
				}
				else if (childName == "radialGradient") {
					radialGradient rg;
					for (xml_attribute<>* attribute = child->first_attribute(); attribute; attribute = attribute->next_attribute()) {
						readRadial(attribute->name(), attribute->value(), rg);
					}
					for (xml_node<>* grandchild = child->first_node(); grandchild; grandchild = grandchild->next_sibling()) {
						string s = grandchild->name();
						if (s == "stop") {
							stop newStop;
							for (rapidxml::xml_attribute<>* attribute = grandchild->first_attribute(); attribute; attribute = attribute->next_attribute()) {
								readStop(attribute->name(), attribute->value(), newStop);
							}
							rg.stopList.push_back(newStop);
						}
					}
					lst.radialList.push_back(rg);
				}
			}
		} 
		else if (name == "linearGradient") {
			linearGradient lg;
			for (rapidxml::xml_attribute<>* attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute()) {
				readLinear(attribute->name(), attribute->value(), lg);
			}
			for (rapidxml::xml_node<>* grandchild = node->first_node(); grandchild; grandchild = grandchild->next_sibling()) {
				string s = grandchild->name();
				if (s == "stop") {
					stop newStop;
					for (rapidxml::xml_attribute<>* attribute = grandchild->first_attribute(); attribute; attribute = attribute->next_attribute()) {
						readStop(attribute->name(), attribute->value(), newStop);
					}
					lg.stopList.push_back(newStop);
				}
			}
			lst.linearList.push_back(lg);
		}
		else if (name == "radialGradient") {
			radialGradient rg;
			for (rapidxml::xml_attribute<>* attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute()) {
				readRadial(attribute->name(), attribute->value(), rg);
			}
			for (rapidxml::xml_node<>* grandchild = node->first_node(); grandchild; grandchild = grandchild->next_sibling()) {
				string s = grandchild->name();
				if (s == "stop") {
					stop newStop;
					for (rapidxml::xml_attribute<>* attribute = grandchild->first_attribute(); attribute; attribute = attribute->next_attribute()) {
						readStop(attribute->name(), attribute->value(), newStop);
					}
					rg.stopList.push_back(newStop);
				}
			}
			lst.radialList.push_back(rg);
		}
	}
}
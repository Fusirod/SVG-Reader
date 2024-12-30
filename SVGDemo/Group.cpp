#include "stdafx.h"
#include "Group.h"

vector<shapes*> readFile(const string& fileName, float& maxWidth, float& maxHeight, defs& def, viewBox& vb) {
	ifstream file(fileName);
	if (!file.is_open()) {
		throw runtime_error("Cannot open file: " + fileName);
	}

	vector<shapes*> shapes;
	vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	buffer.push_back('\0'); 

	xml_document<> doc;
	doc.parse<0>(&buffer[0]);
	xml_node<>* root = doc.first_node("svg");
	if (!root) {
		throw runtime_error("Invalid SVG format: Missing root <svg> element.");
	}


	for (xml_attribute<>* attribute = root->first_attribute(); attribute; attribute = attribute->next_attribute()) {
		string attributeName = attribute->name();
		string attributeValue = attribute->value();
		if (attributeName == "viewBox") {
			vb.setAttributes(attributeValue);
		}

		if (attributeName == "width" || attributeName == "height") {
			size_t pos = 0;
			while (pos < attributeValue.size() && (isdigit(attributeValue[pos]) || attributeValue[pos] == '.')) {
				pos++;
			}
			attributeValue = attributeValue.substr(0, pos);
			if (attributeName == "width")
				vb.width = stof(attributeValue);
			else
				vb.height = stof(attributeValue);
		}
	}
	maxWidth = 0, maxHeight = 0;
	Group g;
	g.groupTraversal(root, maxWidth, maxHeight, shapes, def);
	unordered_map<string, Gradient> gradientMap;
	for (const auto& lg : def.lgList) {
		gradientMap[lg.id] = lg;
	}

	for (auto& rg : def.rgList) {
		if (!rg.xlink_href.empty()) {
			string id = rg.xlink_href.substr(1);
			if (gradientMap.find(id) != gradientMap.end()) {
				rg.stopList = gradientMap[id].stopList;
			}
		}
	}
	return shapes;
}

void Group::groupTraversal(rapidxml::xml_node<>* root, float& maxWidth, float& maxHeight, vector<shapes*>& shape, defs& def) {
	if (!root)
		return;

	for (xml_node<>* node = root->first_node(); node; node = node->next_sibling()) {
		string name = node->name();

		if (name == "line")
			processLine(node, shape);

		else if (name == "rect")
			processRectangle(node, shape);

		else if (name == "ellipse")
			processEllipse(node, shape);

		else if (name == "circle")
			processCircle(node, shape);

		else if (name == "polygon")
			processPolygon(node, shape);

		else if (name == "polyline")
			processPolyline(node, shape);

		else if (name == "text")
			processText(node, shape);

		else if (name == "path")
			processPath(node, shape);

		else if (name == "g")
			processGroup(node, maxWidth, maxHeight, shape, def);

		else if (name == "defs")
			processDefs(node, def);

		else if (name == "linearGradient")
			processLinearGradient(node, def);

		else if (name == "radialGradient")
			processRadialGradient(node, def);
	}
}

void Group::processLine(xml_node<>* node, vector<shapes*>& shape) {
	//line* tmpLine = new line();
	unique_ptr<line> tmpLine = make_unique<line>();
	auto processAttri = [&](const string& name, const string& value) {
		readLine(name, value, tmpLine.get());
		};

	for (const auto& attri : attributes)
		//readLine(attri.first, attri.second, tmpLine);
		processAttri(attri.first, attri.second);

	for (xml_attribute<>* attri = node->first_attribute(); attri; attri = attri->next_attribute())
		//readLine(attri->name(), attri->value(), tmpLine);
		processAttri(attri->name(), attri->value());

	shape.push_back(tmpLine.release());
}

void Group::processRectangle(xml_node<>* node, vector<shapes*>& shape) {
	//rectangle* tmpRect = new rectangle();
	unique_ptr<rectangle> tmpRect = make_unique<rectangle>();
	auto processAttri = [&](const string& name, const string& value) {
		readRectangle(name, value, tmpRect.get());
		};
	for (const auto& attri : attributes)
		//readRectangle(attri.first, attri.second, tmpRect);
		processAttri(attri.first, attri.second);

	for (xml_attribute<>* attri = node->first_attribute(); attri; attri = attri->next_attribute())
		//readRectangle(attri->name(), attri->value(), tmpRect);
		processAttri(attri->name(), attri->value());
	
	shape.push_back(tmpRect.release());
}

void Group::processEllipse(xml_node<>* node, vector<shapes*>& shape) {
	//ellipse* tmpEllipse = new ellipse();
	unique_ptr<ellipse> tmpEllipse = make_unique<ellipse>();
	auto processAttri = [&](const string& name, const string& value) {
		readEllipse(name, value, tmpEllipse.get());
		};

	for (const auto& attri : attributes)
		//readEllipse(attri.first, attri.second, tmpEllipse);
		processAttri(attri.first, attri.second);
	
	for (xml_attribute<>* attri = node->first_attribute(); attri; attri = attri->next_attribute())
		//readEllipse(attri->name(), attri->value(), tmpEllipse);
		processAttri(attri->name(), attri->value());
	
	shape.push_back(tmpEllipse.release());
}

void Group::processCircle(xml_node<>* node, vector<shapes*>& shape) {
	//circle* tmpCircle = new circle();
	unique_ptr<circle> tmpCircle = make_unique<circle>();
	auto processAttri = [&](const string& name, const string& value) {
		readCircle(name, value, tmpCircle.get());
		};

	for (const auto& attri : attributes)
		//readCircle(attri.first, attri.second, tmpCircle);
		processAttri(attri.first, attri.second);

	for (xml_attribute<>* attri = node->first_attribute(); attri; attri = attri->next_attribute())
		//readCircle(attri->name(), attri->value(), tmpCircle);
		processAttri(attri->name(), attri->value());

	shape.push_back(tmpCircle.release());
}

void Group::processPolygon(xml_node<>* node, vector<shapes*>& shape) {
	//polygon* tmpPolygon = new polygon();
	unique_ptr<polygon> tmpPolygon = make_unique<polygon>();
	auto processAttri = [&](const string& name, const string& value) {
		readPolygon(name, value, tmpPolygon.get());
		};

	for (const auto& attri : attributes)
		//readPolygon(attri.first, attri.second, tmpPolygon);
		processAttri(attri.first, attri.second);

	for (xml_attribute<>* attri = node->first_attribute(); attri; attri = attri->next_attribute())
		//readPolygon(attri->name(), attri->value(), tmpPolygon);
		processAttri(attri->name(), attri->value());

	shape.push_back(tmpPolygon.release());
}

void Group::processPolyline(xml_node<>* node, vector<shapes*>& shape) {
	//polyline* tmpPolyline = new polyline();
	unique_ptr<polyline> tmpPolyline = make_unique<polyline>();
	auto processAttri = [&](const string& name, const string& value) {
		readPolyline(name, value, tmpPolyline.get());
		};

	for (const auto& attri : attributes)
		//readPolyline(attri.first, attri.second, tmpPolyline);
		processAttri(attri.first, attri.second);
	
	for (xml_attribute<>* attri = node->first_attribute(); attri; attri = attri->next_attribute())
		//readPolyline(attri->name(), attri->value(), tmpPolyline);
		processAttri(attri->name(), attri->value());
	
	shape.push_back(tmpPolyline.release());
}

void Group::processText(xml_node<>* node, vector<shapes*>& shape) {
	//text* tmpText = new text();
	unique_ptr<text> tmpText = make_unique<text>();
	auto processAttri = [&](const string& name, const string& value) {
		readText(name, value, tmpText.get());
		};

	for (const auto& attri : attributes)
		//readText(attri.first, attri.second, tmpText);
		processAttri(attri.first, attri.second);
	
	for (xml_attribute<>* attri = node->first_attribute(); attri; attri = attri->next_attribute())
		//readText(attri->name(), attri->value(), tmpText);
		processAttri(attri->name(), attri->value());
	
	tmpText->content = node->value();
	shape.push_back(tmpText.release());
}

void Group::processPath(xml_node<>* node, vector<shapes*>& shape) {
	//Path* tmpPath = new Path();
	unique_ptr<Path> tmpPath = make_unique<Path>();
	auto processAttri = [&](const string& name, const string& value) {
		readPath(name, value, tmpPath.get());
		};

	for (const auto& attri : attributes)
		//readPath(attribute.first, attribute.second, tmpPath);
		processAttri(attri.first, attri.second);
	
	for (xml_attribute<>* attri = node->first_attribute(); attri; attri = attri->next_attribute())
		//readPath(attri->name(), attri->value(), tmpPath);
		processAttri(attri->name(), attri->value());
	
	shape.push_back(tmpPath.release());
}

void Group::processGroup(xml_node<>* node, float& maxWidth, float& maxHeight, vector<shapes*>& shape, defs& def) {
	Group tmpGroup;
	for (const auto& attri : this->attributes) 
		tmpGroup.attributes[attri.first] = attri.second;

	for (xml_attribute<>* attri = node->first_attribute(); attri; attri = attri->next_attribute()) {
		auto it = tmpGroup.attributes.find(attri->name());
		if (it != tmpGroup.attributes.end()) {
			if (attri->name() == "transform") {
				tmpGroup.attributes[attri->name()] = it->second + " " + attri->value();
			}
			else {
				tmpGroup.attributes[attri->name()] = attri->value();
			}
		}
		else {
			tmpGroup.attributes[attri->name()] = attri->value();
		}
	}
	tmpGroup.groupTraversal(node, maxWidth, maxHeight, shape, def);
}

void Group::processDefs(xml_node<>* node, defs& def) {
	for (xml_node<>* child = node->first_node(); child; child = child->next_sibling()) {
		if (child->name() == "linearGradient")
			processLinearGradient(child, def);
		else if (child->name() == "radialGradient")
			processRadialGradient(child, def);
	}
}

void Group::processLinearGradient(xml_node<>* node, defs& def) {
	linearGradient lg;
	for (rapidxml::xml_attribute<>* attri = node->first_attribute(); attri; attri = attri->next_attribute()) 
		readLinearGradient(attri->name(), attri->value(), lg);
	
	for (rapidxml::xml_node<>* attri = node->first_node(); attri; attri = attri->next_sibling()) {
		if (attri->name() == string("stop")) {
			stopPoint newStop;
			for (xml_attribute<>* attribute = attri->first_attribute(); attribute; attribute = attribute->next_attribute()) {
				readStop(attribute->name(), attribute->value(), newStop);
			}
			lg.stopList.push_back(newStop);
		}
	}
	def.lgList.push_back(lg);
}

void Group::processRadialGradient(xml_node<>* node, defs& def) {
	radialGradient rg;
	for (rapidxml::xml_attribute<>* attri = node->first_attribute(); attri; attri = attri->next_attribute()) 
		readRadialGradient(attri->name(), attri->value(), rg);
	
	for (rapidxml::xml_node<>* attri = node->first_node(); attri; attri = attri->next_sibling()) {
		if (attri->name() == string("stop")) {
			stopPoint newStop;
			for (xml_attribute<>* attribute = attri->first_attribute(); attribute; attribute = attribute->next_attribute()) {
				readStop(attribute->name(), attribute->value(), newStop);
			}
			rg.stopList.push_back(newStop);
		}
	}
	def.rgList.push_back(rg);
}


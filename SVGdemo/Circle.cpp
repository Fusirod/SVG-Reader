#include "Shape.h"

void circle::draw(Graphics& gra, List lst) {
	GraphicsState save = gra.Save();
	Pen p(Color(static_cast<int>(strokeOpacity * 255), strokeColor.red, strokeColor.green, strokeColor.blue), static_cast<REAL>(strokeWidth));
	SolidBrush fillBrush(Color(static_cast<int>(fillOpacity * 255), fillColor.red, fillColor.green, fillColor.blue));
	int idx = 0;
	for (int i = 0; i < changes.types.size(); i++) {
		if (changes.types[i] == "translate")
			gra.TranslateTransform(changes.values[idx], changes.values[idx + 1]);
		if (changes.types[i] == "rotate")
			gra.RotateTransform(changes.values[idx]);
		if (changes.types[i] == "scale")
			gra.ScaleTransform(changes.values[idx], changes.values[idx + 1]);
		idx += 2;
		if (changes.types[i] == "rotate")
			idx--;
	}

	if (fillID != "") {
		for (int i = 0; i < lst.linearList.size(); i++) {
			if (fillID == lst.linearList[i].id) {
				float* points = lst.linearList[i].getPointList();
				Color* colors = lst.linearList[i].getColorList();
				LinearGradientBrush linGrBrush(PointF(lst.linearList[i].start.x, lst.linearList[i].start.y),
					PointF(lst.linearList[i].end.x, lst.linearList[i].end.y),
					colors[0], colors[lst.linearList[i].stopList.size() - 1]);
				apply(linGrBrush, lst.linearList[i].changes);
				linGrBrush.SetWrapMode(WrapModeTileFlipXY);
				linGrBrush.SetInterpolationColors(colors, points, lst.linearList[i].stopList.size());
				gra.FillEllipse(&linGrBrush, start.x - r, start.y - r, 2 * r, 2 * r);
				delete[] points;
				delete[] colors;
				break;
			}
		}
	}
	else {
		gra.FillEllipse(&fillBrush, start.x - r, start.y - r, 2 * r, 2 * r);
	}
	if (strokeWidth != 0)
		gra.DrawEllipse(&p, start.x - r, start.y - r, 2 * r, 2 * r);
	gra.Restore(save);
}

void readCircle(string name, string val, circle* cir) {
	if (name == "fill-opacity")
		cir->fillOpacity = stof(val);
	else if (name == "stroke-opacity")
		cir->strokeOpacity = stof(val);
	else if (name == "fill") {
		if (val == "none" || val == "transparent")
			cir->fillOpacity = 0;
		else if (val[0] == 'u' && val[1] == 'r' && val[2] == 'l')
			cir->fillID = val.substr(5, val.length() - 6);
		else
			cir->fillColor = readRGB(val);
	}
	else if (name == "stroke") {
		if (val == "none" || val == "transparent")
			cir->strokeOpacity = 0;
		else if (val[0] == 'u' && val[1] == 'r' && val[2] == 'l')
			cir->strokeID = val.substr(5, val.length() - 6);
		else {
			cir->strokeColor = readRGB(val);

			if (cir->strokeWidth == 0)
				cir->strokeWidth = 1;
		}
	}
	else if (name == "cx")
		cir->start.x = stof(val);
	else if (name == "cy")
		cir->start.y = stof(val);
	else if (name == "r")
		cir->r = stof(val);
	else if (name == "stroke-width")
		cir->strokeWidth = stof(val);
	else if (name == "transform")
		readTransform(val, cir->changes);
	else if (name == "style") {
		istringstream iss(trim(val));
		string tmp;
		while (getline(iss, tmp, ';')) {
			string s1, s2;
			size_t colonPos = tmp.find(':');
			if (colonPos != string::npos) {
				s1 = tmp.substr(0, colonPos);
				s2 = tmp.substr(colonPos + 1);
			}
			readCircle(s1, s2, cir);
		}
	}
}
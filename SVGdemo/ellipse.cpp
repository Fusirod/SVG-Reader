#include "Shape.h"

VOID ellipse::draw(Graphics& gra, List lst) {
	GraphicsState save = gra.Save();
	Pen pen(Color(static_cast<int>(strokeOpacity * 255), strokeColor.red, strokeColor.green, strokeColor.blue), static_cast<REAL>(strokeWidth));
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
				float* p = lst.linearList[i].getPointList();
				Color* co = lst.linearList[i].getColorList();
				LinearGradientBrush linGrBrush(PointF(lst.linearList[i].start.x, lst.linearList[i].start.y), PointF(lst.linearList[i].end.x, lst.linearList[i].end.y), co[0], co[lst.linearList[i].stopList.size() - 1]);
				apply(linGrBrush, lst.linearList[i].changes);
				linGrBrush.SetWrapMode(WrapModeTileFlipXY);
				linGrBrush.SetInterpolationColors(co, p, lst.linearList[i].stopList.size());
				gra.FillEllipse(&linGrBrush, start.x - rx, start.y - ry, 2 * rx, 2 * ry);
				delete[] p;
				delete[] co;
				break;
			}
		}

		for (int i = 0; i < lst.radialList.size(); i++) {
			if (fillID == lst.radialList[i].id) {
				float* p = lst.radialList[i].getPointList();
				Color* co = lst.radialList[i].getColorList();
				LinearGradientBrush linGrBrush(PointF(lst.radialList[i].center.x - lst.radialList[i].r, lst.radialList[i].center.y - lst.radialList[i].r), PointF(lst.radialList[i].center.x + lst.radialList[i].r, lst.radialList[i].center.y + lst.radialList[i].r), co[0], co[lst.radialList[i].stopList.size() - 1]);
				linGrBrush.SetWrapMode(WrapModeTileFlipXY);
				apply(linGrBrush, lst.radialList[i].changes);
				gra.FillEllipse(&linGrBrush, start.x - rx, start.y - ry, 2 * rx, 2 * ry);
				delete[] p;
				delete[] co;
				break;
			}
		}
	}
	else {
		gra.FillEllipse(&fillBrush, start.x - rx, start.y - ry, 2 * rx, 2 * ry);
	}

	if (strokeWidth != 0)
		gra.DrawEllipse(&pen, start.x - rx, start.y - ry, 2 * rx, 2 * ry);
	gra.Restore(save);
}

void readEllipse(string name, string val, ellipse* ell) {
    if (name == "fill-opacity") {
        ell->fillOpacity = stof(val);
    }
    else if (name == "stroke-opacity") {
        ell->strokeOpacity = stof(val);
    }
    else if (name == "fill") {
        if (val == "none" || val == "transparent") {
            ell->fillOpacity = 0;
        }
        else if (val[0] == 'u' && val[1] == 'r' && val[2] == 'l') {
            ell->fillID = val.substr(5, val.length() - 6);
        }
        else
            ell->fillColor = readRGB(val);
    }
    else if (name == "stroke") {
        if (val == "none" || val == "transparent") {
            ell->strokeOpacity = 0;
        }
        else if (val[0] == 'u' && val[1] == 'r' && val[2] == 'l') {
            ell->strokeID = val.substr(5, val.length() - 6);
        }
        else {
            ell->strokeColor = readRGB(val);
            if (ell->strokeWidth == 0)
                ell->strokeWidth = 1;
        }
    }
    else if (name == "cx") {
        ell->start.x = stof(val);
    }
    else if (name == "cy") {
        ell->start.y = stof(val);
    }
    else if (name == "rx") {
        ell->rx = stof(val);
    }
    else if (name == "ry") {
        ell->ry = stof(val);
    }
    else if (name == "stroke-width") {
        ell->strokeWidth = stof(val);
    }
    else if (name == "transform") {
        readTransform(val, ell->changes);
    }

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
            readEllipse(s1, s2, ell);
        }
    }
}
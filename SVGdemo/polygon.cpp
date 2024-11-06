#include "Shape.h"

VOID polygon::draw(Graphics& gra, List lst) {
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

    Point* point = new Point[poly.size()];
    for (int i = 0; i < poly.size(); i++) {
        point[i] = Point(poly[i].x, poly[i].y);
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
                gra.FillPolygon(&linGrBrush, point, static_cast<int>(poly.size()), FillModeWinding);
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
                gra.FillPolygon(&linGrBrush, point, static_cast<int>(poly.size()), FillModeWinding);
                delete[] p;
                delete[] co;
                break;
            }
        }
    }
    else {
        gra.FillPolygon(&fillBrush, point, static_cast<int>(poly.size()), FillModeWinding);
    }
    if (strokeWidth != 0)
        gra.DrawPolygon(&pen, point, static_cast<int>(poly.size()));
    delete[] point;
    gra.Restore(save);
}

VOID polyline::draw(Graphics& gra, List lst) {
    GraphicsState save = gra.Save();
    Pen pen(Color(static_cast<int>(strokeOpacity * 255), strokeColor.red, strokeColor.green, strokeColor.blue), static_cast<REAL>(strokeWidth));
    SolidBrush fillBrush(Color(static_cast<int>(fillOpacity * 255), fillColor.red, fillColor.green, fillColor.blue));
    int index = 0;

    for (int i = 0; i < changes.types.size(); i++) {
        if (changes.types[i] == "translate")
            gra.TranslateTransform(changes.values[index], changes.values[index + 1]);
        if (changes.types[i] == "rotate")
            gra.RotateTransform(changes.values[index]);
        if (changes.types[i] == "scale")
            gra.ScaleTransform(changes.values[index], changes.values[index + 1]);
        index += 2;
        if (changes.types[i] == "rotate")
            index--;
    }
    Point* point = new Point[poly.size()];
    for (int i = 0; i < poly.size(); i++) {
        point[i] = Point(poly[i].x, poly[i].y);
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
                gra.FillPolygon(&linGrBrush, point, static_cast<int>(poly.size()), FillModeWinding);
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
                gra.FillPolygon(&linGrBrush, point, static_cast<int>(poly.size()), FillModeWinding);
                delete[] p;
                delete[] co;
                break;
            }
        }
    }
    else {
        gra.FillPolygon(&fillBrush, point, static_cast<int>(poly.size()), FillModeWinding);
    }
    if (strokeWidth != 0)
        gra.DrawLines(&pen, point, static_cast<int>(poly.size()));
    delete[] point;
    gra.Restore(save);
}


void readPolygon(string name, string val, polygon* gon) {
    if (name == "fill-opacity") {
        gon->fillOpacity = stof(val);
    }
    else if (name == "stroke-opacity") {
        gon->strokeOpacity = stof(val);
    }
    else if (name == "fill") {
        if (val == "none" || val == "transparent") {
            gon->fillOpacity = 0;
        }
        else if (val[0] == 'u' && val[1] == 'r' && val[2] == 'l') {
            gon->fillID = val.substr(5, val.length() - 6);
        }
        else
            gon->fillColor = readRGB(val);
    }
    else if (name == "stroke") {
        if (val == "none" || val == "transparent") {
            gon->strokeOpacity = 0;
        }
        else if (val[0] == 'u' && val[1] == 'r' && val[2] == 'l') {
            gon->strokeID = val.substr(5, val.length() - 6);
        }
        else {
            gon->strokeColor = readRGB(val);
            if (gon->strokeWidth == 0)
                gon->strokeWidth = 1;
        }
    }
    else if (name == "stroke-width") {
        gon->strokeWidth = stof(val);
    }
    else if (name == "points") {
        gon->poly = readPoints(val);
    }
    else if (name == "transform") {
        string s = val;
        readTransform(val, gon->changes);
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
            readPolygon(s1, s2, gon);
        }
    }
}

void readPolyline(string name, string val, polyline* line) {
    if (name == "fill-opacity") {
        line->fillOpacity = stof(val);
    }
    else if (name == "stroke-opacity") {
        line->strokeOpacity = stof(val);
    }
    else if (name == "fill") {
        if (val == "none" || val == "transparent") {
            line->fillOpacity = 0;
        }
        else if (val[0] == 'u' && val[1] == 'r' && val[2] == 'l') {
            line->fillID = val.substr(5, val.length() - 6);
        }
        else
            line->fillColor = readRGB(val);
    }
    else if (name == "stroke") {
        if (val == "none" || val == "transparent") {
            line->strokeOpacity = 0;
        }
        else if (val[0] == 'u' && val[1] == 'r' && val[2] == 'l') {
            line->strokeID = val.substr(5, val.length() - 6);
        }
        else {
            line->strokeColor = readRGB(val);
            if (line->strokeWidth == 0)
                line->strokeWidth = 1;
        }
    }
    else if (name == "stroke-width") {
        line->strokeWidth = stof(val);
    }
    else if (name == "points") {
        line->poly = readPoints(val);
    }
    else if (name == "transform") {
        readTransform(val, line->changes);
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
            readPolyline(s1, s2, line);
        }
    }
}

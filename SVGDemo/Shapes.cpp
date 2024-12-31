#include "stdafx.h"
#include "Shapes.h"

void readLine(string name, string value, line* line) {
    if (name == "stroke-opacity") {
        line->strokeOpacity = stof(value);
    }
    else if (name == "stroke") {
        if (value == "none" || value == "transparent") {
            line->strokeOpacity = 0;
        }
        else if (value[0] == 'u' && value[1] == 'r' && value[2] == 'l') {
            line->strokeId = value.substr(5, value.length() - 6);
        }
        else {
            line->strokeColor = readRGB(value);
            if (line->strokeWidth == 0)
                line->strokeWidth = 1;
        }
    }
    else if (name == "x1") {
        line->start.x = stof(value);
    }
    else if (name == "y1") {
        line->start.y = stof(value);
    }
    else if (name == "x2") {
        line->end.x = stof(value);
    }
    else if (name == "y2") {
        line->end.y = stof(value);
    }
    else if (name == "stroke-width") {
        line->strokeWidth = stof(value);
    }
    else if (name == "transform") {
        readTransform(value, line->trs);
    }
    else if (name == "style") {
        istringstream iss(trim(value));
        string tmp;
        while (getline(iss, tmp, ';')) {
            string str1, str2;
            size_t colonPos = tmp.find(':');
            if (colonPos != string::npos) {
                str1 = tmp.substr(0, colonPos);
                str2 = tmp.substr(colonPos + 1);
            }
            readLine(str1, str2, line);
        }
    }
}

void readRectangle(string name, string value, rectangle* rect) {
    if (name == "fill-opacity") {
        rect->fillOpacity = stof(value);
    }
    else if (name == "stroke-opacity") {
        rect->strokeOpacity = stof(value);
    }
    else if (name == "fill") {
        if (value == "none" || value == "transparent") {
            rect->fillOpacity = 0;
        }
        else if (value.find("url") != string::npos) {
            size_t start = value.find("#");
            size_t end = value.find(")");
            rect->fillId = value.substr(start + 1, end - start - 1);
        }
        else
            rect->fillColor = readRGB(value);
    }
    else if (name == "stroke") {
        if (value == "none" || value == "transparent") {
            rect->strokeOpacity = 0;
        }
        else if (value[0] == 'u' && value[1] == 'r' && value[2] == 'l') {
            rect->strokeId = value.substr(5, value.length() - 6);
        }
        else {
            rect->strokeColor = readRGB(value);
            if (rect->strokeWidth == 0)
                rect->strokeWidth = 1;
        }
    }
    else if (name == "x") {
        rect->start.x = stof(value);
    }
    else if (name == "y") {
        rect->start.y = stof(value);
    }
    else if (name == "width") {
        if (value[value.length() - 1] == 't')
            value = value.substr(0, value.length() - 2);
        rect->width = stof(value);
    }
    else if (name == "height") {
        if (value[value.length() - 1] == 't')
            value = value.substr(0, value.length() - 2);
        rect->height = stof(value);
    }
    else if (name == "stroke-width") {
        rect->strokeWidth = stof(value);
    }
    else if (name == "transform") {
        readTransform(value, rect->trs);
    }
    else if (name == "style") {
        istringstream iss(trim(value));
        string tmp;
        while (getline(iss, tmp, ';')) {
            string str1, str2;
            size_t colonPos = tmp.find(':');
            if (colonPos != string::npos) {
                str1 = tmp.substr(0, colonPos);
                str2 = tmp.substr(colonPos + 1);
            }
            readRectangle(str1, str2, rect);
        }
    }
}

void readEllipse(string name, string value, ellipse* elli) {
    if (name == "fill-opacity") {
        elli->fillOpacity = stof(value);
    }
    else if (name == "stroke-opacity") {
        elli->strokeOpacity = stof(value);
    }
    else if (name == "fill") {
        if (value == "none" || value == "transparent") {
            elli->fillOpacity = 0;
        }
        else if (value[0] == 'u' && value[1] == 'r' && value[2] == 'l') {
            elli->fillId = value.substr(5, value.length() - 6);
        }
        else
            elli->fillColor = readRGB(value);
    }
    else if (name == "stroke") {
        if (value == "none" || value == "transparent") {
            elli->strokeOpacity = 0;
        }
        else if (value[0] == 'u' && value[1] == 'r' && value[2] == 'l') {
            elli->strokeId = value.substr(5, value.length() - 6);
        }
        else {
            elli->strokeColor = readRGB(value);
            if (elli->strokeWidth == 0)
                elli->strokeWidth = 1;
        }
    }
    else if (name == "cx") {
        elli->start.x = stof(value);
    }
    else if (name == "cy") {
        elli->start.y = stof(value);
    }
    else if (name == "rx") {
        elli->rx = stof(value);
    }
    else if (name == "ry") {
        elli->ry = stof(value);
    }
    else if (name == "stroke-width") {
        elli->strokeWidth = stof(value);
    }
    else if (name == "transform") {
        readTransform(value, elli->trs);
    }

    else if (name == "style") {
        istringstream iss(trim(value));
        string tmp;
        while (getline(iss, tmp, ';')) {
            string str1, str2;
            size_t colonPos = tmp.find(':');
            if (colonPos != string::npos) {
                str1 = tmp.substr(0, colonPos);
                str2 = tmp.substr(colonPos + 1);
            }
            readEllipse(str1, str2, elli);
        }
    }
}

void readCircle(string name, string value, circle* cir) {
    if (name == "fill-opacity") {
        cir->fillOpacity = stof(value);
    }
    else if (name == "stroke-opacity") {
        cir->strokeOpacity = stof(value);
    }
    else if (name == "fill") {
        if (value == "none" || value == "transparent") {
            cir->fillOpacity = 0;
        }
        else if (value[0] == 'u' && value[1] == 'r' && value[2] == 'l') {
            cir->fillId = value.substr(5, value.length() - 6);
        }
        else
            cir->fillColor = readRGB(value);
    }
    else if (name == "stroke") {
        if (value == "none" || value == "transparent") {
            cir->strokeOpacity = 0;
        }
        else if (value[0] == 'u' && value[1] == 'r' && value[2] == 'l') {
            cir->strokeId = value.substr(5, value.length() - 6);
        }
        else {
            cir->strokeColor = readRGB(value);
            if (cir->strokeWidth == 0)
                cir->strokeWidth = 1;
        }
    }
    else if (name == "cx") {
        cir->start.x = stof(value);
    }
    else if (name == "cy") {
        cir->start.y = stof(value);
    }
    else if (name == "r") {
        cir->r = stof(value);
    }
    else if (name == "stroke-width") {
        cir->strokeWidth = stof(value);
    }
    else if (name == "transform") {
        readTransform(value, cir->trs);
    }

    else if (name == "style") {
        istringstream iss(trim(value));
        string tmp;
        while (getline(iss, tmp, ';')) {
            string str1, str2;
            size_t colonPos = tmp.find(':');
            if (colonPos != string::npos) {
                str1 = tmp.substr(0, colonPos);
                str2 = tmp.substr(colonPos + 1);
            }
            readCircle(str1, str2, cir);
        }
    }
}

void readPolygon(string name, string value, polygon* polygon) {
    if (name == "fill-opacity") {
        polygon->fillOpacity = stof(value);
    }
    else if (name == "stroke-opacity") {
        polygon->strokeOpacity = stof(value);
    }
    else if (name == "fill") {
        if (value == "none" || value == "transparent") {
            polygon->fillOpacity = 0;
        }
        else if (value[0] == 'u' && value[1] == 'r' && value[2] == 'l') {
            polygon->fillId = value.substr(5, value.length() - 6);
        }
        else
            polygon->fillColor = readRGB(value);
    }
    else if (name == "stroke") {
        if (value == "none" || value == "transparent") {
            polygon->strokeOpacity = 0;
        }
        else if (value[0] == 'u' && value[1] == 'r' && value[2] == 'l') {
            polygon->strokeId = value.substr(5, value.length() - 6);
        }
        else {
            polygon->strokeColor = readRGB(value);
            if (polygon->strokeWidth == 0)
                polygon->strokeWidth = 1;
        }
    }
    else if (name == "stroke-width") {
        polygon->strokeWidth = stof(value);
    }
    else if (name == "points") {
        polygon->points = readPoint(value);
    }
    else if (name == "transform") {
        string s = value;
        readTransform(value, polygon->trs);
    }
    else if (name == "style") {
        istringstream iss(trim(value));
        string tmp;
        while (getline(iss, tmp, ';')) {
            string str1, str2;
            size_t colonPos = tmp.find(':');
            if (colonPos != string::npos) {
                str1 = tmp.substr(0, colonPos);
                str2 = tmp.substr(colonPos + 1);
            }
            readPolygon(str1, str2, polygon);
        }
    }
}

void readPolyline(string name, string value, polyline* polyline) {
    if (name == "fill-opacity") {
        polyline->fillOpacity = stof(value);
    }
    else if (name == "stroke-opacity") {
        polyline->strokeOpacity = stof(value);
    }
    else if (name == "fill") {
        if (value == "none" || value == "transparent") {
            polyline->fillOpacity = 0;
        }
        else if (value[0] == 'u' && value[1] == 'r' && value[2] == 'l') {
            polyline->fillId = value.substr(5, value.length() - 6);
        }
        else
            polyline->fillColor = readRGB(value);
    }
    else if (name == "stroke") {
        if (value == "none" || value == "transparent") {
            polyline->strokeOpacity = 0;
        }
        else if (value[0] == 'u' && value[1] == 'r' && value[2] == 'l') {
            polyline->strokeId = value.substr(5, value.length() - 6);
        }
        else {
            polyline->strokeColor = readRGB(value);
            if (polyline->strokeWidth == 0)
                polyline->strokeWidth = 1;
        }
    }
    else if (name == "stroke-width") {
        polyline->strokeWidth = stof(value);
    }
    else if (name == "points") {
        polyline->points = readPoint(value);
    }
    else if (name == "transform") {
        readTransform(value, polyline->trs);
    }

    else if (name == "style") {
        istringstream iss(trim(value));
        string tmp;
        while (getline(iss, tmp, ';')) {
            string str1, str2;
            size_t colonPos = tmp.find(':');
            if (colonPos != string::npos) {
                str1 = tmp.substr(0, colonPos);
                str2 = tmp.substr(colonPos + 1);
            }
            readPolyline(str1, str2, polyline);
        }
    }
}

void readText(string name, string value, text* text) {
    if (name == "x") {
        text->start.x = stof(value);
    }
    else if (name == "y") {
        text->start.y = stof(value);
    }
    else if (name == "font-size") {
        text->fontSize = stof(value);
    }
    else if (name == "fill") {
        if (value == "none" || value == "transparent") {
            text->fillOpacity = 0;
        }
        else if (value[0] == 'u' && value[1] == 'r' && value[2] == 'l') {
            text->fillId = value.substr(5, value.length() - 6);
        }
        else
            text->fillColor = readRGB(value);
    }
    else if (name == "fill-opacity") {
        text->fillOpacity = stof(value);
    }
    else if (name == "stroke") {
        if (value == "none" || value == "transparent") {
            text->strokeOpacity = 0;
        }
        else if (value[0] == 'u' && value[1] == 'r' && value[2] == 'l') {
            text->strokeId = value.substr(5, value.length() - 6);
        }
        else {
            text->strokeColor = readRGB(value);
            if (text->strokeWidth == 0)
                text->strokeWidth = 1;
        }
    }
    else if (name == "stroke-opacity") {
        text->strokeOpacity = stof(value);
    }
    else if (name == "font-family") {
        text->fontFamily = value;
    }
    else if (name == "transform") {
        readTransform(value, text->trs);
    }
    else if (name == "dx") {
        text->dx = stof(value);
    }
    else if (name == "dy") {
        text->dy = stof(value);
    }
    else if (name == "text-anchor") {
        text->textAnchor = value;
    }

    else if (name == "style") {
        istringstream iss(trim(value));
        string tmp;
        while (getline(iss, tmp, ';')) {
            string str1, str2;
            size_t colonPos = tmp.find(':');
            if (colonPos != string::npos) {
                str1 = tmp.substr(0, colonPos);
                str2 = tmp.substr(colonPos + 1);
            }
            readText(str1, str2, text);
        }
    }
}

void line::draw(Graphics& graphics, defs def) {
    GraphicsState save = graphics.Save();
    Pen pen(Color(static_cast<int>(strokeOpacity * 255), strokeColor.red, strokeColor.green, strokeColor.blue), static_cast<REAL>(strokeWidth));
    int index = 0;
    for (int i = 0; i < trs.type.size(); i++) {
        if (trs.type[i] == "translate")
            graphics.TranslateTransform(trs.value[index], trs.value[index + 1]);
        if (trs.type[i] == "rotate")
            graphics.RotateTransform(trs.value[index]);
        if (trs.type[i] == "scale")
            graphics.ScaleTransform(trs.value[index], trs.value[index + 1]);
        index += 2;
        if (trs.type[i] == "rotate")
            index--;
    }
    graphics.DrawLine(&pen, start.x, start.y, end.x, end.y);
    graphics.Restore(save);
}

void rectangle::draw(Graphics& graphics, defs def) {
    GraphicsState save = graphics.Save();
    Pen pen(Color(static_cast<int>(strokeOpacity * 255), strokeColor.red, strokeColor.green, strokeColor.blue), static_cast<REAL>(strokeWidth));
    SolidBrush fillBrush(Color(static_cast<int>(fillOpacity * 255), fillColor.red, fillColor.green, fillColor.blue));
    int index = 0;
    for (int i = 0; i < trs.type.size(); i++) {
        if (trs.type[i] == "translate")
            graphics.TranslateTransform(trs.value[index], trs.value[index + 1]);
        if (trs.type[i] == "rotate")
            graphics.RotateTransform(trs.value[index]);
        if (trs.type[i] == "scale")
            graphics.ScaleTransform(trs.value[index], trs.value[index + 1]);
        index += 2;
        if (trs.type[i] == "rotate")
            index--;
    }
    if (fillId != "") {
        for (int i = 0; i < def.lgList.size(); i++) {
            if (fillId == def.lgList[i].id) {
                float* points = def.lgList[i].getPointList();
                Color* colors = def.lgList[i].getColorList();
                LinearGradientBrush linGrBrush(PointF(def.lgList[i].start.x, def.lgList[i].start.y), PointF(def.lgList[i].end.x, def.lgList[i].end.y), colors[0], colors[def.lgList[i].stopList.size() - 1]);
                applyTransform(linGrBrush, def.lgList[i].trs);
                linGrBrush.SetWrapMode(WrapModeTileFlipXY);
                linGrBrush.SetInterpolationColors(colors, points, def.lgList[i].stopList.size());
                graphics.FillRectangle(&linGrBrush, start.x, start.y, width, height);
                delete[] points;
                delete[] colors;
                break;
            }
        }
        for (int i = 0; i < def.rgList.size(); i++) {
            if (fillId == def.rgList[i].id) {
                float* points = def.rgList[i].getPointList();
                Color* colors = def.rgList[i].getColorList();
                LinearGradientBrush linGrBrush(PointF(def.rgList[i].center.x - def.rgList[i].r, def.rgList[i].center.y - def.rgList[i].r), PointF(def.rgList[i].center.x + def.rgList[i].r, def.rgList[i].center.y + def.rgList[i].r), colors[0], colors[def.rgList[i].stopList.size() - 1]);
                linGrBrush.SetWrapMode(WrapModeTileFlipXY);
                applyTransform(linGrBrush, def.rgList[i].trs);
                graphics.FillRectangle(&linGrBrush, start.x, start.y, width, height);
                delete[] points;
                delete[] colors;
                break;
            }
        }
    }
    else {
        graphics.FillRectangle(&fillBrush, start.x, start.y, width, height);
    }

    if (strokeWidth != 0)
        graphics.DrawRectangle(&pen, start.x, start.y, width, height);
    graphics.Restore(save);
}

void circle::draw(Graphics& graphics, defs def) {
    GraphicsState save = graphics.Save();
    Pen pen(Color(static_cast<int>(strokeOpacity * 255), strokeColor.red, strokeColor.green, strokeColor.blue), static_cast<REAL>(strokeWidth));
    SolidBrush fillBrush(Color(static_cast<int>(fillOpacity * 255), fillColor.red, fillColor.green, fillColor.blue));
    int index = 0;
    for (int i = 0; i < trs.type.size(); i++) {
        if (trs.type[i] == "translate")
            graphics.TranslateTransform(trs.value[index], trs.value[index + 1]);
        if (trs.type[i] == "rotate")
            graphics.RotateTransform(trs.value[index]);
        if (trs.type[i] == "scale")
            graphics.ScaleTransform(trs.value[index], trs.value[index + 1]);
        index += 2;
        if (trs.type[i] == "rotate")
            index--;
    }
    if (fillId != "") {
        for (int i = 0; i < def.lgList.size(); i++) {
            if (fillId == def.lgList[i].id) {
                float* points = def.lgList[i].getPointList();
                Color* colors = def.lgList[i].getColorList();
                LinearGradientBrush linGrBrush(PointF(def.lgList[i].start.x, def.lgList[i].start.y), PointF(def.lgList[i].end.x, def.lgList[i].end.y), colors[0], colors[def.lgList[i].stopList.size() - 1]);
                applyTransform(linGrBrush, def.lgList[i].trs);
                linGrBrush.SetWrapMode(WrapModeTileFlipXY);
                linGrBrush.SetInterpolationColors(colors, points, def.lgList[i].stopList.size());
                graphics.FillEllipse(&linGrBrush, start.x - r, start.y - r, 2 * r, 2 * r);
                delete[] points;
                delete[] colors;
                break;
            }
        }
        for (int i = 0; i < def.rgList.size(); i++) {
            if (fillId == def.rgList[i].id) {
                float* points = def.rgList[i].getPointList();
                Color* colors = def.rgList[i].getColorList();
                LinearGradientBrush linGrBrush(PointF(def.rgList[i].center.x - def.rgList[i].r, def.rgList[i].center.y - def.rgList[i].r), PointF(def.rgList[i].center.x + def.rgList[i].r, def.rgList[i].center.y + def.rgList[i].r), colors[0], colors[def.rgList[i].stopList.size() - 1]);
                linGrBrush.SetWrapMode(WrapModeTileFlipXY);
                applyTransform(linGrBrush, def.rgList[i].trs);
                graphics.FillEllipse(&linGrBrush, start.x - r, start.y - r, 2 * r, 2 * r);
                delete[] points;
                delete[] colors;
                break;
            }
        }
    }
    else {
        graphics.FillEllipse(&fillBrush, start.x - r, start.y - r, 2 * r, 2 * r);
    }
    
    if (strokeWidth != 0)
        graphics.DrawEllipse(&pen, start.x - r, start.y - r, 2 * r, 2 * r);
    graphics.Restore(save);
}


void ellipse::draw(Graphics& graphics, defs def) {
    GraphicsState save = graphics.Save();
    Pen pen(Color(static_cast<int>(strokeOpacity * 255), strokeColor.red, strokeColor.green, strokeColor.blue), static_cast<REAL>(strokeWidth));
    SolidBrush fillBrush(Color(static_cast<int>(fillOpacity * 255), fillColor.red, fillColor.green, fillColor.blue));
    int index = 0;
    for (int i = 0; i < trs.type.size(); i++) {
        if (trs.type[i] == "translate")
            graphics.TranslateTransform(trs.value[index], trs.value[index + 1]);
        if (trs.type[i] == "rotate")
            graphics.RotateTransform(trs.value[index]);
        if (trs.type[i] == "scale")
            graphics.ScaleTransform(trs.value[index], trs.value[index + 1]);
        index += 2;
        if (trs.type[i] == "rotate")
            index--;
    }
    if (fillId != "") {
        for (int i = 0; i < def.lgList.size(); i++) {
            if (fillId == def.lgList[i].id) {
                float* points = def.lgList[i].getPointList();
                Color* colors = def.lgList[i].getColorList();
                LinearGradientBrush linGrBrush(PointF(def.lgList[i].start.x, def.lgList[i].start.y), PointF(def.lgList[i].end.x, def.lgList[i].end.y), colors[0], colors[def.lgList[i].stopList.size() - 1]);
                applyTransform(linGrBrush, def.lgList[i].trs);
                linGrBrush.SetWrapMode(WrapModeTileFlipXY);
                linGrBrush.SetInterpolationColors(colors, points, def.lgList[i].stopList.size());
                graphics.FillEllipse(&linGrBrush, start.x - rx, start.y - ry, 2 * rx, 2 * ry);
                delete[] points;
                delete[] colors;
                break;
            }
        }
        for (int i = 0; i < def.rgList.size(); i++) {
            if (fillId == def.rgList[i].id) {
                float* points = def.rgList[i].getPointList();
                Color* colors = def.rgList[i].getColorList();
                LinearGradientBrush linGrBrush(PointF(def.rgList[i].center.x - def.rgList[i].r, def.rgList[i].center.y - def.rgList[i].r), PointF(def.rgList[i].center.x + def.rgList[i].r, def.rgList[i].center.y + def.rgList[i].r), colors[0], colors[def.rgList[i].stopList.size() - 1]);
                linGrBrush.SetWrapMode(WrapModeTileFlipXY);
                applyTransform(linGrBrush, def.rgList[i].trs);
                graphics.FillEllipse(&linGrBrush, start.x - rx, start.y - ry, 2 * rx, 2 * ry);
                delete[] points;
                delete[] colors;
                break;
            }
        }
    }
    else {
        graphics.FillEllipse(&fillBrush, start.x - rx, start.y - ry, 2 * rx, 2 * ry);
    }
    
    if (strokeWidth != 0)
        graphics.DrawEllipse(&pen, start.x - rx, start.y - ry, 2 * rx, 2 * ry);
    graphics.Restore(save);
}


void polygon::draw(Graphics& graphics, defs def) {
    GraphicsState save = graphics.Save();
    Pen pen(Color(static_cast<int>(strokeOpacity * 255), strokeColor.red, strokeColor.green, strokeColor.blue), static_cast<REAL>(strokeWidth));
    SolidBrush fillBrush(Color(static_cast<int>(fillOpacity * 255), fillColor.red, fillColor.green, fillColor.blue));
    int index = 0;
    for (int i = 0; i < trs.type.size(); i++) {
        if (trs.type[i] == "translate")
            graphics.TranslateTransform(trs.value[index], trs.value[index + 1]);
        if (trs.type[i] == "rotate")
            graphics.RotateTransform(trs.value[index]);
        if (trs.type[i] == "scale")
            graphics.ScaleTransform(trs.value[index], trs.value[index + 1]);
        index += 2;
        if (trs.type[i] == "rotate")
            index--;
    }
    Point* point = new Point[points.size()];
    for (int i = 0; i < points.size(); i++) {
        point[i] = Point(points[i].x, points[i].y);
    }
    if (fillId != "") {
        for (int i = 0; i < def.lgList.size(); i++) {
            if (fillId == def.lgList[i].id) {
                float* pts = def.lgList[i].getPointList();
                Color* colors = def.lgList[i].getColorList();
                LinearGradientBrush linGrBrush(PointF(def.lgList[i].start.x, def.lgList[i].start.y), PointF(def.lgList[i].end.x, def.lgList[i].end.y), colors[0], colors[def.lgList[i].stopList.size() - 1]);
                applyTransform(linGrBrush, def.lgList[i].trs);
                linGrBrush.SetWrapMode(WrapModeTileFlipXY);
                linGrBrush.SetInterpolationColors(colors, pts, def.lgList[i].stopList.size());
                graphics.FillPolygon(&linGrBrush, point, static_cast<int>(points.size()), FillModeWinding);
                delete[] pts;
                delete[] colors;
                break;
            }
        }
        for (int i = 0; i < def.rgList.size(); i++) {
            if (fillId == def.rgList[i].id) {
                float* pts = def.rgList[i].getPointList();
                Color* colors = def.rgList[i].getColorList();
                LinearGradientBrush linGrBrush(PointF(def.rgList[i].center.x - def.rgList[i].r, def.rgList[i].center.y - def.rgList[i].r), PointF(def.rgList[i].center.x + def.rgList[i].r, def.rgList[i].center.y + def.rgList[i].r), colors[0], colors[def.rgList[i].stopList.size() - 1]);
                linGrBrush.SetWrapMode(WrapModeTileFlipXY);
                applyTransform(linGrBrush, def.rgList[i].trs);
                graphics.FillPolygon(&linGrBrush, point, static_cast<int>(points.size()), FillModeWinding);
                delete[] pts;
                delete[] colors;
                break;
            }
        }
    }
    else {
        graphics.FillPolygon(&fillBrush, point, static_cast<int>(points.size()), FillModeWinding);
    }
    
    if (strokeWidth != 0)
        graphics.DrawPolygon(&pen, point, static_cast<int>(points.size()));
    delete[] point;
    graphics.Restore(save);
}


void polyline::draw(Graphics& graphics, defs def) {
    GraphicsState save = graphics.Save();
    Pen pen(Color(static_cast<int>(strokeOpacity * 255), strokeColor.red, strokeColor.green, strokeColor.blue), static_cast<REAL>(strokeWidth));
    int index = 0;
    for (int i = 0; i < trs.type.size(); i++) {
        if (trs.type[i] == "translate")
            graphics.TranslateTransform(trs.value[index], trs.value[index + 1]);
        if (trs.type[i] == "rotate")
            graphics.RotateTransform(trs.value[index]);
        if (trs.type[i] == "scale")
            graphics.ScaleTransform(trs.value[index], trs.value[index + 1]);
        index += 2;
        if (trs.type[i] == "rotate")
            index--;
    }
    Point* point = new Point[points.size()];
    for (int i = 0; i < points.size(); i++) {
        point[i] = Point(points[i].x, points[i].y);
    }
    SolidBrush fillBrush(Color(static_cast<int>(fillOpacity * 255), fillColor.red, fillColor.green, fillColor.blue));
    if (fillId != "") {
        for (int i = 0; i < def.lgList.size(); i++) {
            if (fillId == def.lgList[i].id) {
                float* pts = def.lgList[i].getPointList();
                Color* colors = def.lgList[i].getColorList();
                LinearGradientBrush linGrBrush(PointF(def.lgList[i].start.x, def.lgList[i].start.y), PointF(def.lgList[i].end.x, def.lgList[i].end.y), colors[0], colors[def.lgList[i].stopList.size() - 1]);
                applyTransform(linGrBrush, def.lgList[i].trs);
                linGrBrush.SetWrapMode(WrapModeTileFlipXY);
                linGrBrush.SetInterpolationColors(colors, pts, def.lgList[i].stopList.size());
                graphics.FillPolygon(&linGrBrush, point, static_cast<int>(points.size()), FillModeWinding);
                delete[] pts;
                delete[] colors;
                break;
            }
        }
        for (int i = 0; i < def.rgList.size(); i++) {
            if (fillId == def.rgList[i].id) {
                float* pts = def.rgList[i].getPointList();
                Color* colors = def.rgList[i].getColorList();
                LinearGradientBrush linGrBrush(PointF(def.rgList[i].center.x - def.rgList[i].r, def.rgList[i].center.y - def.rgList[i].r), PointF(def.rgList[i].center.x + def.rgList[i].r, def.rgList[i].center.y + def.rgList[i].r), colors[0], colors[def.rgList[i].stopList.size() - 1]);
                linGrBrush.SetWrapMode(WrapModeTileFlipXY);
                applyTransform(linGrBrush, def.rgList[i].trs);
                graphics.FillPolygon(&linGrBrush, point, static_cast<int>(points.size()), FillModeWinding);
                delete[] pts;
                delete[] colors;
                break;
            }
        }
    }
    else {
        graphics.FillPolygon(&fillBrush, point, static_cast<int>(points.size()), FillModeWinding);
    }
    
    if (strokeWidth != 0)
        graphics.DrawLines(&pen, point, static_cast<int>(points.size()));
    delete[] point;
    graphics.Restore(save);
}


void text::draw(Graphics& graphics, defs def) {
    SolidBrush fillBrush(Color(static_cast<int>(fillOpacity * 255), fillColor.red, fillColor.green, fillColor.blue));
    Pen pen(Color(static_cast<int>(strokeOpacity * 255), strokeColor.red, strokeColor.green, strokeColor.blue), static_cast<REAL>(strokeWidth));
    
    for (int i = 0; i < content.length(); i++) {
        if (content[i] == '\n') {
            content[i] = ' ';
        }
        if (content[i] == ' ' && content[i + 1] == ' ') {
            content.erase(i, 1);
            i--;
        }
    }
    float rate = 0;
    GraphicsState save = graphics.Save();
    int index = 0;
    for (int i = 0; i < trs.type.size(); i++) {
        if (trs.type[i] == "translate")
            graphics.TranslateTransform(trs.value[index], trs.value[index + 1]);
        if (trs.type[i] == "rotate")
            graphics.RotateTransform(trs.value[index]);
        if (trs.type[i] == "scale")
            graphics.ScaleTransform(trs.value[index], trs.value[index + 1]);
        index += 2;
        if (trs.type[i] == "rotate")
            index--;
    }
    wstring_convert<codecvt_utf8<wchar_t>> converter;

    wstring wFontFamily = converter.from_bytes(fontFamily);

    FontFamily fontFamily(wFontFamily.c_str());
    Font font(&fontFamily, static_cast<REAL>(fontSize), italic ? FontStyleItalic : FontStyleRegular, UnitPixel);

    StringFormat stringFormat; 
    if (textAnchor == "middle") {
        stringFormat.SetAlignment(StringAlignmentCenter); 
    }
    else if (textAnchor == "end") {
        stringFormat.SetAlignment(StringAlignmentFar); 
        rate = 0.15;
    }
    else
    {
        stringFormat.SetAlignment(StringAlignmentNear); 
        rate = -0.15;
    }
    PointF pointF(static_cast<REAL>(start.x + dx + rate * fontSize), static_cast<REAL>(start.y + dy - 0.9 * fontSize));
    GraphicsPath path;

    const wstring wstr = wstring(content.begin(), content.end());
    path.AddString(wstr.c_str(), -1, &fontFamily, italic ? FontStyleItalic : FontStyleRegular, static_cast<REAL>(fontSize), pointF, &stringFormat);

    graphics.FillPath(&fillBrush, &path);
    if (strokeWidth != 0)
        graphics.DrawPath(&pen, &path);
    graphics.Restore(save);
}

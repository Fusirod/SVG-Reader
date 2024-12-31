#include "stdafx.h"
#include "Path.h"

void readPath(const string & name, const string & value, Path * path) {
    if (name == "d") {
        path->data = value;
    }
    else if (name == "stroke") {
        if (value == "none" || value == "transparent") {
            path->strokeOpacity = 0;
        }
        else if (value.substr(0, 4) == "url(" && value.back() == ')') {
            path->strokeId = value.substr(4, value.length() - 5);
        }
        else {
            path->strokeColor = readRGB(value);
            if (path->strokeWidth == 0)
                path->strokeWidth = 1;
        }
    }
    else if (name == "stroke-width") {
        path->strokeWidth = stoi(value);
    }
    else if (name == "stroke-linejoin") {
        path->linejoin = value;
    }
    else if (name == "stroke-linecap") {
        path->linecap = value;
    }
    else if (name == "fill-opacity") {
        path->fillOpacity = stof(value);
    }
    else if (name == "stroke-opacity") {
        path->strokeOpacity = stof(value);
    }
    else if (name == "fill") {
        if (value == "none" || value == "transparent") {
            path->fillOpacity = 0;
        }
        else if (value.substr(0, 4) == "url(" && value.back() == ')') {
            path->strokeId = value.substr(4, value.length() - 5);
        }
        else
            path->fillColor = readRGB(value);
    }
    else if (name == "transform") {
        readTransform(value, path->trs);
    }
    else if (name == "style") {
        istringstream iss(value);
        string tmp;
        while (getline(iss, tmp, ';')) {
            //string str1, str2;
            size_t colonPos = tmp.find(':');
            if (colonPos != string::npos) {
                string key = trim(tmp.substr(0, colonPos));
                string val = trim(tmp.substr(colonPos + 1));
                readPath(key, val, path);
            }
        }
    }
    else {
        cout << "Unknown attribute: " << name << " with value: " << value << endl;
    }
}

void Path::readSinglePointFirst(const string& data, int& idx, point& p) {
    try {
        bool negativeX = false, negativeY = false;

        // Find first element for x
        while (idx < data.size() && !isdigit(data[idx]) && data[idx] != '-' && data[idx] != '.') {
            idx++;
        }

        // Check sign for x
        if (idx < data.size() && data[idx] == '-') {
            negativeX = true;
            idx++;
        }

        // Export value of x
        size_t endPos;
        p.x = stof(data.substr(idx), &endPos);  // Convert to interger
        idx += endPos;  // Update idx
        if (negativeX)
            p.x *= -1;  // Solution if x is negative

        // Find next element for y
        while (idx < data.size() && !isdigit(data[idx]) && data[idx] != '-' && data[idx] != '.') {
            idx++;
        }

        // Check sign for y
        if (idx < data.size() && data[idx] == '-') {
            negativeY = true;
            idx++;
        }

        // Export value of y
        p.y = stof(data.substr(idx), &endPos);  // Convert to interger
        idx += endPos;  // Update idx
        if (negativeY)
            p.y *= -1;  // Solution if y is negative
    }
    catch (const exception& e) {
        // Solution if can't convert string into interger
        cout << "Error parsing point at idx " << idx << ": " << e.what() << endl;
        p.x = p.y = 0;
    }
}

float Path::readSinglePointSecond(const string& data, int& idx) {
    try {
        while (idx < data.size() && !isdigit(data[idx]) && data[idx] != '-' && data[idx] != '.') {
            idx++;
        }
        if (idx >= data.size()) {
            cout << "Error: No valid number found at idx: " << idx << endl;
            return 0.0f;
        }

        bool isNegative = (data[idx] == '-');
        if (isNegative)
            idx++;

        // Extract real part
        size_t endPos;
        float result = stof(data.substr(idx), &endPos);
        idx += endPos;
        return isNegative ? -result : result;
    }
    catch (const exception& e) {
        // Solution if can't convert string into interger
        cout << "Error parsing float: " << e.what() << endl;
        return 0.0f;
    }
}


void Path::draw(Graphics& graphic, defs def) {
    GraphicsState save = graphic.Save();
    SolidBrush fillBrush(Color(static_cast<int>(fillOpacity * 255), fillColor.red, fillColor.green, fillColor.blue));
    Pen pen(Color(static_cast<int>(strokeOpacity * 255), strokeColor.red, strokeColor.green, strokeColor.blue), static_cast<REAL>(strokeWidth));

    int idx = 0;
    for (int i = 0; i < trs.type.size(); i++) {
        if (trs.type[i] == "translate")
            graphic.TranslateTransform(trs.value[idx], trs.value[idx + 1]);
        if (trs.type[i] == "rotate")
            graphic.RotateTransform(trs.value[idx]);
        if (trs.type[i] == "scale")
            graphic.ScaleTransform(trs.value[idx], trs.value[idx + 1]);
        idx += 2;
        if (trs.type[i] == "rotate")
            idx--;
    }
    GraphicsPath path;
    point currentPoint;
    point startPoint, lastControlPoint;
    point r, d, d1, d2, d3;
    bool firstPoint = true;
    idx = 0;
    //float singleType;
    char lastCommand = ' ';

    while (idx < data.length()) {
        if (data[idx] == 'Z' || data[idx] == 'z') {
            idx++;
            handleClosePath(path, currentPoint, startPoint, lastCommand);
        }
        else if (data[idx] == 'A' || data[idx] == 'a') {
            handleArcTo(path, data, idx, currentPoint, data[idx]);
            lastCommand = data[idx];
        }
        else if (data[idx] == 'Q' || data[idx] == 'q') {
            handleQuadraticBezier(data, idx, data[idx], path, currentPoint, lastCommand);
        }
        else if (data[idx] == 'S' || data[idx] == 's') {
            handleSmoothCubicBezier(data, idx, data[idx], path, currentPoint, d2, lastCommand);
        }
        else if (data[idx] == 'M' || data[idx] == 'm') {
            handleMoveTo(data, idx, data[idx], path, currentPoint, startPoint, firstPoint, lastCommand);
        }
        else if (data[idx] == 'L' || data[idx] == 'l') {
            handleLineTo(data, idx, data[idx], path, currentPoint, lastCommand);
        }
        else if (data[idx] == 'h' || data[idx] == 'H') {
            handleHorizontalLineTo(data, idx, data[idx], path, currentPoint, lastCommand);
        }
        else if (data[idx] == 'V' || data[idx] == 'v') {
            handleVerticalLineTo(data, idx, data[idx], path, currentPoint, lastCommand);
        }
        else if (data[idx] == 'C' || data[idx] == 'c') {
            handleCubicBezier(data, idx, data[idx], path, currentPoint, d2, lastCommand);
        }
        else if (data[idx] == 't' || data[idx] == 'T') {
            handleSmoothQuadraticBezier(data, idx, data[idx], path, currentPoint, lastControlPoint, lastCommand);
        }
        else if (data[idx] <= '9' && data[idx] >= '0' || data[idx] == '.') {
            switch (lastCommand) {
            case 'm':
            case 'M':
                handleMoveTo(data, idx, lastCommand, path, currentPoint, startPoint, firstPoint, lastCommand);
                break;
            case 'l':
            case 'L':
                handleLineTo(data, idx, lastCommand, path, currentPoint, lastCommand);
                break;
            case 'h':
            case 'H':
                handleHorizontalLineTo(data, idx, lastCommand, path, currentPoint, lastCommand);
                break;
            case 'v':
            case 'V':
                handleVerticalLineTo(data, idx, lastCommand, path, currentPoint, lastCommand);
                break;
            case 'c':
            case 'C':
                handleCubicBezier(data, idx, lastCommand, path, currentPoint, d2, lastCommand);
                break;
            case 's':
            case 'S':
                handleSmoothCubicBezier(data, idx, lastCommand, path, currentPoint, d2, lastCommand);
                break;
            case 'q':
            case 'Q':
                handleQuadraticBezier(data, idx, lastCommand, path, currentPoint, lastCommand);
                break;
            case 't':
            case 'T':
                handleSmoothQuadraticBezier(data, idx, lastCommand, path, currentPoint, d1, lastCommand);
                break;
            case 'a':
            case 'A':
                handleArcTo(path, data, idx, currentPoint, lastCommand);
                break;
            case 'z':
            case 'Z':
                handleClosePath(path, currentPoint, startPoint, lastCommand);
                break;
            }
        }
        else {
            idx++;
        }
    }

    configurePen(pen, linecap, linejoin);
    graphic.FillPath(&fillBrush, &path);

    if (fillId != "") {
        for (int i = 0; i < def.lgList.size(); i++) {
            if (fillId == def.lgList[i].id) {
                float* points = def.lgList[i].getPointList();
                Color* colors = def.lgList[i].getColorList();
                LinearGradientBrush linGrBrush(PointF(def.lgList[i].start.x, def.lgList[i].start.y), PointF(def.lgList[i].end.x, def.lgList[i].end.y), colors[0], colors[def.lgList[i].stopList.size() - 1]);
                applyTransform(linGrBrush, def.lgList[i].trs);
                linGrBrush.SetWrapMode(WrapModeTileFlipXY);
                linGrBrush.SetInterpolationColors(colors, points, def.lgList[i].stopList.size());
                graphic.FillPath(&linGrBrush, &path);
                delete[] points;
                delete[] colors;
                goto Draw;
                break;
            }
        }
        
        for (int i = 0; i < def.rgList.size(); i++) {
            if (fillId == def.rgList[i].id) {
                float* points = def.rgList[i].getPointList();
                Color* colors = def.rgList[i].getColorList();
                LinearGradientBrush linGrBrush(PointF(def.rgList[i].center.x - def.rgList[i].r, def.rgList[i].center.y - def.rgList[i].r), PointF(def.rgList[i].center.x + def.rgList[i].r, def.rgList[i].center.y + def.rgList[i].r), colors[0], colors[def.rgList[i].stopList.size() - 1]);
                linGrBrush.SetWrapMode(WrapModeTileFlipXY);
                linGrBrush.SetInterpolationColors(colors, points, def.rgList[i].stopList.size());
                applyTransform(linGrBrush, def.rgList[i].trs);
                graphic.FillPath(&linGrBrush, &path);
                delete[] points;
                delete[] colors;
                break;
            }
        }
    }
    else {
        graphic.FillPath(&fillBrush, &path);
    }
Draw:
    if (strokeWidth != 0)
        graphic.DrawPath(&pen, &path);
    graphic.Restore(save);
}

void Path::handleClosePath(GraphicsPath& path, point& currentPoint, point& startPoint, char& lastCommand) {
    path.CloseFigure();  // Close path
    startPoint.x = currentPoint.x;  // Update new starting point
    startPoint.y = currentPoint.y;
    // Save last command
    lastCommand = (lastCommand == 'z' || lastCommand == 'Z') ? 'Z' : 'z';
}

void Path::handleArcTo(GraphicsPath& path, const string& data, int& idx, point& currentPoint, char command) {
    // Read data
    point r, d, d1, d2, d3;
    readSinglePointFirst(data, idx, r);
    float rotation = readSinglePointSecond(data, ++idx);
    float largeArc = readSinglePointSecond(data, ++idx);
    float sweep = readSinglePointSecond(data, ++idx);
    readSinglePointFirst(data, ++idx, d);
    if (command == 'a') {
        d.x += currentPoint.x;
        d.y += currentPoint.y;
    }

    float angle = rotation * M_PI / 180.0f;
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);

    float cons = cosAngle * cosAngle - (sinAngle * -sinAngle);
    d1.x = cons * (currentPoint.x - d.x) / 2.0f;
    d1.y = cons * (currentPoint.y - d.y) / 2.0f;

    float checkR = (d1.x * d1.x) / (r.x * r.x) + (d1.y * d1.y) / (r.y * r.y);
    if (checkR > 1.0f) {
        r.x *= sqrt(checkR);
        r.y *= sqrt(checkR);
    }

    int sign = (largeArc == sweep) ? -1 : 1;
    float num = abs(r.x * r.x * r.y * r.y - r.x * r.x * d1.y * d1.y - r.y * r.y * d1.x * d1.x);
    float den = r.x * r.x * d1.y * d1.y + r.y * r.y * d1.x * d1.x;
    d2.x = sign * sqrt(num / den) * r.x * d1.y / r.y;
    d2.y = sign * sqrt(num / den) * -r.y * d1.x / r.x;

    cons = cosAngle * cosAngle - (sinAngle * sinAngle);
    d3.x = cons * d2.x + (currentPoint.x + d.x) / 2.0f;
    d3.y = cons * d2.y + (currentPoint.y + d.y) / 2.0f;

    float angle1 = atan2((d1.y - d2.y) / r.y, (d1.x - d2.x) / r.x);
    float angle2 = atan2((-d2.y - d1.y) / r.y, (-d2.x - d1.x) / r.x);
    float deltaAngle = angle2 - angle1;

    if (sweep == 0 && deltaAngle > 0) {
        deltaAngle -= 2 * M_PI;
    }
    else if (sweep == 1 && deltaAngle < 0) {
        deltaAngle += 2 * M_PI;
    }

    path.AddArc(d3.x - r.x, d3.y - r.y, 2 * r.x, 2 * r.y, fmod(angle1 * 180.0f / M_PI, 360), fmod(deltaAngle * 180.0f / M_PI, 360));

    // Update new co-ordinates after calculating
    currentPoint.x = d.x;
    currentPoint.y = d.y;
}

void Path::handleQuadraticBezier(const string& data, int& idx, char command, GraphicsPath& path, point& currentPoint, char& lastCommand) {
    point d1, d;

    // Read point
    readSinglePointFirst(data, idx, d1);
    readSinglePointFirst(data, idx, d);

    if (command == 'q') {
        //path.AddBezier(currentPoint.x, currentPoint.y, currentPoint.x + d1.x, currentPoint.y + d1.y, currentPoint.x + d1.x, currentPoint.y + d1.y, currentPoint.x + d.x, currentPoint.y + d.y);
        path.AddBezier(currentPoint.x, currentPoint.y,
            currentPoint.x + d1.x, currentPoint.y + d1.y,  // First control point
            currentPoint.x + d1.x, currentPoint.y + d1.y,  // Second control point
            currentPoint.x + d.x, currentPoint.y + d.y);   // Final point

        // Update co-ordinates
        currentPoint.x += d.x;
        currentPoint.y += d.y;
    }
    else if (command == 'Q') {
        //path.AddBezier(currentPoint.x, currentPoint.y, d1.x, d1.y, d1.x, d1.y, d.x, d.y);

        path.AddBezier(currentPoint.x, currentPoint.y,
            d1.x, d1.y,  // First control point
            d1.x, d1.y,  // Second control point
            d.x, d.y);   // Final point

        // Update co-ordinates
        currentPoint.x = d.x;
        currentPoint.y = d.y;
    }

    lastCommand = command;
}

void Path::handleSmoothCubicBezier(const string& data, int& idx, char command, GraphicsPath& path, point& currentPoint, point& d2, char& lastCommand) {
    point d1, d3, d;

    readSinglePointFirst(data, idx, d3);
    readSinglePointFirst(data, idx, d);

    if (lastCommand == 'c' || lastCommand == 'C' || lastCommand == 's' || lastCommand == 'S') {
        d1.x = 2 * currentPoint.x - d2.x;
        d1.y = 2 * currentPoint.y - d2.y;
    }
    else {
        d1.x = currentPoint.x;
        d1.y = currentPoint.y;
    }

    if (command == 's') {
        //path.AddBezier(currentPoint.x, currentPoint.y, d1.x, d1.y, currentPoint.x + d3.x, currentPoint.y + d3.y, currentPoint.x + d.x, currentPoint.y + d.y);

        path.AddBezier(currentPoint.x, currentPoint.y,
            currentPoint.x + d1.x, currentPoint.y + d1.y,
            currentPoint.x + d3.x, currentPoint.y + d3.y,
            currentPoint.x + d.x, currentPoint.y + d.y);
        currentPoint.x += d.x;
        currentPoint.y += d.y;
        d2.x = currentPoint.x + d3.x;
        d2.y = currentPoint.y + d3.y;
    }
    else if (command == 'S') {

        //path.AddBezier(currentPoint.x, currentPoint.y, d1.x, d1.y, d3.x, d3.y, d.x, d.y);

        path.AddBezier(currentPoint.x, currentPoint.y,
            d1.x, d1.y,
            d3.x, d3.y,
            d.x, d.y);


        currentPoint.x = d.x;
        currentPoint.y = d.y;
        d2.x = d3.x;
        d2.y = d3.y;
    }

    lastCommand = command;
}

void Path::handleMoveTo(const string& data, int& idx, char command, GraphicsPath& path, point& currentPoint, point& startPoint, bool& firstPoint, char& lastCommand) {
    point d;
    readSinglePointFirst(data, idx, d);

    if (firstPoint) {
        firstPoint = false;
        startPoint.x = (command == 'm') ? d.x + currentPoint.x : d.x;
        startPoint.y = (command == 'm') ? d.y + currentPoint.y : d.y;
        currentPoint = startPoint;
        path.StartFigure();
    }
    else {
        currentPoint.x = (command == 'm') ? currentPoint.x + d.x : d.x;
        currentPoint.y = (command == 'm') ? currentPoint.y + d.y : d.y;
    }

    lastCommand = command;
}

void Path::handleLineTo(const string& data, int& idx, char command, GraphicsPath& path, point& currentPoint, char& lastCommand) {
    point d;
    readSinglePointFirst(data, idx, d);

    if (command == 'l') {
        path.AddLine(currentPoint.x, currentPoint.y, currentPoint.x + d.x, currentPoint.y + d.y);
        currentPoint.x += d.x;
        currentPoint.y += d.y;
    }
    else if (command == 'L') {
        path.AddLine(currentPoint.x, currentPoint.y, d.x, d.y);
        currentPoint = d;
    }

    lastCommand = command;
}

void Path::handleHorizontalLineTo(const string& data, int& idx, char command, GraphicsPath& path, point& currentPoint, char& lastCommand) {
    float singleType = readSinglePointSecond(data, idx);

    if (command == 'h') {
        path.AddLine(currentPoint.x, currentPoint.y, currentPoint.x + singleType, currentPoint.y);
        currentPoint.x += singleType;
    }
    else if (command == 'H') {
        path.AddLine(currentPoint.x, currentPoint.y, singleType, currentPoint.y);
        currentPoint.x = singleType;
    }

    lastCommand = command;
}

void Path::handleVerticalLineTo(const string& data, int& idx, char command, GraphicsPath& path, point& currentPoint, char& lastCommand) {
    float singleType = readSinglePointSecond(data, idx);

    if (command == 'v') {
        path.AddLine(currentPoint.x, currentPoint.y, currentPoint.x, currentPoint.y + singleType);
        currentPoint.y += singleType;
    }
    else if (command == 'V') {
        path.AddLine(currentPoint.x, currentPoint.y, currentPoint.x, singleType);
        currentPoint.y = singleType;
    }

    lastCommand = command;
}

void Path::handleCubicBezier(const string& data, int& idx, char command, GraphicsPath& path, point& currentPoint, point& d2, char& lastCommand) {
    point d1, control2, d;

    readSinglePointFirst(data, idx, d1);
    readSinglePointFirst(data, idx, control2);
    readSinglePointFirst(data, idx, d);

    // Calculate those point
    point p1 = (command == 'c') ? point{ currentPoint.x + d1.x, currentPoint.y + d1.y } : d1;
    point p2 = (command == 'c') ? point{ currentPoint.x + control2.x, currentPoint.y + control2.y } : control2;
    point p3 = (command == 'c') ? point{ currentPoint.x + d.x, currentPoint.y + d.y } : d;

    // Add Bezier into path
    path.AddBezier(currentPoint.x, currentPoint.y, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);

    // Upadate last control point
    d2.x = p2.x;
    d2.y = p2.y;

    // Update currentPoint
    if (command == 'c') {
        currentPoint.x += d.x;
        currentPoint.y += d.y;
    }
    else if (command == 'C') {
        currentPoint = d;
    }

    lastCommand = command;
}


void Path::handleSmoothQuadraticBezier(const string& data, int& idx, char command, GraphicsPath& path, point& currentPoint, point& lastControlPoint, char& lastCommand) {
    point d, controlPoint;

    // Calculate control point base on last command
    if (lastCommand == 'Q' || lastCommand == 'T') {
        controlPoint.x = 2 * currentPoint.x - lastControlPoint.x;
        controlPoint.y = 2 * currentPoint.y - lastControlPoint.y;
    }
    else {
        // If there is no command before, control point is set to current point
        controlPoint.x = currentPoint.x;
        controlPoint.y = currentPoint.y;
    }

    readSinglePointFirst(data, idx, d);

    if (command == 't' || command == 'T') {
        float endX = (command == 't') ? currentPoint.x + d.x : d.x;
        float endY = (command == 't') ? currentPoint.y + d.y : d.y;

        path.AddBezier(
            currentPoint.x, currentPoint.y,
            controlPoint.x, controlPoint.y,
            controlPoint.x, controlPoint.y,
            endX, endY
        );

        currentPoint.x = endX;
        currentPoint.y = endY;
    }

    // Update status
    lastControlPoint = controlPoint; // Save last point
    lastCommand = command; // SAve last command
}

void Path::configurePen(Pen& pen, const string& linecap, const string& linejoin) {
    // LineCap
    if (linecap == "round") {
        pen.SetStartCap(LineCapRound);
        pen.SetEndCap(LineCapRound);
    }
    else if (linecap == "square") {
        pen.SetStartCap(LineCapSquare);
        pen.SetEndCap(LineCapSquare);
    }
    else if (linecap == "flat") {
        pen.SetStartCap(LineCapFlat);
        pen.SetEndCap(LineCapFlat);
    }
    else {
        // Warning if value is invalid
        cout << "Warning: Invalid linecap value \"" << linecap << "\". Using default (flat) instead." << endl;
        pen.SetStartCap(LineCapFlat);
        pen.SetEndCap(LineCapFlat);
    }
    // LineJoin
    if (linejoin == "round") {
        pen.SetLineJoin(LineJoinRound);
    }
    else if (linejoin == "bevel") {
        pen.SetLineJoin(LineJoinBevel);
    }
    else if (linejoin == "miter") {
        pen.SetLineJoin(LineJoinMiter);
    }
    else {
        // Warning if value is invalid
        cout << "Warning: Invalid linejoin value \"" << linejoin << "\". Using default (miter) instead." << endl;
        pen.SetLineJoin(LineJoinMiter);
    }
}





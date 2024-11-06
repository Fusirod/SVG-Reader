#include "Path.h"

void readPath(string name, string val, Path* path) {
    if (name == "d") {
        path->data = val;
    }
    else if (name == "stroke") {
        if (val == "none" || val == "transparent") {
            path->strokeOpacity = 0;
        }
        else if (val[0] == 'u' && val[1] == 'r' && val[2] == 'l') {
            path->strokeID = val.substr(5, val.length() - 6);
        }
        else {
            path->strokeColor = readRGB(val);
            if (path->strokeWidth == 0)
                path->strokeWidth = 1;
        }
    }
    else if (name == "stroke-width") {
        path->strokeWidth = stoi(val);
    }
    else if (name == "stroke-linejoin") {
        path->linejoin = val;
    }
    else if (name == "stroke-linecap") {
        path->linecap = val;
    }
    else if (name == "fill-opacity") {
        path->fillOpacity = stof(val);
    }
    else if (name == "stroke-opacity") {
        path->strokeOpacity = stof(val);
    }
    else if (name == "fill") {
        if (val == "none" || val == "transparent") {
            path->fillOpacity = 0;
        }
        else if (val[0] == 'u' && val[1] == 'r' && val[2] == 'l') {
            path->fillID = val.substr(5, val.length() - 6);
        }
        else
            path->fillColor = readRGB(val);
    }
    else if (name == "transform") {
        readTransform(val, path->changes);
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
            readPath(s1, s2, path);
        }
    }
}

void Path::readSinglePointFirst(string data, int& idx, point& p) {
    string n1 = "0", n2 = "0";
    bool s1 = false, s2 = false, accept = false;
    bool negative1 = false, negative2 = false;
    while (true) {
        //check if the idx is out of range
        if ((data[idx] > '9' || data[idx] < '0') && data[idx] != '.' && data[idx] != 'e') {
            if (s1 == true) {
                s1 = false;
                accept = true;
            }
            if (s2 == true) {
                p.x = stof(n1);
                p.y = stof(n2);
                if (negative1)
                    p.x *= -1;
                if (negative2)
                    p.y *= -1;
                return;
            }
        }
        else if (data[idx] <= '9' && data[idx] >= '0' && s1 == false && s2 == false && accept == false) {
            n1 += data[idx];
            if (data[idx - 1] == '-') {
                negative1 = true;
            }
            s1 = true;
            idx++;
            continue;
        }
        else if (data[idx] == '.' && s1 == false && s2 == false && accept == false) {
            n1 += data[idx];
            if (data[idx - 1] == '-') {
                negative1 = true;
            }
            s1 = true;
            idx++;
            continue;
        }
        else if (data[idx] <= '9' && data[idx] >= '0' && s1 == false && accept == true) {
            n2 += data[idx];
            s2 = true;
            if (data[idx - 1] == '-') {
                negative2 = true;
            }
            idx++;
            continue;
        }
        else if (data[idx] == '.' && s1 == true && n1.find('.') != string::npos) {
            n2 += data[idx];
            s2 = true;
            if (data[idx - 1] == '-') {
                negative2 = true;
            }
            idx++;
            s1 = false;
            accept = true;
            continue;
        }
        else if (data[idx] == '.' && s1 == false && accept == true && s2 == false) {
            n2 += data[idx];
            s2 = true;
            if (data[idx - 1] == '-') {
                negative2 = true;
            }
            idx++;
            s1 = false;
            accept = true;
            continue;
        }
        if (data[idx] <= '9' && data[idx] >= '0' || data[idx] == '.' || data[idx] == 'e') {
            if (s1) {
                if (data[idx] == 'e') {
                    n1 += data[idx];
                    n1 += data[idx + 1];
                    idx += 2;
                    continue;
                }
                n1 += data[idx];
            }
            if (s2) {
                if (data[idx] == 'e') {
                    n2 += data[idx];
                    n2 += data[idx + 1];
                    idx += 2;
                    continue;
                }
                if (data[idx] == '.' && n2.find('.') != string::npos) {
                    p.x = stof(n1);
                    p.y = stof(n2);
                    if (negative1)
                        p.x *= -1;
                    if (negative2)
                        p.y *= -1;
                    return;
                }
                else
                    n2 += data[idx];
            }
        }
        idx++;
    }
}

float Path::readSinglePointSecond(string data, int& idx) {
    string n = "0";
    bool s = false;
    bool negative = false;
    while (true) {
        if (data[idx] == '-' && data[idx + 1] <= '9' && data[idx + 1] >= '0' && n == "0") {
            negative = true;
        }
        if ((data[idx] > '9' || data[idx] < '0') && data[idx] != '.' && data[idx] != 'e') {
            if (s == true) {
                float result = stof(n);
                if (negative)
                    result *= -1;
                return  result;
            }
        }
        else if (data[idx] <= '9' && data[idx] >= '0' && s == false) {
            n += data[idx];
            s = true;
            if (data[idx - 1] == '-') {
                negative = true;
            }
            idx++;
            continue;
        }
        else if (data[idx] == '.' && s == false) {
            n += data[idx];
            if (data[idx - 1] == '-') {
                negative = true;
            }
            s = true;
            idx++;
            continue;
        }
        if (data[idx] <= '9' && data[idx] >= '0' || data[idx] == '.' || data[idx] == 'e') {
            if (data[idx] == 'e') {
                n += data[idx];
                n += data[idx + 1];
                idx += 2;
                continue;
            }
            n += data[idx];
        }
        idx++;
    }
}

void Path::draw(Graphics& gra, List lst) {
    GraphicsState save = gra.Save();
    SolidBrush fillBrush(Color(static_cast<int>(fillOpacity * 255), fillColor.red, fillColor.green, fillColor.blue));
    Pen pen(Color(static_cast<int>(strokeOpacity * 255), strokeColor.red, strokeColor.green, strokeColor.blue), static_cast<REAL>(strokeWidth));
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
    GraphicsPath path;
    point curPoint;
    point startPoint;
    point d, d1, d2, d3, r;
    bool firstPoint = true;
    idx = 0;
    float singleType;
    char lastCommand = ' ';
    while (idx < data.length()) {
        if (data[idx] == 'z' || data[idx] == 'Z') {
            idx++;
            path.CloseFigure();
            startPoint.x = curPoint.x;
            startPoint.y = curPoint.y;
            lastCommand = 'z';
        }
        else if (data[idx] == 'A') {
        command_A:
            readSinglePointFirst(data, idx, r);
            float rotation = readSinglePointSecond(data, ++idx);
            float large_arc = readSinglePointSecond(data, ++idx);
            float sweep = readSinglePointSecond(data, ++idx);
            readSinglePointFirst(data, ++idx, d);
            float angle = rotation * M_PI / 180.0f; //M_PI 3.14159265358979323846
            float cos_angle = cos(angle);
            float sin_angle = sin(angle);
            float cons = cos_angle * cos_angle - (sin_angle * -sin_angle);
            d1.x = cons * (curPoint.x - d.x) / 2.0f;
            d1.y = cons * (curPoint.y - d.y) / 2.0f;
            float check_r = (d1.x * d1.x) / (r.x * r.x) + (d1.y * d1.y) / (r.y * r.y);
            if (check_r > 1.0f) {
                r.x *= sqrt(check_r);
                r.y *= sqrt(check_r);
            }
            int sign = (large_arc == sweep) ? -1 : 1;
            float num = abs(r.x * r.x * r.y * r.y - r.x * r.x * d1.y * d1.y - r.y * r.y * d1.x * d1.x);
            float den = r.x * r.x * d1.y * d1.y + r.y * r.y * d1.x * d1.x;
            d2.x = sign * sqrt(num / den) * r.x * d1.y / r.y;
            d2.y = sign * sqrt(num / den) * -r.y * d1.x / r.x;
            cons = cos_angle * cos_angle - (sin_angle * -sin_angle);
            d3.x = cons * d2.x + (curPoint.x + d.x) / 2.0f;
            d3.y = cons * d2.y + (curPoint.y + d.y) / 2.0f;
            float angle1 = atan2((d1.y - d2.y) / r.y, (d1.x - d2.x) / r.x);
            float angle2 = atan2((-d2.y - d1.y) / r.y, (-d2.x - d1.x) / r.x);
            float delta_angle = angle2 - angle1;
            if (sweep == 0 && delta_angle > 0) {
                delta_angle -= 2 * M_PI; //M_PI
            }
            else if (sweep == 1 && delta_angle < 0) {
                delta_angle += 2 * M_PI; //M_PI
            }
            path.AddArc(d3.x - r.x, d3.y - r.y, 2 * r.x, 2 * r.y, fmod((long double)(angle1 * 180.0f) / M_PI, 360), fmod((long double)(delta_angle * 180.0f) / M_PI, 360));
            curPoint.x = d.x;
            curPoint.y = d.y;
            lastCommand = 'A';
        }
        else if (data[idx] == 'a') {
        command_a:
            readSinglePointFirst(data, idx, r);
            float rotation = readSinglePointSecond(data, ++idx);
            float large_arc = readSinglePointSecond(data, ++idx);
            float sweep = readSinglePointSecond(data, ++idx);
            readSinglePointFirst(data, ++idx, d);
            d.x += curPoint.x;
            d.y += curPoint.y;
            float angle = rotation * M_PI / 180.0f;
            float cos_angle = cos(angle);
            float sin_angle = sin(angle);
            float cons = cos_angle * cos_angle - (sin_angle * -sin_angle);
            d1.x = cons * (curPoint.x - d.x) / 2.0f;
            d1.y = cons * (curPoint.y - d.y) / 2.0f;
            float check_r = (d1.x * d1.x) / (r.x * r.x) + (d1.y * d1.y) / (r.y * r.y);
            if (check_r > 1.0f) {
                r.x *= sqrt(check_r);
                r.y *= sqrt(check_r);
            }
            int sign = (large_arc == sweep) ? -1 : 1;
            float num = abs(r.x * r.x * r.y * r.y - r.x * r.x * d1.y * d1.y - r.y * r.y * d1.x * d1.x);
            float den = r.x * r.x * d1.y * d1.y + r.y * r.y * d1.x * d1.x;
            d2.x = sign * sqrt(num / den) * r.x * d1.y / r.y;
            d2.y = sign * sqrt(num / den) * -r.y * d1.x / r.x;
            cons = cos_angle * cos_angle - (sin_angle * -sin_angle);
            d3.x = cons * d2.x + (curPoint.x + d.x) / 2.0f;
            d3.y = cons * d2.y + (curPoint.y + d.y) / 2.0f;
            float angle1 = atan2((d1.y - d2.y) / r.y, (d1.x - d2.x) / r.x);
            float angle2 = atan2((-d2.y - d1.y) / r.y, (-d2.x - d1.x) / r.x);
            float delta_angle = angle2 - angle1;
            if (sweep == 0 && delta_angle > 0) {
                delta_angle -= 2 * M_PI;
            }
            else if (sweep == 1 && delta_angle < 0) {
                delta_angle += 2 * M_PI;
            }
            path.AddArc(d3.x - r.x, d3.y - r.y, 2 * r.x, 2 * r.y, fmod((long double)(angle1 * 180.0f) / M_PI, 360), fmod((long double)(delta_angle * 180.0f) / M_PI, 360));
            curPoint.x = d.x;
            curPoint.y = d.y;
            lastCommand = 'a';
        }
        else if (data[idx] == 'q') {
        command_q:
            readSinglePointFirst(data, idx, d1);
            readSinglePointFirst(data, idx, d);
            path.AddBezier(curPoint.x, curPoint.y, curPoint.x + d1.x, curPoint.y + d1.y, curPoint.x + d1.x, curPoint.y + d1.y, curPoint.x + d.x, curPoint.y + d.y);
            curPoint.x += d.x;
            curPoint.y += d.y;
            lastCommand = 'q';
        }
        else if (data[idx] == 'Q') {
        command_Q:
            readSinglePointFirst(data, idx, d1);
            readSinglePointFirst(data, idx, d);
            path.AddBezier(curPoint.x, curPoint.y, d1.x, d1.y, d1.x, d1.y, d.x, d.y);
            curPoint.x = d.x;
            curPoint.y = d.y;
            lastCommand = 'Q';
        }
        else if (data[idx] == 's') {
        command_s:
            readSinglePointFirst(data, idx, d3);
            readSinglePointFirst(data, idx, d);
            if (lastCommand == 'c' || lastCommand == 'C') {
                d1.x = 2 * curPoint.x - d2.x;
                d1.y = 2 * curPoint.y - d2.y;
            }
            else if (lastCommand == 's' || lastCommand == 'S') {
                d1.x = 2 * curPoint.x - d2.x;
                d1.y = 2 * curPoint.y - d2.y;
            }
            else {
                d1.x = curPoint.x;
                d1.y = curPoint.y;
            }
            path.AddBezier(curPoint.x, curPoint.y, d1.x, d1.y, curPoint.x + d3.x, curPoint.y + d3.y, curPoint.x + d.x, curPoint.y + d.y);
            curPoint.x += d.x;
            curPoint.y += d.y;
            d2.x = d3.x + curPoint.x;
            d2.y = d3.y + curPoint.y;
            lastCommand = 's';
        }
        else if (data[idx] == 'S') {
        command_S:
            readSinglePointFirst(data, idx, d3);
            readSinglePointFirst(data, idx, d);
            if (lastCommand == 'c' || lastCommand == 'C') {
                d1.x = 2 * curPoint.x - d2.x;
                d1.y = 2 * curPoint.y - d2.y;
            }
            else if (lastCommand == 's' || lastCommand == 'S') {
                d1.x = 2 * curPoint.x - d2.x;
                d1.y = 2 * curPoint.y - d2.y;
            }
            else {
                d1.x = curPoint.x;
                d1.y = curPoint.y;
            }
            path.AddBezier(curPoint.x, curPoint.y, d1.x, d1.y, d3.x, d3.y, d.x, d.y);
            curPoint.x = d.x;
            curPoint.y = d.y;
            d2.x = d3.x;
            d2.y = d3.y;
            lastCommand = 'S';
        }
        else if (data[idx] == 'm') {
            readSinglePointFirst(data, idx, d);
            if (firstPoint) {
                firstPoint = false;
                startPoint.x = d.x, startPoint.y = d.y;
                curPoint.x = d.x, curPoint.y = d.y;
                path.StartFigure();

                lastCommand = 'm';
            }
            else {
                curPoint.x += d.x;
                curPoint.y += d.y;

                lastCommand = 'm';
            }
        }
        else if (data[idx] == 'M') {
            readSinglePointFirst(data, idx, d);
            if (firstPoint) {
                firstPoint = false;
                startPoint.x = d.x, startPoint.y = d.y;
                curPoint.x = d.x, curPoint.y = d.y;
                path.StartFigure();
                lastCommand = 'M';
            }
            else {
                curPoint.x = d.x;
                curPoint.y = d.y;
                lastCommand = 'M';
            }
        }
        else if (data[idx] == 'l') {
        command_l:
            readSinglePointFirst(data, idx, d);
            path.AddLine(curPoint.x, curPoint.y, curPoint.x + d.x, curPoint.y + d.y);
            curPoint.x += d.x;
            curPoint.y += d.y;
            lastCommand = 'l';
        }
        else if (data[idx] == 'L') {
        command_L:
            readSinglePointFirst(data, idx, d);
            path.AddLine(curPoint.x, curPoint.y, d.x, d.y);
            curPoint.x = d.x;
            curPoint.y = d.y;
            lastCommand = 'L';
        }
        else if (data[idx] == 'h') {
        command_h:
            singleType = readSinglePointSecond(data, idx);
            path.AddLine(curPoint.x, curPoint.y, curPoint.x + singleType, curPoint.y);
            curPoint.x += singleType;
            lastCommand = 'h';
        }
        else if (data[idx] == 'H') {
        command_H:
            singleType = readSinglePointSecond(data, idx);
            path.AddLine(curPoint.x, curPoint.y, singleType, curPoint.y);
            curPoint.x = singleType;
            lastCommand = 'H';
        }
        else if (data[idx] == 'v') {
        command_v:
            singleType = readSinglePointSecond(data, idx);
            path.AddLine(curPoint.x, curPoint.y, curPoint.x, curPoint.y + singleType);
            curPoint.y += singleType;
            lastCommand = 'v';
        }
        else if (data[idx] == 'V') {
        command_V:
            singleType = readSinglePointSecond(data, idx);
            path.AddLine(curPoint.x, curPoint.y, curPoint.x, singleType);
            curPoint.y = singleType;
            lastCommand = 'V';
        }
        else if (data[idx] == 'c') {
        command_c:
            readSinglePointFirst(data, idx, d1);
            readSinglePointFirst(data, idx, d2);
            readSinglePointFirst(data, idx, d);
            path.AddBezier(curPoint.x, curPoint.y, curPoint.x + d1.x, curPoint.y + d1.y, curPoint.x + d2.x, curPoint.y + d2.y, curPoint.x + d.x, curPoint.y + d.y);
            d2.x += curPoint.x;
            d2.y += curPoint.y;
            curPoint.x += d.x;
            curPoint.y += d.y;
            lastCommand = 'c';
        }
        else if (data[idx] == 'C') {
        command_C:
            readSinglePointFirst(data, idx, d1);
            readSinglePointFirst(data, idx, d2);
            readSinglePointFirst(data, idx, d);
            path.AddBezier(curPoint.x, curPoint.y, d1.x, d1.y, d2.x, d2.y, d.x, d.y);
            curPoint.x = d.x;
            curPoint.y = d.y;
            lastCommand = 'C';
        }
        else if (data[idx] <= '9' && data[idx] >= '0' || data[idx] == '.') {
            switch (lastCommand) {
            case 'm':
                readSinglePointFirst(data, idx, d);
                path.AddLine(curPoint.x, curPoint.y, curPoint.x + d.x, curPoint.y + d.y);
                curPoint.x += d.x;
                curPoint.y += d.y;
                break;

            case 'M':
                readSinglePointFirst(data, idx, d);
                path.AddLine(curPoint.x, curPoint.y, d.x, d.y);
                curPoint.x = d.x;
                curPoint.y = d.y;
                break;
            case 'c':
                goto command_c;
                break;
            case 'C':
                goto command_C;
                break;
            case 'L':
                goto command_L;
            case 'l':
                goto command_l;
                break;
            case 'h':
                goto command_h;
                break;
            case 'H':
                goto command_H;
                break;
            case 'v':
                goto command_v;
                break;
            case 'V':
                goto command_V;
                break;
            case 's':
                goto command_s;
                break;
            case 'S':
                goto command_S;
                break;
            case 'q':
                goto command_q;
                break;
            case 'Q':
                goto command_Q;
                break;
            case 'a':
                goto command_a;
                break;
            case 'A':
                goto command_A;
                break;
            }
        }
        else {
            idx++;
        }
    }

    if (linecap == "round") {
        pen.SetStartCap(LineCapRound);
        pen.SetEndCap(LineCapRound);
    }
    else if (linecap == "square") {
        pen.SetStartCap(LineCapSquare);
        pen.SetEndCap(LineCapSquare);
    }
    else {
        pen.SetStartCap(LineCapFlat);
        pen.SetEndCap(LineCapFlat);
    }
    if (linejoin == "round") {
        pen.SetLineJoin(LineJoinRound);
    }
    else if (linejoin == "bevel") {
        pen.SetLineJoin(LineJoinBevel);
    }
    else {
        pen.SetLineJoin(LineJoinMiter);
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
                gra.FillPath(&linGrBrush, &path);
                delete[] p;
                delete[] co;
                goto Draw;
                break;
            }
        }
        for (int i = 0; i < lst.radialList.size(); i++) {
            if (fillID == lst.radialList[i].id) {
                float* p = lst.radialList[i].getPointList();
                Color* co = lst.radialList[i].getColorList();
                LinearGradientBrush linGrBrush(PointF(lst.radialList[i].center.x - lst.radialList[i].r, lst.radialList[i].center.y - lst.radialList[i].r), PointF(lst.radialList[i].center.x + lst.radialList[i].r, lst.radialList[i].center.y + lst.radialList[i].r), co[0], co[lst.radialList[i].stopList.size() - 1]);
                linGrBrush.SetWrapMode(WrapModeTileFlipXY);
                linGrBrush.SetInterpolationColors(co, p, lst.radialList[i].stopList.size());
                apply(linGrBrush, lst.radialList[i].changes);
                gra.FillPath(&linGrBrush, &path);
                delete[] p;
                delete[] co;
                break;
            }
        }
    }
    else {
        gra.FillPath(&fillBrush, &path);
    }
Draw:
    if (strokeWidth != 0)
        gra.DrawPath(&pen, &path);
    gra.Restore(save);
}
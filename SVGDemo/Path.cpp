﻿#include "stdafx.h"
#include "Path.h"

void readPath(const string& name, const string& value, Path* path) {
    if (name == "d") {
        path->data = value;
    }
    else if (name == "stroke") {
        if (value == "none" || value == "transparent") {
            path->strokeOpacity = 0;
        }
        /*else if (value[0] == 'u' && value[1] == 'r' && value[2] == 'l') {
            path->strokeId = value.substr(4, value.length() - 5);
        }*/
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

        // Tìm kiếm phần tử số đầu tiên cho x
        while (idx < data.size() && !isdigit(data[idx]) && data[idx] != '-' && data[idx] != '.') {
            idx++;  // Bỏ qua các ký tự không phải là số, dấu '-' hoặc dấu '.'
        }

        // Kiểm tra dấu âm cho x
        if (idx < data.size() && data[idx] == '-') {
            negativeX = true;
            idx++;
        }

        // Trích xuất giá trị x, xử lý số mũ (nếu có)
        size_t endPos;
        p.x = stof(data.substr(idx), &endPos);  // Chuyển đổi chuỗi thành số thực
        idx += endPos;  // Cập nhật chỉ số idx sau khi trích xuất số
        if (negativeX)
            p.x *= -1;  // Nếu có dấu âm, nhân với -1

        // Tìm kiếm phần tử số tiếp theo cho y
        while (idx < data.size() && !isdigit(data[idx]) && data[idx] != '-' && data[idx] != '.') {
            idx++;  // Bỏ qua các ký tự không phải là số, dấu '-' hoặc dấu '.'
        }

        // Kiểm tra dấu âm cho y
        if (idx < data.size() && data[idx] == '-') {
            negativeY = true;
            idx++;
        }

        // Trích xuất giá trị y, xử lý số mũ (nếu có)
        p.y = stof(data.substr(idx), &endPos);  // Chuyển đổi chuỗi thành số thực
        idx += endPos;  // Cập nhật chỉ số idx sau khi trích xuất số
        if (negativeY)
            p.y *= -1;  // Nếu có dấu âm, nhân với -1
    }
    catch (const exception& e) {
        // Xử lý lỗi nếu không thể chuyển đổi chuỗi thành số
        cout << "Error parsing point at idx " << idx << ": " << e.what() << endl;
        p.x = p.y = 0;  // Gán giá trị mặc định nếu gặp lỗi
    }
}


//void Path::readSinglePointFirst(string data, int& idx, point& p) {
//    string n1 = "0", n2 = "0";
//    bool s1 = false, s2 = false, accept = false;
//    bool negative1 = false, negative2 = false;
//    while (true) {
//        //check if the idx is out of range
//        if ((data[idx] > '9' || data[idx] < '0') && data[idx] != '.' && data[idx] != 'e') {
//            if (s1 == true) {
//                s1 = false;
//                accept = true;
//            }
//            if (s2 == true) {
//                p.x = stof(n1);
//                p.y = stof(n2);
//                if (negative1)
//                    p.x *= -1;
//                if (negative2)
//                    p.y *= -1;
//                return;
//            }
//        }
//        else if (data[idx] <= '9' && data[idx] >= '0' && s1 == false && s2 == false && accept == false) {
//            n1 += data[idx];
//            if (data[idx - 1] == '-') {
//                negative1 = true;
//            }
//            s1 = true;
//            idx++;
//            continue;
//        }
//        else if (data[idx] == '.' && s1 == false && s2 == false && accept == false) {
//            n1 += data[idx];
//            if (data[idx - 1] == '-') {
//                negative1 = true;
//            }
//            s1 = true;
//            idx++;
//            continue;
//        }
//        else if (data[idx] <= '9' && data[idx] >= '0' && s1 == false && accept == true) {
//            n2 += data[idx];
//            s2 = true;
//            if (data[idx - 1] == '-') {
//                negative2 = true;
//            }
//            idx++;
//            continue;
//        }
//        else if (data[idx] == '.' && s1 == true && n1.find('.') != string::npos) {
//            n2 += data[idx];
//            s2 = true;
//            if (data[idx - 1] == '-') {
//                negative2 = true;
//            }
//            idx++;
//            s1 = false;
//            accept = true;
//            continue;
//        }
//        else if (data[idx] == '.' && s1 == false && accept == true && s2 == false) {
//            n2 += data[idx];
//            s2 = true;
//            if (data[idx - 1] == '-') {
//                negative2 = true;
//            }
//            idx++;
//            s1 = false;
//            accept = true;
//            continue;
//        }
//        if (data[idx] <= '9' && data[idx] >= '0' || data[idx] == '.' || data[idx] == 'e') {
//            if (s1) {
//                if (data[idx] == 'e') {
//                    n1 += data[idx];
//                    n1 += data[idx + 1];
//                    idx += 2;
//                    continue;
//                }
//                n1 += data[idx];
//            }
//            if (s2) {
//                if (data[idx] == 'e') {
//                    n2 += data[idx];
//                    n2 += data[idx + 1];
//                    idx += 2;
//                    continue;
//                }
//                if (data[idx] == '.' && n2.find('.') != string::npos) {
//                    p.x = stof(n1);
//                    p.y = stof(n2);
//                    if (negative1)
//                        p.x *= -1;
//                    if (negative2)
//                        p.y *= -1;
//                    return;
//                }
//                else
//                    n2 += data[idx];
//            }
//        }
//        idx++;
//    }
//}

//void Path::readSinglePointFirst(string data, int& idx, point& p) {
//    string n1 = "0", n2 = "0";
//    bool s1 = false, s2 = false, accept = false;
//    bool negative1 = false, negative2 = false;
//
//    while (idx < data.length()) {
//        char currentChar = data[idx];
//
//        // Kiểm tra nếu ký tự không phải số, dấu chấm hay 'e'
//        if ((currentChar < '0' || currentChar > '9') && currentChar != '.' && currentChar != 'e') {
//            if (s1) {  // Nếu đã đọc được n1
//                s1 = false;
//                accept = true;
//            }
//            if (s2) {  // Nếu đã đọc được n2
//                p.x = stof(n1);
//                p.y = stof(n2);
//                if (negative1) p.x *= -1;
//                if (negative2) p.y *= -1;
//                return;
//            }
//        }
//        // Xử lý trường hợp đọc n1
//        else if ((currentChar >= '0' && currentChar <= '9' || currentChar == '.') && !s1 && !s2 && !accept) {
//            n1 += currentChar;
//            if (idx > 0 && data[idx - 1] == '-') {
//                negative1 = true;
//            }
//            s1 = true;
//            idx++;
//            continue;
//        }
//        // Xử lý dấu chấm cho n1
//        else if (currentChar == '.' && !s1 && !s2 && !accept) {
//            n1 += currentChar;
//            if (idx > 0 && data[idx - 1] == '-') {
//                negative1 = true;
//            }
//            s1 = true;
//            idx++;
//            continue;
//        }
//        // Xử lý trường hợp đọc n2
//        else if ((currentChar >= '0' && currentChar <= '9' || currentChar == '.') && s1 && accept) {
//            n2 += currentChar;
//            s2 = true;
//            if (idx > 0 && data[idx - 1] == '-') {
//                negative2 = true;
//            }
//            idx++;
//            continue;
//        }
//        // Nếu gặp dấu chấm cho n2 khi đã có n1
//        else if (currentChar == '.' && s1 && n1.find('.') == string::npos) {
//            n2 += currentChar;
//            s2 = true;
//            if (idx > 0 && data[idx - 1] == '-') {
//                negative2 = true;
//            }
//            idx++;
//            s1 = false;
//            accept = true;
//            continue;
//        }
//        // Xử lý 'e' trong cả n1 và n2
//        else if (currentChar == 'e') {
//            if (s1) {
//                n1 += currentChar;
//                n1 += data[idx + 1];
//                idx += 2;
//                continue;
//            }
//            if (s2) {
//                n2 += currentChar;
//                n2 += data[idx + 1];
//                idx += 2;
//                continue;
//            }
//        }
//        // Thêm ký tự vào n1 hoặc n2 tùy theo trạng thái
//        if ((currentChar >= '0' && currentChar <= '9') || currentChar == '.' || currentChar == 'e') {
//            if (s1) {
//                n1 += currentChar;
//            }
//            if (s2) {
//                n2 += currentChar;
//            }
//        }
//        idx++;
//    }
//}


//float Path::readSinglePointSecond(string data, int& idx) {
//    string n = "0";
//    bool s = false;
//    bool negative = false;
//    while (true) {
//        if (data[idx] == '-' && data[idx + 1] <= '9' && data[idx + 1] >= '0' && n == "0") {
//            negative = true;
//        }
//        if ((data[idx] > '9' || data[idx] < '0') && data[idx] != '.' && data[idx] != 'e') {
//            if (s == true) {
//                float result = stof(n);
//                if (negative)
//                    result *= -1;
//                return  result;
//            }
//        }
//        else if (data[idx] <= '9' && data[idx] >= '0' && s == false) {
//            n += data[idx];
//            s = true;
//            if (data[idx - 1] == '-') {
//                negative = true;
//            }
//            idx++;
//            continue;
//        }
//        else if (data[idx] == '.' && s == false) {
//            n += data[idx];
//            if (data[idx - 1] == '-') {
//                negative = true;
//            }
//            s = true;
//            idx++;
//            continue;
//        }
//        if (data[idx] <= '9' && data[idx] >= '0' || data[idx] == '.' || data[idx] == 'e') {
//            if (data[idx] == 'e') {
//                n += data[idx];
//                n += data[idx + 1];
//                idx += 2;
//                continue;
//            }
//            n += data[idx];
//        }
//        idx++;
//    }
//} // đây là bản ban đầu

float Path::readSinglePointSecond(const string& data, int& idx) {
    try {
        //bool negative = false;
        //string n = "0";

        // Bỏ qua các ký tự không hợp lệ trước khi gặp số
        while (idx < data.size() && !isdigit(data[idx]) && data[idx] != '-' && data[idx] != '.') {
            idx++;
        }

        // Kiểm tra dấu âm
        /*if (data[idx] == '-') {
            negative = true;
            idx++;
        }*/

        if (idx >= data.size()) {
            cout << "Error: No valid number found at idx: " << idx << endl;
            return 0.0f;
        }

        bool isNegative = (data[idx] == '-');
        if (isNegative)
            idx++;

        // Trích xuất phần số thực
        size_t endPos;
        //n = data.substr(idx);
        //float result = stof(n, &endPos);
        float result = stof(data.substr(idx), &endPos);
        idx += endPos;

        // Nếu có dấu âm, nhân với -1
        //if (negative) {
        //    result *= -1;
        //}

        //return result;
        return isNegative ? -result : result;
    }
    catch (const exception& e) {
        // Xử lý lỗi khi không thể chuyển chuỗi thành số
        cout << "Error parsing float: " << e.what() << endl;
        return 0.0f; // Trả về giá trị mặc định nếu có lỗi
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
            idx++; // Bỏ qua ký tự không hợp lệ
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
    path.CloseFigure();  // Đóng đường path lại
    startPoint.x = currentPoint.x;  // Cập nhật điểm bắt đầu
    startPoint.y = currentPoint.y;
    //lastCommand = 'z';  // Lưu lại lệnh đã thực thi
    lastCommand = (lastCommand == 'z' || lastCommand == 'Z') ? 'Z' : 'z';
}

void Path::handleArcTo(GraphicsPath& path, const string& data, int& idx, point& currentPoint, char command) {
    point r, d, d1, d2, d3; // Đọc các giá trị từ chuỗi data
    readSinglePointFirst(data, idx, r); // Đọc bán kính
    float rotation = readSinglePointSecond(data, ++idx); // Đọc góc quay
    float largeArc = readSinglePointSecond(data, ++idx); // Đọc chế độ "large arc"
    float sweep = readSinglePointSecond(data, ++idx); // Đọc chế độ "sweep"
    readSinglePointFirst(data, ++idx, d); // Đọc điểm đích

    // Nếu là lệnh tương đối ('a'), cộng tọa độ hiện tại vào điểm đích
    if (command == 'a') {
        d.x += currentPoint.x;
        d.y += currentPoint.y;
    }

    // Chuyển đổi góc từ độ sang radian
    float angle = rotation * M_PI / 180.0f;
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);

    // Tính tọa độ trung gian d1
    float cons = cosAngle * cosAngle - (sinAngle * -sinAngle);
    d1.x = cons * (currentPoint.x - d.x) / 2.0f;
    d1.y = cons * (currentPoint.y - d.y) / 2.0f;

    // Kiểm tra bán kính
    float checkR = (d1.x * d1.x) / (r.x * r.x) + (d1.y * d1.y) / (r.y * r.y);
    if (checkR > 1.0f) {
        r.x *= sqrt(checkR);
        r.y *= sqrt(checkR);
    }

    // Tính điểm trung gian d2
    int sign = (largeArc == sweep) ? -1 : 1;
    float num = abs(r.x * r.x * r.y * r.y - r.x * r.x * d1.y * d1.y - r.y * r.y * d1.x * d1.x);
    float den = r.x * r.x * d1.y * d1.y + r.y * r.y * d1.x * d1.x;
    d2.x = sign * sqrt(num / den) * r.x * d1.y / r.y;
    d2.y = sign * sqrt(num / den) * -r.y * d1.x / r.x;

    // Tính tọa độ trung tâm d3
    //cons = cosAngle * cosAngle - (sinAngle * -sinAngle); 
    cons = cosAngle * cosAngle - (sinAngle * sinAngle);
    d3.x = cons * d2.x + (currentPoint.x + d.x) / 2.0f;
    d3.y = cons * d2.y + (currentPoint.y + d.y) / 2.0f;

    // Tính góc bắt đầu và góc quét
    float angle1 = atan2((d1.y - d2.y) / r.y, (d1.x - d2.x) / r.x);
    float angle2 = atan2((-d2.y - d1.y) / r.y, (-d2.x - d1.x) / r.x);
    float deltaAngle = angle2 - angle1;

    // Điều chỉnh nếu góc quét không phù hợp
    if (sweep == 0 && deltaAngle > 0) {
        deltaAngle -= 2 * M_PI;
    }
    else if (sweep == 1 && deltaAngle < 0) {
        deltaAngle += 2 * M_PI;
    }

    // Thêm cung tròn vào GraphicsPath
    /*path.AddArc(d3.x - r.x, d3.y - r.y, 2 * r.x, 2 * r.y,
        fmod((long double)(angle1 * 180.0f) / M_PI, 360),
        fmod((long double)(deltaAngle * 180.0f) / M_PI, 360));*/
    path.AddArc(d3.x - r.x, d3.y - r.y, 2 * r.x, 2 * r.y, fmod(angle1 * 180.0f / M_PI, 360), fmod(deltaAngle * 180.0f / M_PI, 360));

    // Cập nhật tọa độ hiện tại
    currentPoint.x = d.x;
    currentPoint.y = d.y;
}

void Path::handleQuadraticBezier(const string& data, int& idx, char command, GraphicsPath& path, point& currentPoint, char& lastCommand) {
    point d1, d;

    // Đọc các điểm điều khiển và đích
    readSinglePointFirst(data, idx, d1);
    readSinglePointFirst(data, idx, d);

    if (command == 'q') {
        // Xử lý tương đối
        //path.AddBezier(currentPoint.x, currentPoint.y, currentPoint.x + d1.x, currentPoint.y + d1.y, currentPoint.x + d1.x, currentPoint.y + d1.y, currentPoint.x + d.x, currentPoint.y + d.y);
        path.AddBezier(currentPoint.x, currentPoint.y,
            currentPoint.x + d1.x, currentPoint.y + d1.y,  // Điểm điều khiển 1
            currentPoint.x + d1.x, currentPoint.y + d1.y,  // Điểm điều khiển 2
            currentPoint.x + d.x, currentPoint.y + d.y);   // Điểm đích

        // Cập nhật tọa độ hiện tại
        currentPoint.x += d.x;
        currentPoint.y += d.y;
    }
    else if (command == 'Q') {
        // Xử lý tuyệt đối
        //path.AddBezier(currentPoint.x, currentPoint.y, d1.x, d1.y, d1.x, d1.y, d.x, d.y);

        path.AddBezier(currentPoint.x, currentPoint.y,
            d1.x, d1.y,  // Điểm điều khiển 1
            d1.x, d1.y,  // Điểm điều khiển 2
            d.x, d.y);   // Điểm đích

        // Cập nhật tọa độ hiện tại
        currentPoint.x = d.x;
        currentPoint.y = d.y;
    }

    lastCommand = command;
}

void Path::handleSmoothCubicBezier(const string& data, int& idx, char command, GraphicsPath& path, point& currentPoint, point& d2, char& lastCommand) {
    point d1, d3, d;

    // Đọc các điểm điều khiển và đích
    readSinglePointFirst(data, idx, d3);
    readSinglePointFirst(data, idx, d);

    // Xác định điểm điều khiển thứ nhất (d1)
    /*if (lastCommand == 'c' || lastCommand == 'C') {
        d1.x = 2 * currentPoint.x - d2.x;
        d1.y = 2 * currentPoint.y - d2.y;
    }
    else if (lastCommand == 's' || lastCommand == 'S') {
        d1.x = 2 * currentPoint.x - d2.x;
        d1.y = 2 * currentPoint.y - d2.y;
    }
    else {
        d1.x = currentPoint.x;
        d1.y = currentPoint.y;
    }*/

    if (lastCommand == 'c' || lastCommand == 'C' || lastCommand == 's' || lastCommand == 'S') {
        // Sử dụng điểm điều khiển đối xứng với d2
        d1.x = 2 * currentPoint.x - d2.x;
        d1.y = 2 * currentPoint.y - d2.y;
    }
    else {
        // Nếu là lệnh đầu tiên, d1 = currentPoint
        d1.x = currentPoint.x;
        d1.y = currentPoint.y;
    }

    if (command == 's') {
        // Xử lý tương đối
        //path.AddBezier(currentPoint.x, currentPoint.y, d1.x, d1.y, currentPoint.x + d3.x, currentPoint.y + d3.y, currentPoint.x + d.x, currentPoint.y + d.y);
        
        path.AddBezier(currentPoint.x, currentPoint.y,
            currentPoint.x + d1.x, currentPoint.y + d1.y,  // Điểm điều khiển 1
            currentPoint.x + d3.x, currentPoint.y + d3.y,  // Điểm điều khiển 2
            currentPoint.x + d.x, currentPoint.y + d.y);   // Điểm đích

        // Cập nhật tọa độ hiện tại
        currentPoint.x += d.x;
        currentPoint.y += d.y;
        d2.x = currentPoint.x + d3.x;
        d2.y = currentPoint.y + d3.y;
    }
    else if (command == 'S') {
        // Xử lý tuyệt đối
        //path.AddBezier(currentPoint.x, currentPoint.y, d1.x, d1.y, d3.x, d3.y, d.x, d.y);

        path.AddBezier(currentPoint.x, currentPoint.y,
                        d1.x, d1.y,  // Điểm điều khiển 1
                        d3.x, d3.y,  // Điểm điều khiển 2
                        d.x, d.y);   // Điểm đích

        // Cập nhật tọa độ hiện tại 
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
        /*currentPoint.x = startPoint.x;
        currentPoint.y = startPoint.y;*/
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
        /*currentPoint.x = d.x;
        currentPoint.y = d.y;*/
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

//void Path::handleCubicBezier(const string& data, int& idx, char command, GraphicsPath& path, point& currentPoint, point& d2, char& lastCommand) {
//    point d1, d, control2;
//
//    // Đọc các điểm điều khiển và điểm kết thúc
//    readSinglePointFirst(data, idx, d1);
//    readSinglePointFirst(data, idx, control2);
//    readSinglePointFirst(data, idx, d);
//
//    if (command == 'c') {
//        path.AddBezier(currentPoint.x, currentPoint.y, currentPoint.x + d1.x, currentPoint.y + d1.y, currentPoint.x + control2.x, currentPoint.y + control2.y, currentPoint.x + d.x, currentPoint.y + d.y);
//
//        d2.x = currentPoint.x + control2.x;
//        d2.y = currentPoint.y + control2.y;
//
//        currentPoint.x += d.x;
//        currentPoint.y += d.y;
//    }
//    else if (command == 'C') {
//        path.AddBezier(currentPoint.x, currentPoint.y, d1.x, d1.y, control2.x, control2.y, d.x, d.y);
//
//        d2.x = control2.x;
//        d2.y = control2.y;
//
//        currentPoint.x = d.x;
//        currentPoint.y = d.y;
//    }
//
//    lastCommand = command;
//}

void Path::handleCubicBezier(const string& data, int& idx, char command, GraphicsPath& path, point& currentPoint, point& d2, char& lastCommand) {
    point d1, control2, d;

    // Đọc các điểm điều khiển và điểm kết thúc
    readSinglePointFirst(data, idx, d1);
    readSinglePointFirst(data, idx, control2);
    readSinglePointFirst(data, idx, d);

    // Tính toán các điểm điều khiển và điểm kết thúc
    point p1 = (command == 'c') ? point{ currentPoint.x + d1.x, currentPoint.y + d1.y } : d1;
    point p2 = (command == 'c') ? point{ currentPoint.x + control2.x, currentPoint.y + control2.y } : control2;
    point p3 = (command == 'c') ? point{ currentPoint.x + d.x, currentPoint.y + d.y } : d;

    // Thêm Bezier vào đường path
    path.AddBezier(currentPoint.x, currentPoint.y, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);

    // Cập nhật d2 (điểm điều khiển cuối cùng)
    d2.x = p2.x;
    d2.y = p2.y;

    // Cập nhật currentPoint
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

    // Tính toán điểm điều khiển dựa trên lệnh trước đó
    if (lastCommand == 'Q' || lastCommand == 'T') {
        controlPoint.x = 2 * currentPoint.x - lastControlPoint.x;
        controlPoint.y = 2 * currentPoint.y - lastControlPoint.y;
    }
    else {
        // Nếu không có lệnh trước hoặc không liên quan, điểm điều khiển trùng với điểm hiện tại
        controlPoint.x = currentPoint.x;
        controlPoint.y = currentPoint.y;
    }

    // Đọc điểm đích
    readSinglePointFirst(data, idx, d);

    //if (command == 't') {
    //    // Xử lý tọa độ tương đối
    //    path.AddBezier(
    //        currentPoint.x, currentPoint.y,
    //        controlPoint.x, controlPoint.y,
    //        controlPoint.x, controlPoint.y,
    //        currentPoint.x + d.x, currentPoint.y + d.y
    //    );
    //    currentPoint.x += d.x;
    //    currentPoint.y += d.y;
    //}
    //else if (command == 'T') {
    //    // Xử lý tọa độ tuyệt đối
    //    path.AddBezier(
    //        currentPoint.x, currentPoint.y,
    //        controlPoint.x, controlPoint.y,
    //        controlPoint.x, controlPoint.y,
    //        d.x, d.y
    //    );
    //    currentPoint.x = d.x;
    //    currentPoint.y = d.y;
    //}

    if (command == 't' || command == 'T') {
        float endX = (command == 't') ? currentPoint.x + d.x : d.x;
        float endY = (command == 't') ? currentPoint.y + d.y : d.y;

        path.AddBezier(
            currentPoint.x, currentPoint.y,
            controlPoint.x, controlPoint.y,
            controlPoint.x, controlPoint.y,
            endX, endY
        );

        // Cập nhật currentPoint
        currentPoint.x = endX;
        currentPoint.y = endY;
    }

    // Cập nhật trạng thái
    lastControlPoint = controlPoint; // Lưu điểm điều khiển cuối cùng
    lastCommand = command; // Lưu lệnh vừa xử lý
}

void Path::configurePen(Pen& pen, const string& linecap, const string& linejoin) {
    // Cấu hình LineCap
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
    // Cảnh báo nếu giá trị không hợp lệ
    cout << "Warning: Invalid linecap value \"" << linecap << "\". Using default (flat) instead." << endl;
    pen.SetStartCap(LineCapFlat);
    pen.SetEndCap(LineCapFlat);
    }

    // Cấu hình LineJoin
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
        // Cảnh báo nếu giá trị không hợp lệ
        cout << "Warning: Invalid linejoin value \"" << linejoin << "\". Using default (miter) instead." << endl;
        pen.SetLineJoin(LineJoinMiter);
    }
}




#include "stdafx.h"
#include "Gradient.h"

void readStop(string name, string value, stopPoint& stop) {
    if (name == "stop-color") 
        stop.stopColor = readRGB(value); // Xử lý màu sắc
    
    else if (name == "stop-opacity") 
        stop.stopOpacity = clarify(value); // Xử lý độ trong suốt
    
    else if (name == "offset") 
        stop.offset = clarify(value); // Xử lý vị trí điểm dừng
    
}

void readLinearGradient(string name, string value, linearGradient& lg) {
    if (name == "id") {
        lg.id = value;
    }
    else if (name == "x1" || name == "x2") {
        float& x = (name == "x1") ? lg.start.x : lg.end.x;
        if (value.back() == '%') {
            x = clarify(value.substr(0, value.length() - 1));
            lg.percentage = true;
        }
        else {
            x = stof(value);
        }
    }
    else if (name == "y1" || name == "y2") {
        float& y = (name == "y1") ? lg.start.y : lg.end.y;
        if (value.back() == '%') {
            y = clarify(value.substr(0, value.length() - 1));
        }
        else {
            y = stof(value);
        }
    }
    else if (name == "gradientTransform") {
        readTransform(value, lg.trs);
    }
    else if (name == "gradientUnits") {
        lg.units = value;
    }
}

void readRadialGradient(string name, string value, radialGradient& rg) {
    if (name == "id") {
        rg.id = value;
    }
    else if (name == "cx" || name == "cy" || name == "r" || name == "fx" || name == "fy") {
        float* target = nullptr;

        if (name == "cx") {
            target = &rg.center.x;
        }
        else if (name == "cy") {
            target = &rg.center.y;
        }
        else if (name == "r") {
            target = &rg.r;
        }
        else if (name == "fx") {
            target = &rg.fx;
        }
        else if (name == "fy") {
            target = &rg.fy;
        }

        if (target) {
            *target = stof(value);  
        }
    }
    else if (name == "xlink:href") {
        rg.xlink_href = value;
    }
    else if (name == "gradientTransform") {
        readTransform(value, rg.trs);
    }
    else if (name == "gradientUnits") {
        rg.units = value;
    }
}

float* Gradient::getPointList() {
    int size = stopList.size();

    // Kiểm tra và điều chỉnh kích thước nếu cần
    if (stopList[0].offset != 0) {
        size++;
    }
    if (stopList[stopList.size() - 1].offset != 1) {
        size++;
    }

    // Cấp phát bộ nhớ cho mảng điểm
    float* points = new float[size];

    // Điền điểm đầu tiên nếu cần
    int index = 0;
    if (stopList[0].offset != 0) {
        points[index++] = 0.0f;
    }

    // Điền các điểm từ stopList
    for (const auto& stop : stopList) {
        if (index < size)
            points[index++] = stop.offset;
    }

    // Điền điểm cuối cùng nếu cần
    if (stopList[stopList.size() - 1].offset != 1 && index < size) {
        points[index] = 1.0f;
    }

    return points;
}

Color* Gradient::getColorList() {
    if (stopList.empty())
        return nullptr;

    int size = stopList.size();
    Color* colors = new Color[size];

    for (int i = 0; i < size; i++) {
        // Đảm bảo stopOpacity nằm trong khoảng [0.0, 1.0]
        float opacity = clamp(stopList[i].stopOpacity, 0.0f, 1.0f) * 255.0f;
        
        colors[i] = Color(
            opacity, // Giá trị opacity
            stopList[i].stopColor.red,  // Giá trị màu đỏ
            stopList[i].stopColor.green,  // Giá trị màu xanh lá
            stopList[i].stopColor.blue   // Giá trị màu xanh dương
        );
    }

    return colors;
}




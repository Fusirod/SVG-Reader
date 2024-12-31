#include "stdafx.h"
#include "Gradient.h"

void readStop(string name, string value, stopPoint& stop) {
    if (name == "stop-color") 
        stop.stopColor = readRGB(value); // Color solving
    
    else if (name == "stop-opacity") 
        stop.stopOpacity = clarify(value); // Opacity solving
    
    else if (name == "offset") 
        stop.offset = clarify(value); // Stop location solving
    
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

    // Check and edit size
    if (stopList[0].offset != 0) {
        size++;
    }
    if (stopList[stopList.size() - 1].offset != 1) {
        size++;
    }
    float* points = new float[size];

    int index = 0;
    if (stopList[0].offset != 0) {
        points[index++] = 0.0f;
    }

    for (const auto& stop : stopList) {
        if (index < size)
            points[index++] = stop.offset;
    }

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
        float opacity = clamp(stopList[i].stopOpacity, 0.0f, 1.0f) * 255.0f;
        
        colors[i] = Color(
            opacity, 
            stopList[i].stopColor.red,  
            stopList[i].stopColor.green,  
            stopList[i].stopColor.blue   
        );
    }

    return colors;
}




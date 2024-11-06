#include "Gradient.h"

void readStop(string name, string val, stop& st) {
    if (name == "stop-color") {
        st.colorStop = readRGB(val);
    }
    else if (name == "stop-opacity") {
        st.opacityStop = clarify(val);
    }
    else if (name == "offset") {
        st.offset = clarify(val);
    }
}

void readLinear(string name, string val, linearGradient& linear) {
    if (name == "id") {
        linear.id = val;
    }
    else if (name == "x1") {
        if (val[val.length() - 1] == '%') {
            linear.percent = true;
            linear.start.x = clarify(val.substr(0, val.length() - 1));
        }
        else {
            linear.start.x = stof(val);
        }
    }
    else if (name == "x2") {
        if (val[val.length() - 1] == '%') {
            linear.end.x = clarify(val.substr(0, val.length() - 1));
        }
        else
            linear.end.x = stof(val);
    }
    else if (name == "y1") {
        if (val[val.length() - 1] == '%') {
            linear.start.y = clarify(val.substr(0, val.length() - 1));
        }
        else
            linear.start.y = stof(val);
    }
    else if (name == "y2") {
        if (val[val.length() - 1] == '%') {
            linear.end.y = clarify(val.substr(0, val.length() - 1));
        }
        else
            linear.end.y = stof(val);
    }
    else if (name == "gradientTransform") {
        readTransform(val, linear.changes);
    }
    else if (name == "gradientUnits") {
        linear.units = val;
    }
}

void readRadial(string name, string val, radialGradient& radial) {
    if (name == "id") {
        radial.id = val;
    }
    else if (name == "cx") {
        radial.center.x = stof(val);
    }
    else if (name == "cy") {
        radial.center.y = stof(val);
    }
    else if (name == "r") {
        radial.r = stof(val);
    }
    else if (name == "xlink:href") {
        radial.link_href = val;
    }
    else if (name == "gradientTransform") {
        readTransform(val, radial.changes);
    }
    else if (name == "gradientUnits") {
       radial.units = val;
    }
    else if (name == "fx") {
        radial.rx = stof(val);
    }
    else if (name == "fy") {
        radial.ry = stof(val);
    }
}


float* Gradient::getPointList() {
    int listSize = stopList.size();
    if (stopList[0].offset != 0) {
        listSize++;
    }
    if (stopList[stopList.size() - 1].offset != 1) {
        listSize++;
    }

    float* p = new float[listSize];
    if (stopList[0].offset != 0) {
        p[0] = 0;
    }
    if (stopList[stopList.size() - 1].offset != 1) {
        p[listSize - 1] = 1.0f;
    }

    for (int j = 0; j < stopList.size(); j++) {
        if (stopList[0].offset != 0) {
            p[j + 1] = stopList[j].offset;
        }
        else {
            p[j] = stopList[j].offset;
        }
    }
    return p;
}

Color* Gradient::getColorList() {
    Color* co = new Color[stopList.size()];
    for (int j = 0; j < stopList.size(); j++)
        co[j] = Color(static_cast<float>(stopList[j].opacityStop * 255), stopList[j].colorStop.red, stopList[j].colorStop.green, stopList[j].colorStop.blue);

    return co;
}
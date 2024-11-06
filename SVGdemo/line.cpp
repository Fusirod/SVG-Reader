#include "Shape.h"

VOID line::draw(Graphics& gra, List lst) {
    GraphicsState save = gra.Save();
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
    gra.DrawLine(&pen, start.x, start.y, end.x, end.y);
    gra.Restore(save);
}

void readLine(string name, string val, line* l) {
    if (name == "stroke-opacity") {
        l->strokeOpacity = stof(val);
    }
    else if (name == "stroke") {
        if (val == "none" || val == "transparent") {
            l->strokeOpacity = 0;
        }
        else if (val[0] == 'u' && val[1] == 'r' && val[2] == 'l') {
            l->strokeID = val.substr(5, val.length() - 6);
        }
        else {
            l->strokeColor = readRGB(val);
            if (l->strokeWidth == 0)
                l->strokeWidth = 1;
        }
    }
    else if (name == "x1") {
        l->start.x = stof(val);
    }
    else if (name == "y1") {
        l->start.y = stof(val);
    }
    else if (name == "x2") {
        l->end.x = stof(val);
    }
    else if (name == "y2") {
        l->end.y = stof(val);
    }
    else if (name == "stroke-width") {
        l->strokeWidth = stof(val);
    }
    else if (name == "transform") {
        readTransform(val, l->changes);
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
            readLine(s1, s2, l);
        }
    }
}
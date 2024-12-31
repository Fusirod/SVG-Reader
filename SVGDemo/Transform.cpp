#include "Transform.h"

void transformImage(Graphics& graphic, float angle, float width, float height, float scrollX, float scrollY, float scale) {
    PointF center(static_cast<REAL>((width / 2 + scrollX) * scale), static_cast<REAL>((height / 2 + scrollY) * scale));

    // Applying all the transformations
    Matrix transformMatrix;
    transformMatrix.Translate(scrollX, scrollY);   // Translate first
    transformMatrix.Scale(scale, scale);           // Then scale
    transformMatrix.RotateAt(angle, center);       // Finally rotate
    graphic.SetTransform(&transformMatrix);
    graphic.SetSmoothingMode(SmoothingModeAntiAlias);
}

void readTransform(string val, transformShape& trs) {
    removeSpace(val); 
    size_t pos = 0;

    while (pos < val.length()) {
        if (val.find("translate", pos) == pos) {
            trs.type.push_back("translate");
            size_t start = val.find('(', pos) + 1;
            size_t end = val.find(')', start);
            string params = val.substr(start, end - start);
            stringstream ss(params);
            string x, y;
            getline(ss, x, ',');
            getline(ss, y, ',');
            trs.value.push_back(clarify(x));
            trs.value.push_back(clarify(y.empty() ? x : y));
            pos = end + 1;
        }
        else if (val.find("scale", pos) == pos) {
            trs.type.push_back("scale");
            size_t start = val.find('(', pos) + 1;
            size_t end = val.find(')', start);
            string params = val.substr(start, end - start);
            stringstream ss(params);
            string sx, sy;
            getline(ss, sx, ',');
            getline(ss, sy, ',');
            trs.value.push_back(clarify(sx));
            trs.value.push_back(clarify(sy.empty() ? sx : sy));
            pos = end + 1;
        }
        else if (val.find("rotate", pos) == pos) {
            trs.type.push_back("rotate");
            size_t start = val.find('(', pos) + 1;
            size_t end = val.find(')', start);
            string param = val.substr(start, end - start);
            trs.value.push_back(clarify(param));
            pos = end + 1;
        }
        else if (val.find("matrix", pos) == pos) {
            trs.type.push_back("translate");
            trs.type.push_back("scale");
            trs.type.push_back("rotate");
            size_t start = val.find('(', pos) + 1;
            size_t end = val.find(')', start);
            string params = val.substr(start, end - start);
            stringstream ss(params);
            string a, b, c, d, e, f;
            getline(ss, a, ',');
            getline(ss, b, ',');
            getline(ss, c, ',');
            getline(ss, d, ',');
            getline(ss, e, ',');
            getline(ss, f, ',');
            trs.value.push_back(clarify(a));
            trs.value.push_back(clarify(b));
            trs.value.push_back(clarify(c));
            trs.value.push_back(clarify(d));
            trs.value.push_back(clarify(e));
            trs.value.push_back(clarify(f));
            pos = end + 1;
        }
        else {
            pos++;
        }
    }
}

void applyTransform(LinearGradientBrush& lgbrush, transformShape& trs) {
    // Check if there no transformation
    if (trs.type.empty()) {
        return;
    }

    // Check if there exactly 3 transformations, use multiply matrix transformation
    if (trs.type.size() == 3) {
        Matrix matrix(trs.value[0], trs.value[1], trs.value[2], trs.value[3], trs.value[4], trs.value[5]);
        lgbrush.MultiplyTransform(&matrix, MatrixOrderAppend);
        return;
    }

    // Use loop to solve all the transformation
    for (int i = 0; i < static_cast<int>(trs.type.size()); ++i) {
        const string& type = trs.type[i];
        if (type == "translate") {
            lgbrush.TranslateTransform(trs.value[i], trs.value[i + 1], MatrixOrderAppend);
        }
        else if (type == "rotate") {
            lgbrush.RotateTransform(trs.value[i], MatrixOrderAppend);
        }
        else if (type == "scale") {
            lgbrush.ScaleTransform(trs.value[i], MatrixOrderAppend);
        }
    }
}

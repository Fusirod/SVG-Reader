#include "Change.h"

void image(Graphics& grap, float width, float height, float scrollX, float scrollY, float scale, float angle) {
    PointF center(static_cast<REAL>((width / 2 + scrollX) * scale), static_cast<REAL>((height / 2 + scrollY) * scale));

    // applying all the transformations
    Matrix transformMatrix;
    transformMatrix.Translate(scrollX, scrollY); // Translate first
    transformMatrix.Scale(scale, scale);           // Then scale
    transformMatrix.RotateAt(angle, center);       // Finally rotate

    grap.SetTransform(&transformMatrix);

    grap.SetSmoothingMode(SmoothingModeAntiAlias);
}

void readTransform(string value, Change& ch) {
    string tmp = value;
    string vessel1 = "", vessel2 = "";
    bool change = true;
    removeSpace(value);
    for (int i = 0; i < value.length() - 1; i++) {
        if (value[i] == 't' && value[i + 1] == 'r') {
            ch.types.push_back("translate");
            if (value.find("translate") != string::npos)
            {
                stringstream ss(value.substr(value.find("translate") + 10));
                getline(ss, tmp, ',');
                ch.values.push_back(clarify(tmp));
                getline(ss, tmp, ')');
                ch.values.push_back(clarify(tmp));
                value = value.substr(value.find("translate") + 10);
            }
        }
        else if (value[i] == 's' && value[i + 1] == 'c') {
            ch.types.push_back("scale");
            if (value.find("scale") != string::npos) {
                stringstream ss(value.substr(value.find("scale") + 6));
                getline(ss, tmp, ')');
                for (int i = 0; i < tmp.length(); i++) {
                    if (tmp[i] == ',') {
                        change = false;
                        continue;
                    }
                    if (change) {
                        vessel1 += tmp[i];
                    }
                    else {
                        vessel2 += tmp[i];
                    }
                }
                if (vessel2.length() == 0) {
                    ch.values.push_back(clarify(vessel1));
                    ch.values.push_back(clarify(vessel1));
                }
                else {
                    ch.values.push_back(clarify(vessel1));
                    ch.values.push_back(clarify(vessel2));
                }
                value = value.substr(value.find("scale") + 6);
            }
        }
        else if (value[i] == 'r' && value[i + 1] == 'o') {
            ch.types.push_back("rotate");
            if (value.find("rotate") != string::npos) {
                stringstream ss(value.substr(value.find("rotate") + 7));
                getline(ss, tmp, ')');
                ch.values.push_back(clarify(tmp));
                value = value.substr(value.find("rotate") + 7);
            }
        }
        else if (value[i] == 'm' && value[i + 1] == 'a' && value[i + 2] == 't') {
            ch.types.push_back("translate");
            ch.types.push_back("scale");
            ch.types.push_back("rotate");
            string tmp1, tmp2, tmp3, tmp4, tmp5;
            stringstream ss(value);
            getline(ss, tmp, '(');
            getline(ss, tmp, ',');
            getline(ss, tmp1, ',');
            getline(ss, tmp2, ',');
            getline(ss, tmp3, ',');
            getline(ss, tmp4, ',');
            getline(ss, tmp5, ')');
            ch.values.push_back(clarify(tmp));
            ch.values.push_back(clarify(tmp1));
            ch.values.push_back(clarify(tmp2));
            ch.values.push_back(clarify(tmp3));
            ch.values.push_back(clarify(tmp4));
            ch.values.push_back(clarify(tmp5));
            break;
        }
    }
}

void apply(LinearGradientBrush& lgbrush, Change& ch) {
    int idx = 0;
    if (ch.types.size() == 0)
        return;
    if (ch.types.size() == 3) {
        Matrix matrix(ch.values[0], ch.values[1], ch.values[2], ch.values[3], ch.values[4], ch.values[5]);
        lgbrush.MultiplyTransform(&matrix, MatrixOrderAppend);
        return;
    }
    if (ch.types[0] == "translate") {
        lgbrush.TranslateTransform(ch.values[idx], ch.values[idx + 1], MatrixOrderAppend);
        return;
    }
    if (ch.types[0] == "rotate") {
        lgbrush.RotateTransform(ch.values[idx], MatrixOrderAppend);
        return;
    }
    if (ch.types[0] == "scale") {
        lgbrush.ScaleTransform(ch.values[idx], MatrixOrderAppend);
        return;
    }
}

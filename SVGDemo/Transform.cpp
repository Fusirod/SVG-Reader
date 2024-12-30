#include "Transform.h"

void transformImage(Graphics& graphic, float angle, float width, float height, float scrollX, float scrollY, float scale) {
    PointF center(static_cast<REAL>((width / 2 + scrollX) * scale), static_cast<REAL>((height / 2 + scrollY) * scale));

    // applying all the transformations
    Matrix transformMatrix;
    transformMatrix.Translate(scrollX, scrollY);   // Translate first
    transformMatrix.Scale(scale, scale);           // Then scale
    transformMatrix.RotateAt(angle, center);       // Finally rotate
    graphic.SetTransform(&transformMatrix);
    graphic.SetSmoothingMode(SmoothingModeAntiAlias);
}

//void readTransform(string val, transformShape& tr) {
//    // Loại bỏ khoảng trắng thừa
//    //val.erase(remove_if(val.begin(), val.end(), ::isspace), val.end());
//    removeSpace(val);
//
//    // Biểu thức regex để tìm phép biến đổi (translate, scale, rotate, matrix)
//    regex rgx(R"((\w+)\(([^)]+)\))");
//    smatch match;
//
//    // Tìm kiếm và xử lý từng phép biến đổi trong chuỗi
//    while (std::regex_search(val, match, rgx)) {
//        string type = match[1].str(); // Lấy tên phép biến đổi (translate, scale, rotate, ...)
//        string params = match[2].str(); // Lấy tham số trong dấu ngoặc
//
//        tr.type.push_back(type); // Thêm loại phép biến đổi vào
//        stringstream ss(params);
//        string param;
//
//        // Phân tách các tham số và chuyển thành float
//        while (getline(ss, param, ',')) {
//            try {
//                tr.val.push_back(stof(param)); // Chuyển chuỗi thành số thực
//            }
//            catch (...) {
//                tr.val.push_back(0.0f); // Nếu có lỗi thì trả về 0
//            }
//        }
//
//        // Nếu là phép biến đổi matrix, tách riêng các giá trị vào tr.val
//        if (type == "matrix") {
//            stringstream ss(params);
//            string param;
//            while (getline(ss, param, ',')) {
//                try {
//                    tr.val.push_back(stof(param)); // Chuyển chuỗi thành số thực
//                }
//                catch (...) {
//                    tr.val.push_back(0.0f);
//                }
//            }
//            tr.type.push_back("matrix"); // Đánh dấu phép biến đổi là matrix
//        }
//
//        // Cập nhật chuỗi còn lại để tiếp tục xử lý các phép biến đổi khác
//        val = match.suffix();
//    }
//}

//void readTransform(string val, transformShape& trs) {
//    string temp = val;
//    string vessel1 = "", vessel2 = "";
//    bool change = true;
//    removeSpace(val);
//    for (int i = 0; i < val.length() - 1; i++) {
//        if (val[i] == 't' && val[i + 1] == 'r') {
//            trs.type.push_back("translate");
//            if (val.find("translate") != string::npos)
//            {
//                stringstream ss(val.substr(val.find("translate") + 10));
//                getline(ss, temp, ',');
//                trs.value.push_back(clarify(temp));
//                getline(ss, temp, ')');
//                trs.value.push_back(clarify(temp));
//                val = val.substr(val.find("translate") + 10);
//            }
//        }
//        else if (val[i] == 's' && val[i + 1] == 'c') {
//            trs.type.push_back("scale");
//            if (val.find("scale") != string::npos) {
//                stringstream ss(val.substr(val.find("scale") + 6));
//                getline(ss, temp, ')');
//                for (int i = 0; i < temp.length(); i++) {
//                    if (temp[i] == ',') {
//                        change = false;
//                        continue;
//                    }
//                    if (change) {
//                        vessel1 += temp[i];
//                    }
//                    else {
//                        vessel2 += temp[i];
//                    }
//                }
//                if (vessel2.length() == 0) {
//                    trs.value.push_back(clarify(vessel1));
//                    trs.value.push_back(clarify(vessel1));
//                }
//                else {
//                    trs.value.push_back(clarify(vessel1));
//                    trs.value.push_back(clarify(vessel2));
//                }
//                val = val.substr(val.find("scale") + 6);
//            }
//        }
//        else if (val[i] == 'r' && val[i + 1] == 'o') {
//            trs.type.push_back("rotate");
//            if (val.find("rotate") != string::npos) {
//                stringstream ss(val.substr(val.find("rotate") + 7));
//                getline(ss, temp, ')');
//                trs.value.push_back(clarify(temp));
//                val = val.substr(val.find("rotate") + 7);
//            }
//        }
//        else if (val[i] == 'm' && val[i + 1] == 'a' && val[i + 2] == 't') {
//            trs.type.push_back("translate");
//            trs.type.push_back("scale");
//            trs.type.push_back("rotate");
//            string temp1, temp2, temp3, temp4, temp5;
//            stringstream ss(val);
//            getline(ss, temp, '(');
//            getline(ss, temp, ',');
//            getline(ss, temp1, ',');
//            getline(ss, temp2, ',');
//            getline(ss, temp3, ',');
//            getline(ss, temp4, ',');
//            getline(ss, temp5, ')');
//            trs.value.push_back(clarify(temp));
//            trs.value.push_back(clarify(temp1));
//            trs.value.push_back(clarify(temp2));
//            trs.value.push_back(clarify(temp3));
//            trs.value.push_back(clarify(temp4));
//            trs.value.push_back(clarify(temp5));
//            break;
//        }
//    }
//}

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
    // Kiểm tra nếu không có phép biến đổi nào
    if (trs.type.empty()) {
        return;
    }

    // Kiểm tra nếu có đúng 3 phép biến đổi, áp dụng ma trận tổng hợp
    if (trs.type.size() == 3) {
        Matrix matrix(trs.value[0], trs.value[1], trs.value[2], trs.value[3], trs.value[4], trs.value[5]);
        lgbrush.MultiplyTransform(&matrix, MatrixOrderAppend);
        return;
    }

    // Dùng vòng lặp với int và if để xử lý các phép biến đổi
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

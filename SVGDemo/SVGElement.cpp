#include "stdafx.h"
#include "SVGElement.h"

string trim(const string& str) {
    int first = 0, last = str.length() - 1;
    while (first <= last && (str[first] == ' ' 
            || str[first] == '\t' 
            || str[first] == '\n' 
            || str[first] == '\r')) {
        ++first;
    }
    while (last >= first && (str[last] == ' ' 
        || str[last] == '\t' 
        || str[last] == '\n' 
        || str[last] == '\r')) {
        --last;
    }
    if (first > last) return "";
    return str.substr(first, last - first + 1);
}

bool check(char a) {
    if (a <= 'Z' && a >= 'A')
        return true;
    if (a <= 'z' && a >= 'a')
        return true;
    if (a <= '9' && a >= '0')
        return true;
    if (a == '(' || a == ')')
        return true;
    return false;
}

void removeSpace(string& s) {
    for (int i = 1; i < s.length() - 1;) {
        if (!check(s[i])) {
            if (s[i - 1] >= '0' && s[i - 1] <= '9' && ((s[i + 1] >= '0' && s[i + 1] <= '9') || s[i + 1] == '-' || s[i + 1] == '.') && s[i] != '.') {
                s[i] = ',';
                ++i;
            } else if (s[i] != '.' && s[i] != '-') {
                s.erase(i, 1);
            } else {
                ++i; 
            }
        } else {
            ++i;
        }
    }
}


float clarify(string s) {
    if (s.empty()) 
        return 0.0f;

    if (s[0] == '.' || (s[0] == '-' && s[1] == '.')) {
        s.insert(0, "0");
    }

    if (s.back() == '%') {
        s.pop_back();
        return stof(s) / 100;
    }

    return stof(s);
}

colour readRGB(string value) {
    colour colour;
    if (value.substr(0, 3) == "rgb") {
        stringstream ss(value);
        string temp;
        getline(ss, temp, '(');
        ss >> colour.red;
        ss.ignore(); 
        ss >> colour.green;
        ss.ignore(); 
        ss >> colour.blue;
        return colour;
    }

    if (value[0] == '#') {
        unsigned int hexValue;
        if (value.length() == 4) { 
            value = "#" + string(1, value[1]) + string(1, value[1]) + string(1, value[2]) + string(1, value[2]) + string(1, value[3]) + string(1, value[3]);
        }
        istringstream(value.substr(1)) >> hex >> hexValue;
        colour.red = (hexValue >> 16) & 0xFF;
        colour.green = (hexValue >> 8) & 0xFF;
        colour.blue = hexValue & 0xFF;
        return colour;
    }

    transform(value.begin(), value.end(), value.begin(), ::tolower);
    ifstream file("rgb.txt");
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string name;
            getline(ss, name, '"');
            if (name == value) {
                ss >> colour.red >> colour.green >> colour.blue;
                break;
            }
        }
        file.close();
    }
    return colour;
}

vector<point> readPoint(string value) {
    vector<point> points;
    value = std::regex_replace(value, std::regex("^\\s+|\\s+$"), "");

    stringstream ss(value);
    string temp = "";
    string pointStr;
    while (getline(ss, pointStr, ' ')) {
        point p;
        if (pointStr.find(',') == string::npos) {
            p.x = stof(pointStr);
            getline(ss, pointStr, ' ');
            p.y = stof(pointStr);
            points.push_back(p);
        }
        else {
            stringstream pointStream(pointStr);
            getline(pointStream, pointStr, ',');
            p.x = stof(pointStr);
            getline(pointStream, pointStr, ',');
            p.y = stof(pointStr);
            points.push_back(p);
        }
    }
    return points;
}

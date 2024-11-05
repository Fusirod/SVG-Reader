#include "Base.h"

string trim(string s) {
	/*s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
	return s;*/

	string res;
	for (char c : s)
		if (!isspace(c))
			res += c;
	return res;
}

bool check(char c) {
	if (c >= 'A' && c <= 'Z')
		return true;
	if (c >= 'a' && c <= 'z')
		return true;
	if (c == '(' || c == ')')
		return true;
	return false;
}

void removeSpace(string& s) {
	for (int i = 1; i < s.length() - 1; i++) {
		if (!check(s[i])) {
			/*if (s[i - 1] <= '9' && s[i - 1] >= '0' &&
				((s[i + 1] <= '9' && s[i + 1] >= '0') ||
				s[i + 1] == '-' || s[i + 1] == '.') &&
				s[i + 1] != '.') {
				s[i] = ',';
				continue;
			}*/
			if (isdigit(s[i - 1]) && (isdigit(s[i + 1]) || s[i + 1] == '-' || s[i + 1] == '.') && s[i + 1] != '.') {
				s[i + 1] = ',';
				continue;
			}
			else if (s[i] != '.' && s[i] != '-') {
				s.erase(i, 1);
				i--;
			}
		}
	}
}

// mod toi day

float clarify(string s) {
	if (s[0] == '.')
		s.insert(0, "0");

	if (s[0] == '-' && s[1] == '.')
		s.insert(1, "0");
	if (s[s.length() - 1] == '%') {
		string str = s.substr(0, s.length() - 1);
		return stof(str) / 100;
	}

	return stof(s);
}

color readRGB(string val) {
	color co;
	if (val[0] == 'r' && val[1] == 'g' && val[2] == 'b') {
		stringstream ss(val);
		string tmp = "";
		getline(ss, tmp, '(');
		getline(ss, tmp, ',');
		if (stof(tmp) > 255)
			tmp = "255";
		co.green = stoi(tmp);
		getline(ss, tmp, ')');
		if (stof(tmp) > 255)
			tmp = "255";
		co.blue = stoi(tmp);
	}
	else if (val[0] == '#') {
		unsigned int hexVal;
		if (val.length() == 4) {
			char c1 = val[1], c2 = val[2], c3 = val[3];
			val = "#" + string(1, c1) + string(1, c1) + string(1, c2) + string(1, c2) + string(1, c3) + string(1, c3);
		}

		istringstream(val.substr(1)) >> hex >> hexVal;
		co.red = (hexVal >> 16) & 0xFF;
		co.green = (hexVal >> 8) & 0xFF;
		co.blue = hexVal & 0xFF;
	}
	else {
		transform(val.begin(), val.end(), val.begin(), ::tolower);

		ifstream fin("color.txt");
		if (!fin.is_open()) {
			string line;
			while (getline(fin, line)) {
				stringstream ss(line);
				string buffer, name, rgb;
				getline(ss, buffer, '"');
				getline(ss, name, '"');
				if (name == val) {
					getline(ss, buffer, '(');
					ss >> co.red;
					ss >> buffer;
					ss >> co.red;
					ss >> buffer;
					ss >> co.blue;
					break;
				}
				else {
					getline(ss, buffer, '}');
				}
			}
		}
		fin.close();
	}
	return co;
}
vector<point> readPointers(string val) {
	vector<point> points;
	val = regex_replace(val, regex("^\\s+|\\s+$"), "");

	stringstream ss(val);
	string tmp = "";
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
			points.push_back(p);
		}
	}
	return points;
}

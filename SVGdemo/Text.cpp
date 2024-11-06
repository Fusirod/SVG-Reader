
#include "Shape.h"

VOID text::draw(Graphics& gra, List lst) {
    for (int i = 0; i < textFile.length(); i++) {
        if (textFile[i] == '\n') {
            textFile[i] = ' ';
        }
        if (textFile[i] == ' ' && textFile[i + 1] == ' ') {
            textFile.erase(i, 1);
            i--;
        }
    }
    float rate = 0;
    GraphicsState save = gra.Save();
    int index = 0;
    for (int i = 0; i < changes.types.size(); i++) {
        if (changes.types[i] == "translate")
            gra.TranslateTransform(changes.values[index], changes.values[index + 1]);
        if (changes.types[i] == "rotate")
            gra.RotateTransform(changes.values[index]);
        if (changes.types[i] == "scale")
            gra.ScaleTransform(changes.values[index], changes.values[index + 1]);
        index += 2;
        if (changes.types[i] == "rotate")
            index--;
    }
    wstring_convert<codecvt_utf8<wchar_t>> converter;

    wstring wFontFamily = converter.from_bytes(fontFamily);

    FontFamily fontFamily(wFontFamily.c_str());
    Font font(&fontFamily, static_cast<REAL>(fontSize), italic ? FontStyleItalic : FontStyleRegular, UnitPixel);

    StringFormat stringFormat; // Create a StringFormat object with the each line of text, and the block
    if (textAnchor == "middle") {
        stringFormat.SetAlignment(StringAlignmentCenter); // Center alignment
    }
    else if (textAnchor == "end") {
        stringFormat.SetAlignment(StringAlignmentFar); // Right and center alignment
        rate = 0.15;
    }
    else
    {
        stringFormat.SetAlignment(StringAlignmentNear); // Left and center alignment
        rate = -0.15;
    }
    PointF pointF(static_cast<REAL>(start.x + dx + rate * fontSize), static_cast<REAL>(start.y + dy - 0.9 * fontSize));
    GraphicsPath path;
    const wstring wstr = wstring(textFile.begin(), textFile.end());
    path.AddString(wstr.c_str(), -1, &fontFamily, italic ? FontStyleItalic : FontStyleRegular, static_cast<REAL>(fontSize), pointF, &stringFormat);
    SolidBrush fillBrush(Color(static_cast<int>(fillOpacity * 255), fillColor.red, fillColor.green, fillColor.blue));
    Pen pen(Color(static_cast<int>(strokeOpacity * 255), strokeColor.red, strokeColor.green, strokeColor.blue), static_cast<REAL>(strokeWidth));
    gra.FillPath(&fillBrush, &path);
    if (strokeWidth != 0)
        gra.DrawPath(&pen, &path);
    gra.Restore(save);
}
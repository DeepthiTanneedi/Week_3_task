/*ask 4: HMI Skin Customization System
Objective: Implement a system to manage different themes or skins for an HMI display.
Requirements:
Create a Theme class:
Attributes: backgroundColor, fontColor, fontSize, and iconStyle.
Implement methods to apply and display the theme.
Store multiple themes using std::map:
Example themes: Classic, Sport, and Eco.
Create a simple user interface to switch between themes:
Use console input to select a theme.
Display a preview of the selected theme on the console (e.g., "Sport Theme: Red Background, White Font").
Deliverables:
A C++ program that demonstrates theme switching.
Output displaying applied themes.*/
#include <iostream>
#include <map>
#include <string>

using namespace std;

class Theme {
    string backgroundColor;
    string fontColor;
    int fontSize;
    string iconStyle;

public:
    Theme() : backgroundColor(""), fontColor(""), fontSize(0), iconStyle("") {}

    Theme(string bg, string font, int size, string icon)
        : backgroundColor(bg), fontColor(font), fontSize(size), iconStyle(icon) {}

    void display() const {
        cout << backgroundColor << " Background, "
             << fontColor << " Font, Size " << fontSize << ", Icon Style: " << iconStyle << endl;
    }
};

int main() {
    map<string, Theme> themes;

    themes["Classic"] = Theme("Blue", "White", 14, "Simple");
    themes["Sport"] = Theme("Red", "White", 16, "Dynamic");
    themes["Eco"] = Theme("Green", "Black", 12, "Nature");

    string choice;
    cout << "Enter theme (Classic, Sport, Eco): ";
    cin >> choice;

    if (themes.find(choice) != themes.end()) {
        cout << choice << " Theme: ";
        themes[choice].display();
    } else {
        cout << "Theme not found!" << endl;
    }

    return 0;
}
/*Total Week 3 tasks. 
Task 1: Build a Menu Navigation System
Objective: Implement a basic menu navigation system similar to what is found in automotive HMI clusters (e.g., settings menu, media options).
Requirements:
Create a hierarchical menu structure using classes and data structures.
Example:
Main Menu
Settings
Display Settings
Audio Settings
Media
Radio
Bluetooth Audio
Use a tree structure (std::vector or custom tree data structure) to represent the hierarchy.
Implement the following functionalities:
Navigate through menu levels (up/down/enter/back).
Display the current menu options on the console.
Add keyboard-based interaction (e.g., 1 to navigate down, 2 to navigate up, 3 to enter).
Deliverables:
A C++ program that uses OOP to create the menu structure.
Console output demonstrating navigation and interactions.*/
#include <iostream>
#include <vector>
#include <memory>
#include <string>

class MenuItem {
public:
    std::string name;
    std::vector<std::shared_ptr<MenuItem>> children;
    MenuItem* parent;

    MenuItem(const std::string& name, MenuItem* parent = nullptr)
        : name(name), parent(parent) {}

    void addChild(const std::shared_ptr<MenuItem>& child) {
        children.push_back(child);
    }
};

class Menu {
private:
    std::shared_ptr<MenuItem> root;
    MenuItem* current;
    size_t currentIndex;

public:
    Menu() : root(std::make_shared<MenuItem>("Main Menu")), current(root.get()), currentIndex(0) {}

    void displayCurrentMenu() {
        std::cout << "\n=== " << current->name << " ===\n";
        for (size_t i = 0; i < current->children.size(); ++i) {
            if (i == currentIndex)
                std::cout << " > ";
            else
                std::cout << "   ";
            std::cout << current->children[i]->name << '\n';
        }
        std::cout << "\nOptions: \n1. Down \n2. Up \n3. Enter \n4. Back \n5. Exit\n";
    }

    void navigateDown() {
        if (!current->children.empty())
            currentIndex = (currentIndex + 1) % current->children.size();
    }

    void navigateUp() {
        if (!current->children.empty())
            currentIndex = (currentIndex == 0) ? current->children.size() - 1 : currentIndex - 1;
    }

    void enter() {
        if (!current->children.empty()) {
            current = current->children[currentIndex].get();
            currentIndex = 0;
        }
    }

    void back() {
        if (current->parent) {
            current = current->parent;
            currentIndex = 0;
        }
    }

    void addMenuItems() {
        // Build the menu structure
        auto settings = std::make_shared<MenuItem>("Settings", root.get());
        settings->addChild(std::make_shared<MenuItem>("Display Settings", settings.get()));
        settings->addChild(std::make_shared<MenuItem>("Audio Settings", settings.get()));
        root->addChild(settings);

        auto media = std::make_shared<MenuItem>("Media", root.get());
        media->addChild(std::make_shared<MenuItem>("Radio", media.get()));
        media->addChild(std::make_shared<MenuItem>("Bluetooth Audio", media.get()));
        root->addChild(media);
    }

    void run() {
        char choice;
        bool running = true;
        while (running) {
            displayCurrentMenu();
            std::cin >> choice;
            switch (choice) {
            case '1': navigateDown(); break;
            case '2': navigateUp(); break;
            case '3': enter(); break;
            case '4': back(); break;
            case '5': running = false; break;
            default: std::cout << "Invalid choice. Try again.\n"; break;
            }
        }
    }
};

int main() {
    Menu menu;
    menu.addMenuItems();
    menu.run();
    return 0;
}
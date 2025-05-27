#ifndef MENU_ITEM_H
#define MENU_ITEM_H

#include <string>
#include <vector>
#include <memory>

// Simple function pointer type for menu actions (without using std::function)
using MenuAction = void(*)();

// Represents a single menu item which may contain an action and/or a submenu
class MenuItem {
private:
    std::string label;  // The label text displayed for this menu item
    MenuAction action = nullptr;  // Optional action to execute when item is selected
    std::vector<std::shared_ptr<MenuItem>> submenu;  // Optional submenu items

public:
    // Constructor with label and optional action
    MenuItem(const std::string& label, MenuAction action = nullptr)
        : label(label), action(action) {}

    virtual ~MenuItem() = default;

    // Returns the label of this menu item
    const std::string& getLabel() const {
        return label;
    }

    // Sets the submenu items for this menu item
    void setSubmenu(const std::vector<std::shared_ptr<MenuItem>>& items) {
        submenu = items;
    }

    // Adds a single submenu item to this menu item
    void addSubmenuItem(const std::shared_ptr<MenuItem>& item) {
        submenu.push_back(item);
    }

    // Checks whether this menu item has a submenu
    bool hasSubmenu() const {
        return !submenu.empty();
    }

    // Returns the submenu associated with this item
    const std::vector<std::shared_ptr<MenuItem>>& getSubmenu() const {
        return submenu;
    }

    // Executes the assigned action if any
    void activate() const {
        if (action) action();
    }
};

#endif

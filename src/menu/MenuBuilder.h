#ifndef MENU_BUILDER_H
#define MENU_BUILDER_H

#include "MenuItem.h"
#include <memory>


// Utility class to simplify the creation of menu items and submenus
class MenuBuilder {
public:
    // Creates a simple menu item with a label and optional action callback
    static std::shared_ptr<MenuItem> createItem(const std::string& label, MenuAction action = nullptr) {
        return std::make_shared<MenuItem>(label, action);
    }

    // Creates a menu item that acts as a parent for a submenu
    static std::shared_ptr<MenuItem> createMenu(const std::string& label, const std::vector<std::shared_ptr<MenuItem>>& submenu) {
        auto item = std::make_shared<MenuItem>(label); // Create a menu item with a label
        item->setSubmenu(submenu);                     // Attach the submenu to this item
        return item;
    }
};

#endif // MENU_BUILDER_H

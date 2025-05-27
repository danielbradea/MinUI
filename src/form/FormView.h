#ifndef FORM_VIEW_H
#define FORM_VIEW_H


#include "../display/DisplayInterface.h"
#include "../button/ButtonManager.h"
#include "FormElement.h"
#include <memory>
#include <vector>

class FormView {
private:
    DisplayInterface& display;
    std::vector<std::shared_ptr<FormElement>> elements;
    size_t currentElement = 0;

public:
      FormView(DisplayInterface& disp) : display(disp) {}

    void addElement(const std::shared_ptr<FormElement>& element);
    void draw();
    void handleInput(ButtonEvent buttonEvent);
};

#endif
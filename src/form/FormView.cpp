#include "FormView.h"
#include "FormElement.h"
#include <memory>
#include <vector>
#include <string>

void FormView::addElement(const std::shared_ptr<FormElement> &element)
{
    if (element)
    { // Check for null pointer
        elements.push_back(element);
    }
}

void FormView::draw()
{
    int h = 0;
    for (size_t i = 0; i < elements.size(); ++i)
    {
        elements[i]->setSelected(i == currentElement);
        elements[i]->draw(display, 0, 0 + h);
        h += elements[i]->getHeight();
        h += 5;
    }
}

void FormView::handleInput(ButtonEvent buttonEvent)
{
    if (elements.empty())
        return;

    if (currentElement && elements[currentElement]->getEditing())
    {
        elements[currentElement]->handleInput(buttonEvent);
        return;
    }

    if (buttonEvent.buttonName == "UP")
    {
        if (buttonEvent.action == ButtonAction::SHORT_CLICK && currentElement > 0)
        {
            currentElement--;
        }
    }
    else if (buttonEvent.buttonName == "DOWN")
    {
        if (buttonEvent.action == ButtonAction::SHORT_CLICK && currentElement < elements.size() - 1)
        {
            currentElement++;
        }
        
    }
    else if (buttonEvent.buttonName == "CENTER")
    {
         if (buttonEvent.action == ButtonAction::SHORT_CLICK)
        {
            if (elements[currentElement]->canEdit())
            {
                elements[currentElement]->setEditing(true);
            }
        }
    }
    else if (buttonEvent.buttonName == "LEFT")
    {
    }
    else if (buttonEvent.buttonName == "RIGHT")
    {
    }
}

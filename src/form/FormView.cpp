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

    // Dacă un element este în mod editare, trimitem toate evenimentele către el
    if (elements[currentElement]->getEditing())
    {
        bool eventHandled = elements[currentElement]->handleInput(buttonEvent);
        if (eventHandled) return; // Evenimentul a fost consumat
    }

    // Navigare între elemente doar dacă niciun element nu este în mod editare
    if (buttonEvent.action == ButtonAction::SHORT_CLICK)
    {
        if (buttonEvent.buttonName == "UP" && currentElement > 0)
        {
            currentElement--;
        }
        else if (buttonEvent.buttonName == "DOWN" && currentElement < elements.size() - 1)
        {
            currentElement++;
        }
        else if (buttonEvent.buttonName == "CENTER")
        {
            if (elements[currentElement]->canEdit())
            {
                elements[currentElement]->setEditing(true);
            }
        }
    }
}

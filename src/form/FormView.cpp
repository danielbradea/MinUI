#include "FormView.h"
#include "FormElement.h"

// Constructor initializes the display reference
FormView::FormView(DisplayInterface& disp) : display(disp) {}

// Adds a new form element to the view, if not null
void FormView::addElement(const std::shared_ptr<FormElement>& element) {
    if (element) {
        elements.push_back(element);
    }
}

// Returns the visible width of the display, accounting for offsets
int FormView::getVisibleWidth() const {
    return display.width() - offsetX - reduceWidth;
}

// Returns the visible height of the display, accounting for offsets
int FormView::getVisibleHeight() const {
    return display.height() - offsetY - reduceHeight;
}

// Draws the form elements that are currently visible on screen
void FormView::draw() {
    if (elements.empty()) return;

    const int spacing = 5;
    const int visibleHeight = getVisibleHeight();

    // Începem de la elementul curent și încercăm să ne întoarcem cât putem în sus
    size_t startIndex = currentElement;
    int totalHeight = elements[currentElement]->getHeight();

    // Extindem în sus cât ne permite spațiul
    while (startIndex > 0) {
        int h = elements[startIndex - 1]->getHeight() + spacing;
        if (totalHeight + h > visibleHeight) break;
        totalHeight += h;
        startIndex--;
    }

    // Acum desenăm în jos cât încape
    int yPos = offsetY;
    for (size_t i = startIndex; i < elements.size(); ++i) {
        int elemHeight = elements[i]->getHeight();
        if (yPos + elemHeight > offsetY + visibleHeight) break;

        elements[i]->setSelected(i == currentElement);
        elements[i]->draw(display, offsetX, yPos, getVisibleWidth());

        yPos += elemHeight + spacing;
    }
}




// Handles input events like UP, DOWN, and CENTER button presses
void FormView::handleInput(ButtonEvent buttonEvent) {
    if (elements.empty()) return;

    // If the current element is being edited, delegate input to it
    if (elements[currentElement]->getEditing()) {
        bool handled = elements[currentElement]->handleInput(buttonEvent);
        if (handled) return;
    }

    // Navigation or selection input
    if (buttonEvent.action == ButtonAction::SHORT_CLICK) {
        if (buttonEvent.buttonName == "UP" && currentElement > 0) {
            currentElement--; // Move selection up
        } else if (buttonEvent.buttonName == "DOWN" && currentElement < elements.size() - 1) {
            currentElement++; // Move selection down
        } else if (buttonEvent.buttonName == "CENTER") {
            // Start editing if the selected element is editable
            if (elements[currentElement]->canEdit()) {
                elements[currentElement]->setEditing(true);
            }else{
                bool handled = elements[currentElement]->handleInput(buttonEvent);
                if (handled) return;
            }
        }
    }
}

// Set horizontal offset for rendering
void FormView::setOffsetX(int x) {
    offsetX = x;
}

// Set vertical offset for rendering
void FormView::setOffsetY(int y) {
    offsetY = y;
}

// Reduce the visible width by a number of pixels
void FormView::reduceVisibleWidth(int pixels) {
    reduceWidth = pixels;
}

// Reduce the visible height by a number of pixels
void FormView::reduceVisibleHeight(int pixels) {
    reduceHeight = pixels;
}

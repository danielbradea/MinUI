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
    int yPos = offsetY;

    // Calculate the total height used from the current element upward
    int usedHeight = 0;
    size_t startIndex = currentElement;

    // Try to include as many elements above the current one as possible to fit in view
    while (startIndex > 0) {
        int elemHeight = elements[startIndex - 1]->getHeight() + spacing;
        if (usedHeight + elemHeight > visibleHeight) break;
        usedHeight += elemHeight;
        startIndex--;
    }

    // Draw elements starting from the calculated index downward
    for (size_t i = startIndex; i < elements.size(); ++i) {
        int elemHeight = elements[i]->getHeight();
        if (yPos + elemHeight > offsetY + visibleHeight) break;

        elements[i]->setSelected(i == currentElement); // Highlight current element
        elements[i]->draw(display, offsetX, yPos, getVisibleWidth()); // Draw element
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

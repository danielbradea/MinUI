#ifndef FORM_VIEW_H
#define FORM_VIEW_H

#include "../display/DisplayInterface.h"
#include "../button/ButtonManager.h"
#include "FormElement.h"
#include <memory>
#include <vector>

// The FormView class is responsible for displaying and interacting with a list of FormElements.
// It manages layout, input handling, and rendering within a specified visible area.
class FormView {
private:
    DisplayInterface& display; // Reference to the display interface used to draw elements
    std::vector<std::shared_ptr<FormElement>> elements; // List of form elements to be displayed
    size_t currentElement = 0; // Index of the currently selected element

    int offsetX = 0;        // Horizontal offset for drawing elements
    int offsetY = 0;        // Vertical offset for drawing elements
    int reduceWidth = 0;    // Pixels to subtract from visible width
    int reduceHeight = 0;   // Pixels to subtract from visible height

    // Calculates the visible width of the form view
    int getVisibleWidth() const;

    // Calculates the visible height of the form view
    int getVisibleHeight() const;

public:
    // Constructor that initializes the form view with a display reference
    FormView(DisplayInterface& disp);

    // Adds a new form element to the view
    void addElement(const std::shared_ptr<FormElement>& element);

    // Renders the form elements currently visible on the screen
    void draw();

    // Handles button input events (e.g., navigation and editing)
    void handleInput(ButtonEvent buttonEvent);

    // Sets the horizontal offset for rendering
    void setOffsetX(int x);

    // Sets the vertical offset for rendering
    void setOffsetY(int y);

    // Reduces the visible width by a specified number of pixels
    void reduceVisibleWidth(int pixels);

    // Reduces the visible height by a specified number of pixels
    void reduceVisibleHeight(int pixels);
};

#endif

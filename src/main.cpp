#include "display/DisplaySH1106G.h"
#include "button/ButtonManager.h"
#include "form/FormView.h"
#include "form/TextInputElement.h"
#include "form/CheckBoxElement.h"
#include "form/ListElement.h"
#include "form/ButtonElement.h"
#include <memory>
#include <Wire.h>
#include <map>
#include <string>

std::map<String, uint8_t> buttonConfig = {
    {"UP", 4},
    {"DOWN", 5},
    {"CENTER", 15},
    {"RIGHT", 16},
    {"LEFT", 17}};

ButtonManager btnManager(buttonConfig);

DisplaySH1106G oled(128, 64, -1);
FormView formW(oled);
void saveAction(String label) {
    Serial.println("Pressed: " + label);
}

void setup()
{
  Serial.begin(115200);
  delay(2000);
  Wire.begin(8, 9);

  btnManager.begin();
  oled.begin();

  //oled.setRotation(1);

  auto username = std::make_shared<TextInputElement>("Username");
  auto password = std::make_shared<CheckBoxElement>("Asddasd sdf");
  std::vector<String> colors = {"Red", "Green", "Blue", "Yellow Green"};
  auto colorEl = std::make_shared<ListElement>("Color",colors);

  auto buttonSave = std::make_shared<ButtonElement>("Save",saveAction);

  formW.addElement(username);
  formW.addElement(password);
  formW.addElement(colorEl);
  formW.addElement(buttonSave);

}

void loop()
{
  oled.clearDisplay();
  btnManager.update();
  formW.draw();
  oled.display();

  ButtonEvent event = btnManager.getAction();

  if (event.action != NO_ACTION)
  {
    formW.handleInput(event);
  }
}

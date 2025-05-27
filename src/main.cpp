#include "display/DisplaySH1106G.h"
#include "button/ButtonManager.h"
#include "form/FormView.h"
#include "form/TextInputElement.h"
#include <memory>
#include <Wire.h>
#include <map>
#include <string>

std::map<String, uint8_t> buttonConfig = {
    {"UP", 4},
    {"DOWN", 5},
    {"CENTER", 15},
    {"RIGHT", 16},
    {"SELECT", 17}};

ButtonManager btnManager(buttonConfig);

DisplaySH1106G oled(128, 64, -1);
FormView formW(oled);

void setup()
{
  Serial.begin(115200);
  delay(2000);
  Wire.begin(8, 9);

  btnManager.begin();
  oled.begin();

  oled.setRotation(1);

  auto username = std::make_shared<TextInputElement>("Username");
  auto password = std::make_shared<TextInputElement>("Password");

  formW.addElement(username);
  formW.addElement(password);
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
    // Afișează evenimentul
    Serial.print("Buton: ");
    Serial.print(event.buttonName);
    Serial.print(" (Pin ");
    Serial.print(event.buttonPin);
    Serial.print(") - Acțiune: ");

    switch (event.action)
    {
    case SHORT_CLICK:
      Serial.println("CLICK SCURT");
      // Adaugă aici acțiunea pentru click scurt
      break;

    case LONG_PRESS:
      Serial.println("LONG PRESS");
      // Adaugă aici acțiunea pentru long press
      break;

    case DOUBLE_CLICK:
      Serial.println("DUBLU CLICK");
      // Adaugă aici acțiunea pentru dublu click
      break;
    }
  }
}

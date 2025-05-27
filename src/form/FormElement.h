#ifndef FORM_ELEMENT_H
#define FORM_ELEMENT_H

#include "../display/DisplayInterface.h"
#include "../button/ButtonManager.h"

class FormElement
{
public:
  virtual ~FormElement() = default; // Asigură ștergerea corectă a obiectelor derivate
  
  // Desenează elementul pe ecran la poziția (x,y)
  virtual void draw(DisplayInterface &display, int x, int y) = 0;
  
  // Gestionează evenimentele de intrare
  // Returnează true dacă evenimentul a fost procesat
  virtual bool handleInput(ButtonEvent buttonEvent) = 0;
  
  // Returnează înălțimea elementului în pixeli
  virtual int getHeight() = 0;
  
  // Indicator dacă elementul poate intra în modul editare
  // Implicit false pentru majoritatea elementelor
  virtual bool canEdit() { return false; }
  
  // Activează/dezactivează modul editare
  // Parametrul editing: true pentru intrare în mod editare, false pentru ieșire
  virtual void setEditing(bool editing) {}

  virtual bool getEditing();
  virtual void setSelected(bool selected);
};

#endif
//LightOff.h
#ifndef LIGHTOFF_H
#define LIGHTOFF_H

#include "Light.h"

class TurnOff: public Node
{
 public:
  TurnOff(Light *L);
  void Render();

 private:
  GLenum LightName;
  
};

inline TurnOff::TurnOff(Light *L)
{
  LightName=L->LightName;
}

inline void
TurnOff::Render()
{
  glDisable(GL_LIGHTING);
  glDisable(LightName);
}



#endif

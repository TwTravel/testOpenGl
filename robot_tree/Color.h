//Color.h

#ifndef COLOR_H
#define COLOR_H

#include "Node.h"

class Color: public Node
{
 public:
  Color();
  void SetValuev(float *v);
  void SetValue(float v1, float v2, float v3);
  void SetValue(Enum Type);
  void Render();

 private:
  bool Changed;
  float Color3f[3];
};

inline Color::Color()
{
}

inline  void 
Color::SetValue(Enum Type)
{
  Changed=true;
  GetColor(Type, Color3f);
}

inline  void
Color::SetValuev(float *v)
{
  int i;

  Changed=true;
  for(i=0; i<3; i++)
    Color3f[i]=v[i];
}

inline void
Color::SetValue(float v1, float v2, float v3)
{
  Changed=true;
  Color3f[0]=v1;
  Color3f[1]=v2;
  Color3f[2]=v3;
}

inline void 
Color::Render()
{
  if(Changed)
    glColor3fv(Color3f);
}


#endif

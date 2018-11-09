//Material.h
#ifndef MAT_H
#define MAT_H

#include "Node.h"

class Material: public Node
{
 public:
  Material();
  void SetValuev(Enum Pname, float *);
  void SetValue(Enum Pname, float v1, float v2, float v3, float v4);
  void SetValue(Enum Pname, float Value);
  void Render();

 private:
  bool Changed[5];
  float Ambient[4];
  float Diffuse[4];
  float Specular[4];
  float Emission[4];
  float Shininess;
};


inline Material::Material()
{
  int i;

  for(i=0; i<5; i++)
    {
      Changed[i]=false;
    }
}

inline void 
Material::SetValuev(Enum Pname, float Value[])
{
  float *temp=NULL;
  int i;

  switch(Pname)
    {
    case AMBIENT:
      temp=Ambient;
      Changed[0]=true;
      break;
    case DIFFUSE:
      temp=Diffuse;
      Changed[1]=true;
      break;
    case SPECULAR:
      temp=Specular;
      Changed[2]=true;
      break;
    case EMISSION:
      temp=Emission;
      Changed[3]=true;
      break;
    default:
      break;
    }
  
  for(i=0; i<4; i++)
    temp[i]=Value[i];
}

inline void 
Material::SetValue(Enum Pname, float v1, float v2, float v3, float v4)
{
  float temp[]={v1, v2, v3, v4};
  
  SetValuev(Pname, temp);
}

inline void
Material::SetValue(Enum Pname, float Value)
{
  if(Pname==SHININESS)
    {
      Changed[4]=true;
      Shininess=Value;
    }
}

inline void
Material::Render()
{
  if(Changed[0])
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Ambient);
  if(Changed[1])
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Diffuse);
  if(Changed[2])
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Specular);
  if(Changed[3])
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Emission);
  if(Changed[4])
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Shininess);
}

#endif

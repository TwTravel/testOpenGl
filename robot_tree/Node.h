//Node.h 
#include <GL/glut.h>
#include <iostream>

#ifndef NODE_H
#define NODE_H

enum Enum
{
  PERSPECTIVE, ORTHO, POSITION, AIMAT, UPDIRECTION, ASPECT,
  NEAR, FAR, YANGLE,
  BLACK, WHITE, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, GREY,
  WIDTH, HEIGHT,DEPTH,
  AMBIENT, DIFFUSE, SPECULAR, SPOT_DIRECTION, DROPOFFRATE,
  CUTOFFANGLE, EMISSION, SHININESS,
  TRANSLATION, ROTATION, SCALE,
  BUFFER, SINGLE, DOUBLE,
  RADIUS,
  STYLE, POINTSIZE, LINEWIDTH, FILLED, LINE, POINT,
  BACKCOLOR
};

class Node
{
 public:
  Node();
  virtual ~Node();
  virtual void Render();
  void AddChild(Node *);

 protected:
  void GetColor(Enum, float *);
  bool KeepMatrix; 

 private:
  void Traverse();
  Node *LeftChild;
  Node *RightSibling;

  friend class GLViewer;
};

inline void 
Node::Traverse()
{
  if(!KeepMatrix)
    glPushMatrix();
 
  Render();
  if(LeftChild!=NULL)
    LeftChild->Traverse();
  if(!KeepMatrix)
    glPopMatrix();
  if(RightSibling!=NULL)
    RightSibling->Traverse();
}

inline void
Node::AddChild(Node *node)
{
  if(LeftChild==NULL)
    LeftChild=node;
  else
    {
      node->RightSibling=LeftChild;
      LeftChild=node;
    }
}

inline Node::Node()
{
  KeepMatrix=false;
  LeftChild=NULL;
  RightSibling=NULL;
}

inline Node::~Node()
{
  if(LeftChild)
    delete LeftChild;
  LeftChild=NULL;
  if(RightSibling)
    delete RightSibling;
  RightSibling=NULL;
}

inline void
Node::GetColor(Enum Color, float *C)
{
  int i;

  switch(Color)
    {
    case BLACK:
      for(i=0; i<3; i++)
	C[i]=0.0;
      break;
    case WHITE:
      for(i=0; i<3; i++)
	C[i]=1.0;
      break;
    case RED:
      C[0]=1.0;
      C[1]=0.0;
      C[2]=0.0;
      break;
    case GREEN:
      C[0]=0.0;
      C[1]=1.0;
      C[2]=0.0;
      break;
    case YELLOW:
      C[0]=1.0;
      C[1]=1.0;
      C[2]=0.0;
      break;
    case BLUE:
      C[0]=0.0;
      C[1]=0.0;
      C[2]=1.0;
      break;
    case MAGENTA:
      C[0]=1.0;
      C[1]=0.0;
      C[2]=1.0;
      break;
    case CYAN:
      C[0]=0.0;
      C[1]=1.0;
      C[2]=1.0;
      break;
    case GREY:
      C[0]=0.5;
      C[1]=0.5;
      C[2]=0.5;
      break;

    default:
      break;
    }
}


inline void
Node::Render()
{
}


#endif  

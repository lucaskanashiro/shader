#ifndef VEC_H
#define VEC_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <string>
#include <vector>

using namespace std;

struct Vertex
{
  float position[3];
  int color[4];

  Vertex(float x=0, float y=0, float z=0, int r=0, int g=0, int b=0, int a=255)
  {
    position[0] = x;
    position[1] = y;
    position[2] = z;

    color[0] = r;
    color[1] = g;
    color[2] = b;
    color[3] = a;
  }

  void drawGl()
  {
    drawGlColor3d();
    drawGlVertex3f();
  }

  void drawGlColor3d()
  {
    glColor3d(color[0], color[1], color[2]);
  }

  void drawGlVertex3f()
  {
    glVertex3f(position[0], position[1], position[2]);
  }

};

struct Vertex2D
{
  float position[2];

  Vertex2D(float x=0, float y=0)
  {
    position[0] = x;
    position[1] = y;
  }
};

struct Face
{
  int indice[3][3];

  Face(){};

  void setIndice(int i, int indiceVertice, int indiceNormal, int indiceTexture)
  {
    indice[i][0] = indiceVertice;
    indice[i][1] = indiceNormal;
    indice[i][2] = indiceTexture;
  }
  
};

struct Material
{
  int illum, imageID, imageWidth, imageHeight;
  GLuint textureID;
  unsigned char* imageData;
  string name;
  string fileName;
  float ns, ni, d, tr, tf[3];
  float ka[3], kd[3], ks[3], ke[3];

  Material() {};
};

struct Shader
{
  const char* filename;
  GLenum type;
  GLchar* source;
  GLuint a;

  Shader(){}

  Shader(const char* _filename, GLenum _type, GLchar* _source)
  {
    filename = _filename;
    type = _type;
    source = _source;
  }

};

#endif


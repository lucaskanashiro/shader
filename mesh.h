#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <Magick++.h>

#include "vec.h"

using namespace std;
using namespace Magick;

class Mesh
{
  public:

    Mesh();
    ~Mesh();

    void carregarArquivo(string nomeArquivo);
    void redimensionar();
    void normalizar();
    void transladar();
	Vertex calcularNormal(Vertex a, Vertex b, Vertex c);
    vector<Vertex> getVertex();
    vector<unsigned int> getIndice();

    void draw(GLuint program);

	float getZMax();
    void setZMax(float z);		    

    void criarVertexArray();
    void criarBufferDeVertex();
    void criarBufferDeIndex();

    void criarBufferTexture();

    void resize(float scalar);
    void resizePoints(Vertex &head, Vertex &tail, float scalar);

    void rotateY(float angleY);
    void rotateX(float angleX);
    void rotateZ(float angleZ);
    void incPosition(float x, float y, float z);

  private:

    void carregarMaterial(string nomeArquivo);
    vector< MyColorRGB > getColorRGB(string nomeArquivo);

    string getPath(string fileName);

    float encontrarDeltaX();
    float encontrarDeltaY();
    float encontrarDeltaZ();

    void freeBuffers();

    int loadImage(string imageName);

    GLuint VBO, IBO, vertexArrayID, textureArrayID;
    float deltaX, deltaY, deltaZ, xMin, yMin, zMin;
    float zMax;
    float xMid, yMid, zMid;
    vector<Vertex> vertex;
    vector<Vertex> normal;
    vector<Vertex> texture;
    vector<unsigned int> vertexIndex;
    vector<unsigned int> normalIndex;
    vector<unsigned int> textureIndex;
    GLfloat angleX, angleY, angleZ;

    string fileMtl;
    vector<Material> material;
};

#endif


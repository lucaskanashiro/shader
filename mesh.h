#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>

#include "vec.h"

using namespace std;

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

    Vertex getMidPoint();

    void criarVertexArray();
    void criarBufferDeVertex();
    void criarBufferDeIndex();

    void criarBufferTexture();
    void criarBufferTextureIndex();

    void resize(float scalar);
    void resizePoints(Vertex &head, Vertex &tail, float scalar);

    void rotateY(float angleY);
    void rotateX(float angleX);
    void rotateZ(float angleZ);

    void setPosition(float x = 0, float y = 0, float z = 0);
    void incPosition(float x = 0, float y = 0, float z = 0);

    float getDeltaX();
    float getDeltaY();
    float getDeltaZ();

    void prepareAllTexture();

    void showPosition();
    void showDimensions();
    void showAngles();

  private:

    void carregarMaterial(string nomeArquivo);

    string getPath(string fileName);

    float encontrarDeltaX();
    float encontrarDeltaY();
    float encontrarDeltaZ();

    void freeBuffers();

    int loadImage(string imageName);
    void prepareTexture(Material &material);

    GLuint VBO, IBO, vertexArrayID, textureArrayID, textureIndexArrayID;
    float deltaX, deltaY, deltaZ, xMin, yMin, zMin;
    float zMax;
    float xMid, yMid, zMid;
    vector<Vertex> vertex;
    vector<Vertex> normal;
    vector<Vertex2D> texture;
    vector<unsigned int> vertexIndex;
    vector<unsigned int> normalIndex;
    vector<unsigned int> textureIndex;
    GLfloat angleX, angleY, angleZ;

    string fileMtl;
    vector<Material> material;
};

#endif


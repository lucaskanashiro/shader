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

    void criarVertexArray();
    void criarBufferDeVertex();
    void criarBufferDeIndex();

    void resize(float scalar);
    void resizePoints(Vertex &head, Vertex &tail, float scalar);

    void rotateY(float angleY);
    void rotateX(float angleX);
    void rotateZ(float angleZ);
    void incPosition(float x, float y, float z);

  private:

    float encontrarDeltaX();
    float encontrarDeltaY();
    float encontrarDeltaZ();

    void freeBuffers();

    GLuint VBO, IBO, vertexArrayID;
    float deltaX, deltaY, deltaZ, xMin, yMin, zMin;
    float zMax;
    int qtdPontos, qtdArestas, qtdTextura;
    vector<Vertex> vertex;
    vector<unsigned int> indice;
    GLfloat angleX, angleY, angleZ;
};

#endif


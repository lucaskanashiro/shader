#ifndef _INIT_SHADER
#define _INIT_SHADER

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "vec.h"
#include "mesh.h"

using namespace std;

class GerenciadorShader
{
  public: 
    
    GerenciadorShader();

    void setVertexShader(const char* vertex);
    void setFragmentShader(const char* fragment);
    void setVertex(vector<Vertex> vertex);
    void setIndice(vector<unsigned int> indice);

    GLuint initShader();
    void renderizarCena(GLuint program);

    void addMesh(Mesh *mesh);

  private:

    char* readShaderSource(const char* shaderFile);

    Shader shaders[2];
    GLuint VBO, IBO, vertexArrayID;
    vector<Vertex> buffer;
    vector<unsigned int> indice;

    vector<Mesh *> vectorMesh;
};

#endif

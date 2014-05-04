#ifndef _INIT_SHADER
#define _INIT_SHADER

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "vec.h"

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
    void criarBufferDeVertex();
    void criarBufferDeIndex();
    void renderizarCena();

  private:

    char* readShaderSource(const char* shaderFile);

    Shader shaders[2];
    GLuint VBO, IBO;
    vector<Vertex> buffer;
    vector<unsigned int> indice;
};

#endif

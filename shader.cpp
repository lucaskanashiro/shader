#include "shader.h"
#include <iostream>
using namespace std;

GerenciadorShader::GerenciadorShader(){}

void
GerenciadorShader::setVertexShader(const char* vertex)
{
  this->shaders[0] = Shader(vertex, GL_VERTEX_SHADER, NULL);
}

void
GerenciadorShader::setFragmentShader(const char* fragment)
{
  this->shaders[1] = Shader(fragment, GL_FRAGMENT_SHADER, NULL);
}

void
GerenciadorShader::setVertex(vector<Vertex> vertex)
{
  this->buffer = vertex;
}

void
GerenciadorShader::setIndice(vector<unsigned int> indice)
{
  this->indice = indice;
}

char*
GerenciadorShader::readShaderSource(const char* shaderFile)
{
    FILE* fp = fopen(shaderFile, "r");

    if ( fp == NULL ) 
    {
      printf("\n Nao abriu arquivo");
      return NULL; 
    }
	
    fseek(fp, 0L, SEEK_END);	
    long size = ftell(fp);

    fseek(fp, 0L, SEEK_SET);
    char* buf = new char[size + 1];
    fread(buf, 1, size, fp);

    buf[size] = '\0';
    fclose(fp);

    return buf;
}

GLuint
GerenciadorShader::initShader()
{
  GLuint program = glCreateProgram();
  cout << "DENTRO DO INIT" << endl;
    
  for ( int i = 0; i < 2; ++i ) 
  {
    Shader &s = this->shaders[i];
    s.source = readShaderSource(s.filename);

    if (shaders[i].source == NULL) 
    {
	    printf("\nFailed to read %s ", s.filename);
	    exit(EXIT_FAILURE);
    }

    GLuint shader = glCreateShader(s.type);
	  glShaderSource(shader, 1, (const GLchar**) &s.source, NULL);
	  glCompileShader(shader);

    GLint  compiled;
	  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	  if (!compiled) 
    {
      printf("\n \n failed to compile: %s", s.filename);
		  GLint  logSize;
	    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
	    char* logMsg = new char[logSize];
	    glGetShaderInfoLog(shader, logSize, NULL, logMsg);
		
		  printf("\n \n logSize %i failed to compile: %s",logSize, logMsg);
		  scanf("%i",&i);
		  exit(EXIT_FAILURE);
	  }

	  glAttachShader(program, shader);
  }

  glLinkProgram(program);

  GLint  linked;
  glGetProgramiv(program, GL_LINK_STATUS, &linked);

  if (!linked) 
  {
	  GLint  logSize;
	  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
	  char* logMsg = new char[logSize];
	  glGetProgramInfoLog(program, logSize, NULL, logMsg);

	  exit( EXIT_FAILURE );
  }

  glUseProgram(program);

  return program;
}

void
GerenciadorShader::criarBufferDeVertex()
{
  glGenBuffers(1, &this->VBO);
  glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
  glBufferData(GL_ARRAY_BUFFER, this->buffer.size()*sizeof(Vertex), &this->buffer[0], GL_STATIC_DRAW);
}

void
GerenciadorShader::criarBufferDeIndex()
{
  glGenBuffers(1, &this->IBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indice.size()*sizeof(unsigned int), &this->indice[0], GL_STATIC_DRAW);
}

void
GerenciadorShader::renderizarCena()
{
  glClear(GL_COLOR_BUFFER_BIT);

  glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) 0);
  glVertexAttribPointer(1, 4, GL_INT, GL_TRUE, sizeof(Vertex), (const GLvoid*) 3);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glDrawElements(GL_TRIANGLES, this->indice.size()*sizeof(unsigned int), GL_UNSIGNED_INT, (const GLvoid*) 0);

  //glDisableVertexAttribArray(0);
  //glDisableVertexAttribArray(1);
}


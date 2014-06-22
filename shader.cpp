#include "shader.h"
#include <iostream>
#include <algorithm>
using namespace std;

bool compareMesh(Mesh a, Mesh b)
{
  return a.getZMax() < b.getZMax();
}

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
GerenciadorShader::renderizarCena()
{
  glClear(GL_COLOR_BUFFER_BIT);


  for(int i = 0; i < (int)this->vectorMesh.size(); i++)
  {
    this->vectorMesh[i].draw();
  }

  // glDrawElements(GL_TRIANGLES, this->indice.size()*sizeof(unsigned int), GL_UNSIGNED_INT, (const GLvoid*) 0);

}

void
GerenciadorShader::addMesh(Mesh mesh)
{
  this->vectorMesh.push_back(mesh);
  sort(this->vectorMesh.begin(), this->vectorMesh.end(), compareMesh);
}
#include "mesh.h"
#include <cmath>

Mesh::Mesh()
{
  this->angleX = 0.0f;
  this->angleY = 0.0f;
  this->angleZ = 0.0f;
}

Mesh::~Mesh(){}

float
Mesh::encontrarDeltaX()
{
  float xMax = vertex[0].position[0];
  float xMin = vertex[0].position[0];

  for(unsigned int i=1; i<this->vertex.size(); i++)
  {
    if(vertex[i].position[0] > xMax) 
      xMax = vertex[i].position[0];

    if(vertex[i].position[0] < xMin) 
      xMin = vertex[i].position[0];
  }

  this->xMin = xMin;

  this->xMid = (xMax + xMin) / 2;

  return xMax - xMin;
}

float
Mesh::encontrarDeltaY()
{
  float yMax = vertex[0].position[1];
  float yMin = vertex[0].position[1];

  for(unsigned int i=1; i<this->vertex.size(); i++)
  {
    if(vertex[i].position[1] > yMax) 
      yMax = vertex[i].position[1];

    if(vertex[i].position[1] < yMin) 
      yMin = vertex[i].position[1];
  }

  this->yMin = yMin;

  this->yMid = (yMax + yMin) / 2;  

  return yMax - yMin;
}

float
Mesh::encontrarDeltaZ()
{
  float zMax = vertex[0].position[2];
  float zMin = vertex[0].position[2];

  for(unsigned int i=1; i<this->vertex.size(); i++)
  {
    if(vertex[i].position[2] > zMax) 
      zMax = vertex[i].position[2];

    if(vertex[i].position[2] < zMin) 
      zMin = vertex[i].position[2];
  }

  this->zMin = zMin;
  this->zMax = zMax;

  this->zMid = (zMax + zMin) / 2;

  return zMax - zMin;
}

float
Mesh::getZMax()
{
  return this->zMax;
}

void
Mesh::setZMax(float z)
{
  this->zMax = z;
}

void
Mesh::carregarArquivo(string nomeArquivo)
{
  ifstream arquivo(nomeArquivo.c_str());

  if(!arquivo.is_open())
  {
    cout << "Não foi possível abrir o aquivo!!!" << endl;
    exit(-1);
  }

  string tipo;
  string line;
  string trash;

  float x, y, z;

  while(getline(arquivo, line))
  {
    stringstream stream(line);

    if(line.substr(0, 6) == "mtllib")
    {
      stream >> trash >> this->fileMtl;
      cout << "fileMtl: " << this->fileMtl << endl;
    }

    else if(line.substr(0, 2) == "v ")
    {
      stream >> trash >> x >> y >> z;
      this->vertex.push_back(Vertex(x, y, z));
    }

    else if(line.substr(0,2) == "vn")
    {
      stream >> trash >> x >> y >> z;
      this->normal.push_back(Vertex(x, y, z));
    }

    else if(line.substr(0,2) == "vt")
    {
      stream >> trash >> x >> y >> z;
      this->texture.push_back(Vertex(x, y, z));
    }

    else if(line.substr(0,2) == "f ")
    {
      unsigned int v, n, t;
      char c_trash;

      stream >> trash;
      for(int i = 0; i < 3; i++)
      {
        stream >> v >> c_trash >> n >> c_trash >> t;
        this->vertexIndex.push_back(v);
        this->normalIndex.push_back(n);
        this->textureIndex.push_back(t);
      }
    }
  }

  for(unsigned int i=0; i<this->vertexIndex.size(); i++)
  {
    cout << this->vertexIndex[i] << endl;
  }

  arquivo.close();

  this->deltaX = this->encontrarDeltaX();
  this->deltaY = this->encontrarDeltaY();
  this->deltaZ = this->encontrarDeltaZ();
}

void
Mesh::redimensionar()
{
  this->resize(1/this->deltaY);

  this->deltaX = this->encontrarDeltaX();
  this->deltaY = this->encontrarDeltaY();
  this->deltaZ = this->encontrarDeltaZ();
}

void 
Mesh::transladar()
{
  for(unsigned int i=0; i<this->vertex.size(); i++)
  {
    vertex[i].position[0] -= ((this->deltaX/2)+this->xMin);
    vertex[i].position[1] -= ((this->deltaY/2)+this->yMin);
    vertex[i].position[2] -= ((this->deltaZ/2)+this->zMin);
  }
}

void
Mesh::criarVertexArray()
{
  glGenVertexArrays(1, &this->vertexArrayID); 
  glBindVertexArray(vertexArrayID);
}

void
Mesh::criarBufferDeVertex()
{
  glGenBuffers(1, &this->VBO);
  glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
  glBufferData(GL_ARRAY_BUFFER, this->vertex.size()*sizeof(Vertex), &this->vertex[0], GL_STATIC_DRAW);
}

void
Mesh::criarBufferDeIndex()
{
  glGenBuffers(1, &this->IBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->vertexIndex.size()*sizeof(unsigned int), &this->vertexIndex[0], GL_STATIC_DRAW);
}

void
Mesh::draw(GLuint program)
{
  this->criarVertexArray();
  this->criarBufferDeVertex();
  this->criarBufferDeIndex();

  GLuint myUniformLocationMidX = glGetUniformLocation(program, "midX");
  GLuint myUniformLocationMidY = glGetUniformLocation(program, "midY");
  GLuint myUniformLocationMidZ = glGetUniformLocation(program, "midZ");

  GLuint myUniformLocationAngleY = glGetUniformLocation(program, "angleY");
  GLuint myUniformLocationAngleX = glGetUniformLocation(program, "angleX");
  GLuint myUniformLocationAngleZ = glGetUniformLocation(program, "angleZ");

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) 0);
  glVertexAttribPointer(1, 4, GL_INT, GL_TRUE, sizeof(Vertex), (const GLvoid*) 3);
  
  glUniform1f(myUniformLocationMidX, this->xMid);
  glUniform1f(myUniformLocationMidY, this->yMid);
  glUniform1f(myUniformLocationMidZ, this->zMid);

  glUniform1f(myUniformLocationAngleY, this->angleY);
  glUniform1f(myUniformLocationAngleX, this->angleX);
  glUniform1f(myUniformLocationAngleZ, this->angleZ);

  glDrawElements(GL_TRIANGLES, this->vertexIndex.size()*sizeof(unsigned int), GL_UNSIGNED_INT, (const GLvoid*) 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

  this->freeBuffers();

  this->deltaX = this->encontrarDeltaX();
  this->deltaY = this->encontrarDeltaY();
  this->deltaZ = this->encontrarDeltaZ();
}

void
Mesh::freeBuffers()
{
  glDeleteBuffers(1, &this->VBO);
  glDeleteBuffers(1, &this->IBO);
  this->VBO = 0;
  this->IBO = 0;
}

vector<Vertex> Mesh::getVertex() { return this->vertex; }
vector<unsigned int> Mesh::getIndice() { return this->vertexIndex; }

void
Mesh::resize(float scalar)
{
  float minX, minY, minZ;
  float maxX, maxY, maxZ;
  float midX, midY, midZ;

  int x = 0, y = 1, z = 2;

  for(unsigned int i = 0; i < vertex.size(); i++)
  {
    if(i == 0)
    {
      minX = vertex[i].position[x];
      minY = vertex[i].position[y];
      minZ = vertex[i].position[z];

      maxX = vertex[i].position[x];
      maxY = vertex[i].position[y];
      maxZ = vertex[i].position[z];
    }

    if(vertex[i].position[x] < minX)
      minX = vertex[i].position[x];

    if(vertex[i].position[x] > maxX)
      maxX = vertex[i].position[x];

    if(vertex[i].position[y] < minY)
      minY = vertex[i].position[y];

    if(vertex[i].position[y] > maxY)
      maxY = vertex[i].position[y];

    if(vertex[i].position[z] < minZ)
      minZ = vertex[i].position[z];

    if(vertex[i].position[z] > maxZ)
      maxZ = vertex[i].position[z];
  }

  midX = (maxX + minX) / 2;
  midY = (maxY + minY) / 2;
  midZ = (maxZ + minZ) / 2;

  Vertex midPoint = Vertex(midX, midY, midZ);

  for(unsigned int i = 0; i < vertex.size(); i++)
  {
    resizePoints(midPoint, vertex[i], scalar);
  }

  this->deltaX = this->encontrarDeltaX();
  this->deltaY = this->encontrarDeltaY();
  this->deltaZ = this->encontrarDeltaZ();
}

void
Mesh::resizePoints(Vertex &head, Vertex &tail, float scalar)
{
  Vertex vertex(tail.position[0] - head.position[0], 
                tail.position[1] - head.position[1],
                tail.position[2] - head.position[2]);

  vertex.position[0] *= scalar;
  vertex.position[1] *= scalar;
  vertex.position[2] *= scalar;

  tail.position[0] = vertex.position[0] + head.position[0];
  tail.position[1] = vertex.position[1] + head.position[1];
  tail.position[2] = vertex.position[2] + head.position[2];
}

void
Mesh::rotateY(float angleY)
{
  this->angleY += angleY;
}


void
Mesh::rotateX(float angleX)
{
  this->angleX += angleX;
}

void
Mesh::rotateZ(float angleZ)
{
  this->angleZ += angleZ;
}

void
Mesh::incPosition(float x, float y, float z)
{
  float minX, minY, minZ;

  for(unsigned int i = 0; i < vertex.size(); i++)
  {
    if(i == 0)
    {
      minX = vertex[i].position[0];
      minY = vertex[i].position[1];
      minZ = vertex[i].position[2];
    }

    if(vertex[i].position[0] < minX)
      minX = vertex[i].position[0];

    if(vertex[i].position[1] < minY)
      minY = vertex[i].position[1];

    if(vertex[i].position[2] < minZ)
      minZ = vertex[i].position[2];
  }

  Vertex newPoint = Vertex(x, y, z);
  Vertex minPoint = Vertex(minX, minY, minZ);
  newPoint.position[0] += minPoint.position[0];
  newPoint.position[1] += minPoint.position[1];
  newPoint.position[2] += minPoint.position[2];

  Vertex vector3d = Vertex(0, 0, 0);
  vector3d.position[0] = newPoint.position[0] - minPoint.position[0];
  vector3d.position[1] = newPoint.position[1] - minPoint.position[1];
  vector3d.position[2] = newPoint.position[2] - minPoint.position[2];
  // Vertex vector3d = Vertex(minPoint, newPoint);

  for(unsigned int i = 0; i < vertex.size(); i++)
  {
    vertex[i].position[0] += vector3d.position[0];
    vertex[i].position[1] += vector3d.position[1];
    vertex[i].position[2] += vector3d.position[2];
  }

  this->deltaX = this->encontrarDeltaX();
  this->deltaY = this->encontrarDeltaY();
  this->deltaZ = this->encontrarDeltaZ();
}

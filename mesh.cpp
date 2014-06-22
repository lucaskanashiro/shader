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
  getline(arquivo, tipo);
  
  if(tipo.find("OFF") == string::npos)
  {
    cout << "Tipo de arquivo invalido!!!" << endl;
    exit(-1);
  }

  string line;
  getline(arquivo, line);

  stringstream ss (line);
  ss >> this->qtdPontos >> this->qtdArestas >> this->qtdTextura;

  for(int i=0; i<this->qtdPontos; i++)
  {
    string linha;

    getline(arquivo, linha);
    stringstream buffer(linha);

    float x, y, z;
    int r, g, b, alpha;

    if(tipo.find("COFF") != string::npos)
    {
      buffer >> x >> y >> z >> r >> g >> b >> alpha;
      this->vertex.push_back(Vertex(x, y, z, r, g, b, alpha));
    }
    else if(tipo.find("OFF") != string::npos)
    {
      buffer >> x >> y >> z;
      this->vertex.push_back(Vertex(x, y, z, 127, 127, 127, 0));
    }

  }

  for(int j=0; j<this->qtdArestas; j++)
  {
    string linha;

    getline(arquivo, linha);
    stringstream buffer(linha);

    unsigned int valor, indice1, indice2, indice3;

    buffer >> valor >> indice1 >> indice2 >> indice3;

    this->indice.push_back(indice1);
    this->indice.push_back(indice2);
    this->indice.push_back(indice3);
  } 

  arquivo.close();

  this->deltaX = this->encontrarDeltaX();
  this->deltaY = this->encontrarDeltaY();
  this->deltaZ = this->encontrarDeltaZ();
}

void
Mesh::redimensionar()
{
  // for(unsigned int i=0; i<this->vertex.size(); i++)
  // {
  //   vertex[i].position[0] /= this->deltaX;
  //   vertex[i].position[1] /= this->deltaY;
  //   vertex[i].position[2] /= this->deltaZ;
  // }

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

Vertex
Mesh::calcularNormal(Vertex a, Vertex b, Vertex c)
{
	Vertex u((b.position[0] - a.position[0]), (b.position[1] - a.position[1]), (b.position[2] - a.position[2]));
	Vertex v((c.position[0] - a.position[0]), (c.position[1] - a.position[1]), (c.position[2] - a.position[2]));

	Vertex normal(((u.position[1] * v.position[2]) - (u.position[2] * v.position[1])), ((u.position[2] * v.position[0]) - (u.position[0] * v.position[2])), ((u.position[0] * v.position[1]) - (u.position[1] * v.position[0])));
	
	return normal;
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
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indice.size()*sizeof(unsigned int), &this->indice[0], GL_STATIC_DRAW);
}

void
Mesh::draw(GLuint program)
{
  this->criarVertexArray();
  this->criarBufferDeVertex();
  this->criarBufferDeIndex();

  // scale += 0.001f;
  /*GLuint gWorldLocation = glGetUniformLocation(program, "gWorld");


  Transformation t;
    t.Scale(sinf(this->scale * 0.1f), sinf(this->scale * 0.1f), sinf(this->scale * 0.1f));
    t.WorldPos(sinf(this->scale), 0.0f, 0.0f);
    t.Rotate(sinf(this->scale) * 90.0f, sinf(this->scale) * 90.0f, sinf(this->scale) * 90.0f);

    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)t.GetTrans());
*/
  GLuint myUniformLocationAngleY = glGetUniformLocation(program, "angleY");
  GLuint myUniformLocationAngleX = glGetUniformLocation(program, "angleX");
  GLuint myUniformLocationAngleZ = glGetUniformLocation(program, "angleZ");

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) 0);
  glVertexAttribPointer(1, 4, GL_INT, GL_TRUE, sizeof(Vertex), (const GLvoid*) 3);
  glUniform1f(myUniformLocationAngleY, this->angleY);
  glUniform1f(myUniformLocationAngleX, this->angleX);
  glUniform1f(myUniformLocationAngleZ, this->angleZ);

  glDrawElements(GL_TRIANGLES, this->indice.size()*sizeof(unsigned int), GL_UNSIGNED_INT, (const GLvoid*) 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

  this->freeBuffers();
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
vector<unsigned int> Mesh::getIndice() { return this->indice; }




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
}
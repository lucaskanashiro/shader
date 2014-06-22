#include "mesh.h"

Mesh::Mesh(){}

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
  for(unsigned int i=0; i<this->vertex.size(); i++)
  {
    vertex[i].position[0] /= this->deltaX;
    vertex[i].position[1] /= this->deltaY;
    vertex[i].position[2] /= this->deltaZ;
  }

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
Mesh::draw()
{
  glBufferData(GL_ARRAY_BUFFER, this->vertex.size()*sizeof(Vertex), &this->vertex[0], GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indice.size()*sizeof(unsigned int), &this->indice[0], GL_STATIC_DRAW);
  glDrawElements(GL_TRIANGLES, this->indice.size()*sizeof(unsigned int), GL_UNSIGNED_INT, (const GLvoid*) 0);
}

vector<Vertex> Mesh::getVertex() { return this->vertex; }
vector<unsigned int> Mesh::getIndice() { return this->indice; }



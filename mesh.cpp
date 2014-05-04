#include "mesh.h"

Mesh::Mesh(){}

Mesh::~Mesh(){}

double
Mesh::encontrarDeltaX()
{
  double xMax = vertex[0].position[0];
  double xMin = vertex[0].position[0];

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

double
Mesh::encontrarDeltaY()
{
  double yMax = vertex[0].position[1];
  double yMin = vertex[0].position[1];

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

double
Mesh::encontrarDeltaZ()
{
  double zMax = vertex[0].position[2];
  double zMin = vertex[0].position[2];

  for(unsigned int i=1; i<this->vertex.size(); i++)
  {
    if(vertex[i].position[2] > zMax) 
      zMax = vertex[i].position[2];

    if(vertex[i].position[2] < zMin) 
      zMin = vertex[i].position[2];
  }

  this->zMin = zMin;

  return zMax - zMin;
}

void Mesh::printArrays()
{
  for(unsigned int i=0; i<this->vertex.size(); i++)
  {
    cout << "VERTEX "<< i << endl;
    cout << "x: " << vertex[i].position[0] << "\ty: " << vertex[i].position[1] << "\tz: " << vertex[i].position[2] << endl;
    cout << "r: " << vertex[i].color[0] << "\tg: " << vertex[i].color[1] << "\tb: " << vertex[i].color[2] << "\ta: " << vertex[i].color[3] << endl;
  }

  cout << endl << "INDICE: " << endl;
  for(unsigned int i=0; i<this->indice.size(); i++)
  {
    cout << indice[i] << endl;
  }
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

  //printArrays();
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

vector<Vertex> Mesh::getVertex() { return this->vertex; }
vector<unsigned int> Mesh::getIndice() { return this->indice; }



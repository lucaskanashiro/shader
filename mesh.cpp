#include "mesh.h"
#include <IL/il.h>
#include <cmath>

using namespace Magick;

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
  string path = getPath(nomeArquivo);

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
      this->fileMtl = path + this->fileMtl;
    }

    else if(line.substr(0, 2) == "v ")
    {
      stream >> trash >> x >> y >> z;
      this->vertex.push_back(Vertex(x, y, z, 100, 100, 100, 100));
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

  // for(unsigned int i=0; i<this->vertexIndex.size(); i++)
  // {
  //   cout << this->vertexIndex[i] << endl;
  // }

  arquivo.close();

  carregarMaterial(this->fileMtl);

  this->deltaX = this->encontrarDeltaX();
  this->deltaY = this->encontrarDeltaY();
  this->deltaZ = this->encontrarDeltaZ();
}

void
Mesh::carregarMaterial(string nomeArquivo)
{
  ifstream arquivo(nomeArquivo.c_str());
  string path = getPath(nomeArquivo);

  if(!arquivo.is_open())
  {
    cout << "Não foi possível abrir o aquivo!!!" << endl;
    exit(-1);
  }

  string tipo;
  string line;
  string trash;

  // float x, y, z;

  cout << "1 --" << endl;

  this->material = vector<Material>();

  cout << "2 --" << endl;

  while(getline(arquivo, line))
  {
    bool copy = false;
    string temp = "";

    for(unsigned int i = 0; i < line.size(); i++)
    {
      if(isalnum(line[i]))
        copy = true;

      if(copy)
        temp += line[i];
    }
    line = temp;

    stringstream stream(line);

    if(line.substr(0, 6) == "newmtl")
    {
      this->material.push_back(Material());
      stream >> trash >> this->material.back().name;
    }
    else if(line.substr(0, 2) == "Ns")
    {
      stream >> trash >> this->material.back().ns;
    }
    else if(line.substr(0, 2) == "Ni")
    {
      stream >> trash >> this->material.back().ni;
    }
    else if(line.substr(0, 2) == "d ")
    {
      stream >> trash >> this->material.back().d;
    }
    else if(line.substr(0, 2) == "Tr")
    {
      stream >> trash >> this->material.back().tr;
    }
    else if(line.substr(0, 2) == "Tf")
    {
      stream >> trash >> this->material.back().tf[0];
      stream >> this->material.back().tf[1];
      stream >> this->material.back().tf[2];
    }
    else if(line.substr(0, 5) == "illum")
    {
      stream >> trash >> this->material.back().illum;
    }
    else if(line.substr(0, 2) == "Ka")
    {
      stream >> trash >> this->material.back().ka[0];
      stream >> this->material.back().ka[1];
      stream >> this->material.back().ka[2];
    }
    else if(line.substr(0, 2) == "Kd")
    {
      stream >> trash >> this->material.back().kd[0];
      stream >> this->material.back().kd[1];
      stream >> this->material.back().kd[2];
    }
    else if(line.substr(0, 2) == "Ks")
    {
      stream >> trash >> this->material.back().ks[0];
      stream >> this->material.back().ks[1];
      stream >> this->material.back().ks[2];
    }
    else if(line.substr(0, 2) == "Ke")
    {
      stream >> trash >> this->material.back().ke[0];
      stream >> this->material.back().ke[1];
      stream >> this->material.back().ke[2];
    }
    else if(line.substr(0, 6) == "map_Kd") //arcade
    // else if(line.substr(0, 8) == "map_bump") //gun8
    {
      stream >> trash >> this->material.back().fileName;
      this->material.back().fileName = path + this->material.back().fileName;

      this->material.back().color = getColorRGB(this->material.back().fileName);

      this->material.back().imageID = loadImage(this->material.back().fileName.c_str());
      if(this->material.back().imageID == 0)
      {
        cout << "Error to load image: " << this->material.back().fileName << endl;
      }
      else
      {
        ilBindImage(this->material.back().imageID);
        this->material.back().imageWidth = ilGetInteger(IL_IMAGE_WIDTH);
        this->material.back().imageHeight = ilGetInteger(IL_IMAGE_HEIGHT);
        this->material.back().imageData = ilGetData();

        cout << endl << "+----------------------------------+" << endl;
        cout << "material.back().fileName: " << this->material.back().fileName << endl;
        cout << "material.back().imageID: " << this->material.back().imageID << endl;
        cout << "material.back().imageWidth: " << this->material.back().imageWidth << endl;
        cout << "material.back().imageHeight: " << this->material.back().imageHeight << endl;
        // cout << "material.back().imageData: " << this->material.back().imageData << endl;
      }
    }

  }

  cout << "3 --" << endl;

  arquivo.close();
  cout << "arquivo.close()" << endl;

  this->deltaX = this->encontrarDeltaX();
  this->deltaY = this->encontrarDeltaY();
  this->deltaZ = this->encontrarDeltaZ();
}

vector<MyColorRGB>
Mesh::getColorRGB(string nomeArquivo)
{
  vector<MyColorRGB> vectorColor;

  // cout << "nomeArquivo: " << nomeArquivo << endl;

  Image image;
  image.read(nomeArquivo);

  int width = image.columns();
  int height = image.rows();

  Pixels view(image);
  PixelPacket *pixels = view.get(0, 0, width, height);

  for(int i = 0; i < width; i++)
  {
    for(int j = 0; j < height; j++)
    {
      MyColorRGB color;
      Color pixelColor = pixels[width * i + j];

      color.red = (int)pixelColor.redQuantum();
      color.green = (int)pixelColor.greenQuantum();
      color.blue = (int)pixelColor.blueQuantum();

      vectorColor.push_back(color);
    }
  }

  return vectorColor;
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
Mesh::criarBufferTexture()
{
  glGenTextures(1, &this->textureArrayID);
  glBindTexture( GL_TEXTURE_2D, this->textureArrayID);
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

string
Mesh::getPath(string fileName)
{
  for(int i = (int)fileName.size() - 1; i >= 0; i--)
  {
    if(fileName[i] == '/') {
      return fileName.substr(0, i+1);
    }
  }

  return "";
}

int
Mesh::loadImage(string imageName)
{
  ILboolean success;
  unsigned int imageID;
 
  // init DevIL. This needs to be done only once per application
  ilInit();
  // generate an image name
  ilGenImages(1, &imageID); 
  // bind it
  ilBindImage(imageID); 
  // match image origin to OpenGL’s
  ilEnable(IL_ORIGIN_SET);
  ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
  // load  the image
  success = ilLoadImage((ILstring)imageName.c_str());
  ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
  // check to see if everything went OK
  if (!success) {
    ilDeleteImages(1, &imageID); 
    return 0;
  }
  else return imageID;
}
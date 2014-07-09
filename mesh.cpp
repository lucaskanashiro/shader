#include "mesh.h"
#include <IL/il.h>
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
      stream >> trash >> x >> y;
      this->texture.push_back(Vertex2D(x, y));
    }

    else if(line.substr(0,2) == "f ")
    {
      unsigned int v, n, t;
      char c_trash;

      stream >> trash;
      for(int i = 0; i < 3; i++)
      {
        if(!this->vertex.empty())
        {
          v = 1;
          stream >> v;
          this->vertexIndex.push_back(v-1);
        }

        stream >> c_trash;

        if(!this->normal.empty())
        {
          n = 1;
          stream >> n;
          this->normalIndex.push_back(n-1);
        }

        stream >> c_trash;

        if(!this->texture.empty())
        {
          t = 1;
          stream >> t;
          this->textureIndex.push_back(t-1);
        }
      }
    }
  }

  // cout << this->vertexIndex.size() << endl;

  // for(unsigned int i = 0; i < this->vertexIndex.size() / 3; i++)
  // {
  //   cout << "f " << this->vertexIndex[(i * 2) + i + 0] << '/' << this->normalIndex[(i * 2) + i + 0] << '/' << this->textureIndex[(i * 2) + i + 0] << ' ';
  //   cout << this->vertexIndex[(i * 2) + i + 1] << '/' << this->normalIndex[(i * 2) + i + 1] << '/' << this->textureIndex[(i * 2) + i + 1] << ' ';
  //   cout << this->vertexIndex[(i * 2) + i + 2] << '/' << this->normalIndex[(i * 2) + i + 2] << '/' << this->textureIndex[(i * 2) + i + 2] << ' ' << endl;
  // }

  // for(unsigned int i = 0; i < this->vertexIndex.size(); i += 3)
  // {
  //   cout << "f " << this->vertexIndex[i] << '/' << this->normalIndex[i] << '/' << this->textureIndex[i] << ' ';
  //   cout << this->vertexIndex[i+1] << '/' << this->normalIndex[i+1] << '/' << this->textureIndex[i+1] << ' ';
  //   cout << this->vertexIndex[i+2] << '/' << this->normalIndex[i+2] << '/' << this->textureIndex[i+2] << ' ' << endl;
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

  this->material = vector<Material>();

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

        // cout << endl << "+----------------------------------+" << endl;
        // cout << "material.back().fileName: " << this->material.back().fileName << endl;
        // cout << "material.back().imageID: " << this->material.back().imageID << endl;
        // cout << "material.back().imageWidth: " << this->material.back().imageWidth << endl;
        // cout << "material.back().imageHeight: " << this->material.back().imageHeight << endl;
      }
    }

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
  glBindVertexArray(this->vertexArrayID);
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
  glGenBuffers(1, &this->textureArrayID);
  glBindBuffer(GL_TEXTURE_2D, this->textureArrayID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->texture.size()*sizeof(Vertex2D), &this->texture[0], GL_STATIC_DRAW);
}

void
Mesh::criarBufferTextureIndex()
{
  glGenBuffers(1, &this->textureIndexArrayID);
  glBindBuffer(GL_TEXTURE_2D, this->textureIndexArrayID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->textureIndex.size()*sizeof(unsigned int), &this->textureIndex[0], GL_STATIC_DRAW);
}

void
Mesh::draw(GLuint)
{

  for(unsigned int i = 0; i < vertexIndex.size() / 3; i++)
  {
    glBegin(GL_LINES);
      this->vertex[vertexIndex[(i * 2) + i + 0]].drawGl();
      this->vertex[vertexIndex[(i * 2) + i + 1]].drawGl();
    glEnd();

    glBegin(GL_LINES);
      this->vertex[vertexIndex[(i * 2) + i + 0]].drawGl();
      this->vertex[vertexIndex[(i * 2) + i + 2]].drawGl();
    glEnd();

    glBegin(GL_LINES); 
      this->vertex[vertexIndex[(i * 2) + i + 1]].drawGl();
      this->vertex[vertexIndex[(i * 2) + i + 2]].drawGl();
    glEnd();
  }


  // this->criarVertexArray();
  // this->criarBufferDeVertex();
  // this->criarBufferDeIndex();

  // this->criarBufferTexture();
  // this->criarBufferTextureIndex();

  // GLuint myUniformLocationMidX = glGetUniformLocation(program, "midX");
  // GLuint myUniformLocationMidY = glGetUniformLocation(program, "midY");
  // GLuint myUniformLocationMidZ = glGetUniformLocation(program, "midZ");

  // GLuint myUniformLocationAngleY = glGetUniformLocation(program, "angleY");
  // GLuint myUniformLocationAngleX = glGetUniformLocation(program, "angleX");
  // GLuint myUniformLocationAngleZ = glGetUniformLocation(program, "angleZ");

  // glEnableVertexAttribArray(0);
  // glEnableVertexAttribArray(1);
  // glEnableVertexAttribArray(2);

  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) 0);
  // glVertexAttribPointer(1, 4, GL_INT, GL_TRUE, sizeof(Vertex), (const GLvoid*) 3);
  // glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, sizeof(Vertex2D), (const GLvoid*) 0);
  
  // glUniform1f(myUniformLocationMidX, this->xMid);
  // glUniform1f(myUniformLocationMidY, this->yMid);
  // glUniform1f(myUniformLocationMidZ, this->zMid);

  // glUniform1f(myUniformLocationAngleY, this->angleY);
  // glUniform1f(myUniformLocationAngleX, this->angleX);
  // glUniform1f(myUniformLocationAngleZ, this->angleZ);

  // glBindTexture(GL_TEXTURE_2D, this->material[0].textureID);

  // glDrawElements(GL_TRIANGLES, this->vertexIndex.size()*sizeof(unsigned int), GL_UNSIGNED_INT, (const GLvoid*) 0);

  // glDisableVertexAttribArray(0);
  // glDisableVertexAttribArray(1);
  // glDisableVertexAttribArray(2);

  // this->freeBuffers();

  // this->deltaX = this->encontrarDeltaX();
  // this->deltaY = this->encontrarDeltaY();
  // this->deltaZ = this->encontrarDeltaZ();
}

void
Mesh::freeBuffers()
{
  glDeleteBuffers(1, &this->VBO);
  glDeleteBuffers(1, &this->IBO);
  glDeleteBuffers(1, &this->textureArrayID);
  glDeleteBuffers(1, &this->textureIndexArrayID);
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
  float pi = 3.141592653589793;
  float radAngleY = angleY*pi/180.0;
  vector<Vertex> b = this->vertex;
  vector<Vertex> c = this->vertex;
  this->angleY += angleY;

  Vertex midPoint = getMidPoint();
  float midX = midPoint.position[0];
  float midZ = midPoint.position[2];

  for(unsigned int i = 0; i < b.size(); i++)
  {
    c[i].position[0] = (b[i].position[0] - midX) * cos(radAngleY) - (b[i].position[2] - midZ) * sin(radAngleY) + midX;
    c[i].position[2] = (b[i].position[2] - midZ) * cos(radAngleY) + (b[i].position[0] - midX) * sin(radAngleY) + midZ;
  }

  this->vertex = c;
}

void
Mesh::rotateX(float angleX)
{
  float pi = 3.141592653589793;
  float radAngleX = angleX*pi/180.0;
  vector<Vertex> a = this->vertex;
  vector<Vertex> b = this->vertex;

  Vertex midPoint = getMidPoint();
  float midY = midPoint.position[1];
  float midZ = midPoint.position[2];
  this->angleX += angleX;

  for(unsigned int i = 0; i < a.size(); i++)
  {
    b[i].position[1] = (a[i].position[1] - midY) * cos(radAngleX) - (a[i].position[2] - midZ) * sin(radAngleX) + midY;
    b[i].position[2] = (a[i].position[2] - midZ) * cos(radAngleX) + (a[i].position[1] - midY) * sin(radAngleX) + midZ;
  }

  this->vertex = b;
}

void
Mesh::rotateZ(float angleZ)
{
  float pi = 3.141592653589793;
  float radAngleZ = angleZ*pi/180.0;
  vector<Vertex> c = this->vertex;
  vector<Vertex> d = this->vertex;
  this->angleZ += angleZ;

  Vertex midPoint = getMidPoint();
  float midX = midPoint.position[0];
  float midY = midPoint.position[1];

  for(unsigned int i = 0; i < c.size(); i++)
  {
    d[i].position[0] = (c[i].position[0] - midX) * cos(radAngleZ) - (c[i].position[1] - midY) * sin(radAngleZ) + midX;
    d[i].position[1] = (c[i].position[1] - midY) * cos(radAngleZ) + (c[i].position[0] - midX) * sin(radAngleZ) + midY;
  }

  this->vertex = d;
}

void
Mesh::setPosition(float x, float y, float z)
{
  Vertex midPoint, newPoint;
  Vertex vector3d;

  midPoint = getMidPoint();
  newPoint = Vertex(x, y, z);
  vector3d.position[0] = newPoint.position[0] - midPoint.position[0];
  vector3d.position[1] = newPoint.position[1] - midPoint.position[1];
  vector3d.position[2] = newPoint.position[2] - midPoint.position[2];

  for(unsigned int i = 0; i < this->vertex.size(); i++)
  {
    this->vertex[i].position[0] += vector3d.position[0];
    this->vertex[i].position[1] += vector3d.position[1];
    this->vertex[i].position[2] += vector3d.position[2];
  }
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

void
Mesh::prepareAllTexture()
{
  prepareTexture(this->material[0]);
}

void
Mesh::prepareTexture(Material &material) {
  int w, h;
  unsigned char* data;

  w = material.imageWidth;
  h = material.imageHeight;
  data = material.imageData;

  /* Create and load texture to OpenGL */
  glGenTextures(1, &material.textureID); /* Texture name generation */
  glBindTexture(GL_TEXTURE_2D, material.textureID); 
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
                w, h, 
                0, GL_RGBA, GL_UNSIGNED_BYTE,
                data);
  // glBindTexture(GL_TEXTURE_2D, material.textureID);
  // glActiveTexture(GL_TEXTURE0);
  // glGenerateMipmap(GL_TEXTURE_2D);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

Vertex
Mesh::getMidPoint()
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

  return midPoint;

}

float
Mesh::getDeltaX()
{
  return deltaX;
}

float
Mesh::getDeltaY()
{
  return deltaY;
}

float
Mesh::getDeltaZ()
{
  return deltaZ;
}

void
Mesh::showPosition()
{
  Vertex midPoint = getMidPoint();
  cout << endl << "+----------showPosition----------+" << endl;
  cout << "midX: " << midPoint.position[0] << endl;
  cout << "midY: " << midPoint.position[1] << endl;
  cout << "midZ: " << midPoint.position[2] << endl;
}

void
Mesh::showDimensions()
{
  cout << endl << "+----------showDimensions----------+" << endl;
  cout << "deltaX: " << this->deltaX << endl;
  cout << "deltaY: " << this->deltaY << endl;
  cout << "deltaZ: " << this->deltaZ << endl;
}

void
Mesh::showAngles()
{
  cout << endl << "+----------showAngles----------+" << endl;
  cout << "angleX: " << this->angleX << endl;
  cout << "angleY: " << this->angleY << endl;
  cout << "angleZ: " << this->angleZ << endl;
}
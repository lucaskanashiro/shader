#include "vec.h"
#include "CG.h"
#include "mesh.h"
#include "shader.h"

#include <GL/freeglut.h>
#include <IL/il.h>

const float VELOCITY_MOVE = 0.05;
const float VELOCITY_ANGLE = 5;

float velocityCameraX = 0.0f;
float velocityCameraY = 0.0f;
float velocityCameraZ = 0.0f;

float velocityCameraAngleX = 0.0f;
float velocityCameraAngleY = 0.0f;
float velocityCameraAngleZ = 0.0f;


vector<Mesh *> vectorMesh;
float velocityX = 0, velocityY = 0, velocityZ = 0;
float velocityAngleY = 0, velocityAngleX = 0;
float velocityAngleZ = 0;
SDL_Event sdlEvent;
bool finish;
int indice = 0;

inline void moveCamera();
inline void moveObjects();
void (*keyboardFunction)();

void printInstructions();

int main()
{

  printInstructions();

  GerenciadorGrafico gerenciador(1024, 720);

  Mesh *mesh;

  char lineType;
  string fileName;
  float x, y, z;
  float scaleResize;
  float angleY;
  while(cin >> lineType)
  {
    if(lineType == 'M')
    {
      cin >> fileName;
      cin >> x >> y >> z;
      cin >> scaleResize;
      cin >> angleY;

      mesh = new Mesh();
      mesh->carregarArquivo(fileName);
      mesh->prepareAllTexture();
      mesh->redimensionar();

      mesh->resize(scaleResize / mesh->getDeltaY());
      mesh->setPosition(x, y, z);
      mesh->rotateY(angleY);
      // mesh->transladar();

      vectorMesh.push_back(mesh);
    }
  }  

  gerenciador.setViewPort(1024, 720);
  gerenciador.iniciarRender();

  glewInit();

  glActiveTextureARB(GL_TEXTURE0_ARB);
  glActiveTexture(GL_TEXTURE0);
  

  GerenciadorShader shader;

  shader.setVertexShader("vertex.glsl");
  shader.setFragmentShader("fragment.glsl");

  GLuint program = shader.initShader();
  cout<< "TUDO OK!" << endl;
  glUseProgram(program);
  glClearColor(1.0, 1.0, 1.0, 1.0);

  // mesh[0].setZMax(mesh[1].getZMax() + 0.9);

  for(int i = 0; i < (int)vectorMesh.size(); i++)
  {
    shader.addMesh(vectorMesh[i]);
  }


  // mesh->rotateY(0.01);

  finish = false;

  // keyboardFunction = moveObjects;
  keyboardFunction = moveCamera;

  // glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_MULTISAMPLE);

  while(!finish)
  {
    while(SDL_PollEvent(&sdlEvent))
    {
      if(sdlEvent.type == SDL_QUIT)
      {
        finish = true;
        break;
      }

      if(sdlEvent.key.keysym.sym == SDLK_1)
      {
        if(sdlEvent.key.state == SDL_PRESSED)
          keyboardFunction = moveCamera;
      }

      if(sdlEvent.key.keysym.sym == SDLK_2)
      {
        if(sdlEvent.key.state == SDL_PRESSED)
          keyboardFunction = moveObjects;
      }

      keyboardFunction();

    }

    glTranslatef(velocityCameraX, velocityCameraY, velocityCameraZ);
    glRotatef(velocityCameraAngleX, 1.0f, 0.0f, 0.0f);
    glRotatef(velocityCameraAngleY, 0.0f, 1.0f, 0.0f);
    glRotatef(velocityCameraAngleZ, 0.0f, 0.0f, 1.0f);

    vectorMesh[indice]->rotateY(velocityAngleY);
    vectorMesh[indice]->rotateX(velocityAngleX);
    vectorMesh[indice]->rotateZ(velocityAngleZ);
    vectorMesh[indice]->incPosition(velocityX, velocityY, velocityZ);

    shader.renderizarCena(program);
    gerenciador.displayRender();
    gerenciador.delay(36);
  }


  // delete mesh;

  return 0;
}

inline void moveCamera()
{
  if(sdlEvent.key.keysym.sym == SDLK_UP)
  {
    if(sdlEvent.key.state == SDL_PRESSED)
      velocityCameraY = -VELOCITY_MOVE;
    else
      velocityCameraY = 0;
  }

  if(sdlEvent.key.keysym.sym == SDLK_DOWN)
  {
    if(sdlEvent.key.state == SDL_PRESSED)
      velocityCameraY = VELOCITY_MOVE;
    else
      velocityCameraY = 0;
  }

  if(sdlEvent.key.keysym.sym == SDLK_LEFT)
  {
    if(sdlEvent.key.state == SDL_PRESSED)
      velocityCameraX = VELOCITY_MOVE;
    else
      velocityCameraX = 0.0f;
  }

  if(sdlEvent.key.keysym.sym == SDLK_RIGHT)
  {
    if(sdlEvent.key.state == SDL_PRESSED)
      velocityCameraX = -VELOCITY_MOVE;
    else
      velocityCameraX = 0.0f;
  }

  if(sdlEvent.key.keysym.sym == SDLK_m)
  {
    if(sdlEvent.key.state == SDL_PRESSED)
      velocityCameraZ = -VELOCITY_MOVE;
    else
      velocityCameraZ = 0;
  }

  if(sdlEvent.key.keysym.sym == SDLK_n)
  {
    if(sdlEvent.key.state == SDL_PRESSED)
      velocityCameraZ = VELOCITY_MOVE;
    else
      velocityCameraZ = 0;
  }

  if(sdlEvent.key.keysym.sym == SDLK_d)
  {
    if(sdlEvent.key.state == SDL_PRESSED)
      velocityCameraAngleY = VELOCITY_ANGLE;
    else
      velocityCameraAngleY = 0;
  }

  if(sdlEvent.key.keysym.sym == SDLK_a)
  {
    if(sdlEvent.key.state == SDL_PRESSED)
      velocityCameraAngleY = -VELOCITY_ANGLE;
    else
      velocityCameraAngleY = 0;
  }

  if(sdlEvent.key.keysym.sym == SDLK_w)
  {
    if(sdlEvent.key.state == SDL_PRESSED)
      velocityCameraAngleX = VELOCITY_ANGLE;
    else
      velocityCameraAngleX = 0;
  }

  if(sdlEvent.key.keysym.sym == SDLK_s)
  {
    if(sdlEvent.key.state == SDL_PRESSED)
      velocityCameraAngleX = -VELOCITY_ANGLE;
    else
      velocityCameraAngleX = 0;
  }

  if(sdlEvent.key.keysym.sym == SDLK_e)
  {
    if(sdlEvent.key.state == SDL_PRESSED)
      velocityCameraAngleZ = VELOCITY_ANGLE;
    else
      velocityCameraAngleZ = 0;
  }

  if(sdlEvent.key.keysym.sym == SDLK_q)
  {
    if(sdlEvent.key.state == SDL_PRESSED)
      velocityCameraAngleZ = -VELOCITY_ANGLE;
    else
      velocityCameraAngleZ = 0;
  }
}

inline void moveObjects()
{
  if(sdlEvent.key.keysym.sym == SDLK_p)
  {
    if(sdlEvent.key.state == SDL_PRESSED)
    {
      vectorMesh[indice]->showPosition();
      vectorMesh[indice]->showDimensions();
      vectorMesh[indice]->showAngles();
    }
  }

  if(sdlEvent.key.keysym.sym == SDLK_c)
  {
    if(sdlEvent.key.state == SDL_PRESSED)
    {
      indice++;
      indice = indice % vectorMesh.size();
    }
  }

  if(sdlEvent.key.keysym.sym == SDLK_x)
  {
    if(sdlEvent.key.state == SDL_PRESSED)
    {
      indice--;
      indice = indice % vectorMesh.size();
    }
  }

  if(sdlEvent.key.keysym.sym == SDLK_PAGEUP)
  {
    if(sdlEvent.key.state == SDL_PRESSED)
    {
      // cout << endl;
      // cout << "vectorMesh: " << vectorMesh->getZMax() << endl;
      vectorMesh[indice]->resize(1.2);
    }
  }

  if(sdlEvent.key.keysym.sym == SDLK_PAGEDOWN)
  {
    if(sdlEvent.key.state == SDL_PRESSED)
    {
      // cout << endl;
      // cout << "vectorMesh: " << vectorMesh->getZMax() << endl;
      vectorMesh[indice]->resize(0.83);
    }
  }

  if(sdlEvent.key.keysym.sym == SDLK_d)
  {
    if(sdlEvent.key.state == SDL_PRESSED)
      velocityAngleY = VELOCITY_ANGLE;
    else
      velocityAngleY = 0;
  }

  if(sdlEvent.key.keysym.sym == SDLK_a)
  {
    if(sdlEvent.key.state == SDL_PRESSED)
      velocityAngleY = -VELOCITY_ANGLE;
    else
      velocityAngleY = 0;
  }

  if(sdlEvent.key.keysym.sym == SDLK_w)
  {
    if(sdlEvent.key.state == SDL_PRESSED)
      velocityAngleX = -VELOCITY_ANGLE;
    else
      velocityAngleX = 0;
  }

  if(sdlEvent.key.keysym.sym == SDLK_s)
  {
    if(sdlEvent.key.state == SDL_PRESSED)
      velocityAngleX = VELOCITY_ANGLE;
    else
      velocityAngleX = 0;
  }

  if(sdlEvent.key.keysym.sym == SDLK_e)
  {
    if(sdlEvent.key.state == SDL_PRESSED)
      velocityAngleZ = -VELOCITY_ANGLE;
    else
      velocityAngleZ = 0;
  }

  if(sdlEvent.key.keysym.sym == SDLK_q)
  {
    if(sdlEvent.key.state == SDL_PRESSED)
      velocityAngleZ = VELOCITY_ANGLE;
    else
      velocityAngleZ = 0;
  }

  if(sdlEvent.key.keysym.sym == SDLK_UP)
  {
    if(sdlEvent.key.state == SDL_PRESSED)
      velocityY = VELOCITY_MOVE;
    else
      velocityY = 0;
  }

  if(sdlEvent.key.keysym.sym == SDLK_DOWN)
  {
    if(sdlEvent.key.state == SDL_PRESSED)
      velocityY = -VELOCITY_MOVE;
    else
      velocityY = 0;
  }

  if(sdlEvent.key.keysym.sym == SDLK_LEFT)
  {
    if(sdlEvent.key.state == SDL_PRESSED)
      velocityX = -VELOCITY_MOVE;
    else
      velocityX = 0;
  }

  if(sdlEvent.key.keysym.sym == SDLK_RIGHT)
  {
    if(sdlEvent.key.state == SDL_PRESSED)
      velocityX = VELOCITY_MOVE;
    else
      velocityX = 0;
  }

  if(sdlEvent.key.keysym.sym == SDLK_m)
  {
    if(sdlEvent.key.state == SDL_PRESSED)
      velocityZ = -VELOCITY_MOVE;
    else
      velocityZ = 0;
  }

  if(sdlEvent.key.keysym.sym == SDLK_n)
  {
    if(sdlEvent.key.state == SDL_PRESSED)
      velocityZ = VELOCITY_MOVE;
    else
      velocityZ = 0;
  }
}

void printInstructions()
{
  cout << endl;
  cout << "+INSTRUCOES+" << endl;
  cout << endl;

  cout << "+---------------GERAL---------------+" << endl;
  cout << "| 1 - Movimentacao da CAMERA" << endl;
  cout << "| 2 - Movimentacao dos OBJETOS" << endl;
  cout << "+-----------------------------------+" << endl;
  cout << endl;

  cout << "+--------------CAMERA--------------+" << endl;
  cout << "+-----Movimentacao:" << endl;
  cout << "| UP - Andar para frente" << endl;
  cout << "| DOWN - Andar para tras" << endl;
  cout << "| RIGHT - Girar para direita" << endl;
  cout << "| LEFT - Girar para esquerda" << endl;
  cout << "+----------------------------------+" << endl;
  cout << endl;

  cout << "+--------------OBJETOS--------------+" << endl;
  cout << "+-----Movimentacao:" << endl;
  cout << "| UP - Mover no eixo Y +" << endl;
  cout << "| DOWN - Mover no eixo Y -" << endl;
  cout << "| RIGHT - Mover no eixo X +" << endl;
  cout << "| LEFT - Mover no eixo Y -" << endl;
  cout << "| M - Mover no eixo Z +" << endl;
  cout << "| N - Mover no eixo Z -" << endl;
  cout << "+-----:" << endl;

  cout << "+-----Rotacao:" << endl;
  cout << "| W - Rotacionao no eixo X +" << endl;
  cout << "| S - Rotacionao no eixo X -" << endl;
  cout << "| D - Rotacionao no eixo Y +" << endl;
  cout << "| A - Rotacionao no eixo Y -" << endl;
  cout << "| E - Rotacionao no eixo Z +" << endl;
  cout << "| Q - Rotacionao no eixo Z -" << endl;
  cout << "+-----:" << endl;

  cout << "+-----Selecionar Objeto:" << endl;
  cout << "| C - Seleciona o proximo objeto" << endl;
  cout << "| X - Seleciona o objeto anterior" << endl;
  cout << "+-----:" << endl;
  cout << endl; 
}
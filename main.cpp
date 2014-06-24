#include "vec.h"
#include "CG.h"
#include "mesh.h"
#include "shader.h"

const float VELOCITY_MOVE = 0.05;
const float VELOCITY_ANGLE = 5;

int main(int argc, char* argv[])
{

  GerenciadorGrafico gerenciador;

  vector<Mesh *> vectorMesh;
  for(int i = 1; i < argc; i++)
  {
    Mesh *mesh = new Mesh();
    mesh->carregarArquivo(argv[i]);
    mesh->redimensionar();
    mesh->transladar();

    vectorMesh.push_back(mesh);
  }
  

  gerenciador.setViewPort(800, 600);
  gerenciador.iniciarRender();

  glewInit();
  

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

  float velocityX = 0, velocityY = 0, velocityZ = 0;
  float velocityAngleY = 0, velocityAngleX = 0;
  float velocityAngleZ = 0;
  SDL_Event sdlEvent;
  bool finish;
  int indice = 0;

  finish = false;

  // mesh->rotateY(0.01);

  while(!finish)
  {
    while(SDL_PollEvent(&sdlEvent))
    {
      if(sdlEvent.type == SDL_QUIT)
      {
        finish = true;
        break;
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

      if(sdlEvent.key.keysym.sym == SDLK_w)
      {
        if(sdlEvent.key.state == SDL_PRESSED)
          velocityAngleY = VELOCITY_ANGLE;
        else
          velocityAngleY = 0;
      }

      if(sdlEvent.key.keysym.sym == SDLK_s)
      {
        if(sdlEvent.key.state == SDL_PRESSED)
          velocityAngleY = -VELOCITY_ANGLE;
        else
          velocityAngleY = 0;
      }

      if(sdlEvent.key.keysym.sym == SDLK_d)
      {
        if(sdlEvent.key.state == SDL_PRESSED)
          velocityAngleX = VELOCITY_ANGLE;
        else
          velocityAngleX = 0;
      }

      if(sdlEvent.key.keysym.sym == SDLK_a)
      {
        if(sdlEvent.key.state == SDL_PRESSED)
          velocityAngleX = -VELOCITY_ANGLE;
        else
          velocityAngleX = 0;
      }

      if(sdlEvent.key.keysym.sym == SDLK_e)
      {
        if(sdlEvent.key.state == SDL_PRESSED)
          velocityAngleZ = VELOCITY_ANGLE;
        else
          velocityAngleZ = 0;
      }

      if(sdlEvent.key.keysym.sym == SDLK_q)
      {
        if(sdlEvent.key.state == SDL_PRESSED)
          velocityAngleZ = -VELOCITY_ANGLE;
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


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

int main(int argc, char* argv[])
{
  // string fileName = "/home/luciano/Documents/UnB/ICG/repositorios/shader/Model_Texture/TexturaArcadeEsquedo/Right_Side_Image.jpg";
  // string fileName = "./Right_Side_Image.jpg";
  // int imageID, imageWidth, imageHeight;
  // unsigned char* imageData;

  // ILuint id;

  // cout << "- 1 -" << endl;

  // ilGenImages(1,&id); 
  // cout << "- 2 -" << endl;
  // ilBindImage(id);
  // cout << "- 3 -" << endl;

  // imageID = ilLoadImage(fileName.c_str());
  // cout << "- 4 -" << endl;
  // if(imageID == 0)
  // {
  //   ilDeleteImages(1,&id);
  //   cout << "Error to load image: " << fileName << endl;
  //   cout << "Error Message: " << ilGetError() << endl;
  // }
  // else
  // {
  //   imageWidth = ilGetInteger(IL_IMAGE_WIDTH);
  //   imageHeight = ilGetInteger(IL_IMAGE_HEIGHT);
  //   imageData = ilGetData();
    
  //   cout << endl << "+----------------------------------+" << endl;
  //   cout << "fileName: " << fileName << endl;
  //   cout << "imageID: " << imageID << endl;
  //   cout << "imageWidth: " << imageWidth << endl;
  //   cout << "imageHeight: " << imageHeight << endl;
  //   cout << "imageData: " << imageData << endl;
  // }

  // glutInit(&argc, argv);
  // glutInitContextVersion(3, 1);
  // glutInitContextProfile (GLUT_CORE_PROFILE );
  // glewExperimental = GL_TRUE;
  // glewInit();

  // if (glewIsSupported("GL_VERSION_1_0"))
  //   printf("Ready for OpenGL 1.0\n");
  // else {
  //   printf("OpenGL 1.0 not supported\n");
  //   return(1);
  // }

  GerenciadorGrafico gerenciador;

  Mesh *mesh;

  for(int i = 1; i < argc; i++)
  {
    mesh = new Mesh();
    mesh->carregarArquivo(argv[i]);
    mesh->prepareAllTexture();
    mesh->redimensionar();
    mesh->transladar();

    vectorMesh.push_back(mesh);
  }
  

  gerenciador.setViewPort(800, 600);
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

  if(sdlEvent.key.keysym.sym == SDLK_w)
  {
    if(sdlEvent.key.state == SDL_PRESSED)
      velocityCameraAngleY = VELOCITY_ANGLE;
    else
      velocityCameraAngleY = 0;
  }

  if(sdlEvent.key.keysym.sym == SDLK_s)
  {
    if(sdlEvent.key.state == SDL_PRESSED)
      velocityCameraAngleY = -VELOCITY_ANGLE;
    else
      velocityCameraAngleY = 0;
  }

  if(sdlEvent.key.keysym.sym == SDLK_d)
  {
    if(sdlEvent.key.state == SDL_PRESSED)
      velocityCameraAngleX = VELOCITY_ANGLE;
    else
      velocityCameraAngleX = 0;
  }

  if(sdlEvent.key.keysym.sym == SDLK_a)
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
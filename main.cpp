#include "vec.h"
#include "CG.h"
#include "mesh.h"
#include "shader.h"

int main(int argc, char* argv[])
{
  if(argc != 3)
  {
    cout << "argc: " << argc << endl;
    cout << "Deve-se passar o nome do arquivo .off juntamente com o nome do programa!!!" << endl;
    exit(-1);
  }

  GerenciadorGrafico gerenciador;
  Mesh mesh[2];

  gerenciador.setViewPort(800, 600);
  gerenciador.iniciarRender();

  glewInit();

  mesh[0].carregarArquivo(argv[1]);
  mesh[0].redimensionar();
  mesh[0].transladar();

  mesh[1].carregarArquivo(argv[2]);
  mesh[1].redimensionar();
  mesh[1].transladar();


  GerenciadorShader shader;

  shader.setVertexShader("vertex.glsl");
  shader.setFragmentShader("fragment.glsl");

  GLuint program = shader.initShader();
  cout<< "TUDO OK!" << endl;
  glUseProgram(program);
  glClearColor(1.0, 1.0, 1.0, 1.0);

  mesh[0].setZMax(mesh[1].getZMax() + 0.9);

  shader.addMesh(mesh[0]);
  shader.addMesh(mesh[1]);

  SDL_Event sdlEvent;
  bool finish;

  finish = false;
  while(!finish)
  {
    while(SDL_PollEvent(&sdlEvent))
    {
      if(sdlEvent.type == SDL_QUIT)
      {
        finish = true;
        break;
      }
    }
    shader.renderizarCena();
    gerenciador.displayRender();
  }

  gerenciador.delay(2000);

  return 0;
}


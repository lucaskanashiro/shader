#include "vec.h"
#include "CG.h"
#include "mesh.h"
#include "shader.h"

int main(int argc, char* argv[])
{
  if(argc != 2)
  {
    cout << "Deve-se passar o nome do arquivo .off juntamente com o nome do programa!!!" << endl;
    exit(-1);
  }

  GerenciadorGrafico gerenciador;
  Mesh mesh;

  gerenciador.setViewPort(800, 600);
  gerenciador.iniciarRender();

  glewInit();

  mesh.carregarArquivo(argv[1]);

  mesh.redimensionar();
  mesh.transladar();


  GerenciadorShader shader;

  shader.setVertexShader("vertex.glsl");
  shader.setFragmentShader("fragment.glsl");
  shader.setVertex(mesh.getVertex());
  shader.setIndice(mesh.getIndice());

  GLuint program = shader.initShader();
  cout<< "TUDO OK!" << endl;
  glUseProgram(program);
  glClearColor(1.0, 1.0, 1.0, 1.0);
  shader.criarBufferDeVertex();
  shader.criarBufferDeIndex();

  shader.renderizarCena();
  gerenciador.displayRender();
  gerenciador.delay(2000);

  return 0;
}


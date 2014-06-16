all:
	g++ -o prog main.cpp CG.cpp mesh.cpp shader.cpp -W -Wall -pedantic -ansi -lGL -lGLEW -lGLU -lSDL2
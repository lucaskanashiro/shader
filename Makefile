all:
	g++ -o prog main.cpp CG.cpp mesh.cpp shader.cpp camera.cpp -W -Wall -pedantic -ansi -lGL -lGLEW -lGLU -lglut -lSDL2
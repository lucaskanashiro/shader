all:
	g++ -o prog main.cpp CG.cpp mesh.cpp shader.cpp -W -Wall -pedantic -ansi -lGL -lGLEW -lGLU -lglut -lSDL2 -lIL -lILU

run:
	cat start_objects.txt | optirun ./prog
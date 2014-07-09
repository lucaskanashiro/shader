all:
	g++ -o prog main.cpp CG.cpp mesh.cpp shader.cpp -W -Wall -pedantic -ansi -lGL -lGLEW -lGLU -lSDL2

run:
	optirun ./prog Models/Apple.off Models/bun_zipper.off Models/space_shuttle.off Models/space_station.off
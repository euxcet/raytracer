all: main

scene.o: scene.cpp *.h
	g++ -c scene.cpp

raytracer.o: raytracer.cpp *.h
	g++ -c raytracer.cpp


main: main.cpp scene.o raytracer.o *.h
	g++ main.cpp scene.o raytracer.o -o main

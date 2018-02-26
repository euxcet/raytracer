all: main


primitive.o: primitive.cpp *.h
	g++ -c primitive.cpp

scene.o: scene.cpp *.h
	g++ -c scene.cpp

raytracer.o: raytracer.cpp *.h
	g++ -c raytracer.cpp


main: main.cpp scene.o raytracer.o primitive.o *.h
	g++ main.cpp scene.o raytracer.o primitive.o -o main

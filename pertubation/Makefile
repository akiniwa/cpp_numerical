satellite: kepler.o libsatellite.so
	g++ -o satellite kepler.o -framework GLUT -framework OpenGL -L. -lsatellite
	rm kepler.o
	rm satellite.o
	rm particle.o
	./satellite file rule
	R --vanilla --slave < rot.R
kepler.o: kepler.cpp
	g++ -c kepler.cpp
libsatellite.so: satellite.o particle.o
	g++ -shared satellite.o particle.o -o libsatellite.so
satellite.o: satellite.cpp
	g++ -c satellite.cpp
particle.o: particle.cpp
	g++ -c particle.cpp

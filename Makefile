OBJECTS = main.o dominus.o particle.o
CFLAGS = -o sim -lsfml-graphics -lsfml-window -lsfml-system -Wall -O2

sim: $(OBJECTS)
	@g++ $(OBJECTS) $(CFLAGS)
	@del *.o

main.o: main.cpp
	@g++ -c main.cpp

dominus.o: Implementation/dominus.cpp
	@g++ -c Implementation/dominus.cpp

particle.o: Implementation/particle.cpp
	@g++ -c Implementation/particle.cpp
all:
	g++ -I ./include -lglfw -lGL -lpthread -lXrandr -lXi -ldl ./src/*.cpp ./src/*.c

clean:
	rm a.out

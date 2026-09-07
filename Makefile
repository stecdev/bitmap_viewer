bmpview: main.c
	gcc main.c -o bmpview -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

clean:
	rm -rf *.o
	rm -rf *.so
	rm -rf bmpview
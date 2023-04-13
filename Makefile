paint.out : main.o canvas.o input.o
	gcc -g -Wall -Werror -o paint.out main.o canvas.o input.o

main.o : main.c  canvas.h input.h
	gcc -g -Wall -Werror -c main.c

input.o : input.c canvas.h input.h
	gcc -g -Wall -Werror -c input.c

canvas.o : canvas.c canvas.h
	gcc -g -Wall -Werror -c canvas.c

clean:
	rm -fr *.o *.out
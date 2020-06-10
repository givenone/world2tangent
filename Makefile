CC = gcc
OBJS = tangent.o obj.o

all: ${OBJS}
	${CC} ${OBJS} -o tangent -lm

obj.o: obj.cpp
	${CC} -c obj.cpp	

tangent.o: tangent.cpp
	${CC} -c tangent.cpp

clean:
	-rm -rf *.o
	-rm test
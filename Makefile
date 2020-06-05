CC = g++
OBJS = tangent.o

all: ${OBJS}
	${CC} ${OBJS} -o tangent
	
%.o: %.cpp
	${CC} -c $*.cpp
	
clean:
	-rm -rf *.o
	-rm tangent
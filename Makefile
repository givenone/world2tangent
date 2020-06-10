CC = gcc
OBJS = tangent.o obj.o

all: ${OBJS}
	${CC} ${OBJS} -o tangent

obj.o: obj.cpp
	${CC} -c obj.cpp	

tangent.o: tangent.cpp tangent.hpp
	${CC} -c tangent.cpp

	
clean:
	-rm -rf *.o
	-rm tangent
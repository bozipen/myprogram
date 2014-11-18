CC=gcc -g -m64
CPP=g++ -g -m64
INC=-I. -I/usr/include -I/usr/local/include
LIBS=
CFLAGS=$(INC)
CPPFLAGS=$(INC)

.SUFFIXES: .c .o .cpp

.c.o:
	$(CC) $(CFLAGS) -c  $*.c

.cpp.o:
	$(CPP) $(CPPFLAGS) -c $*.cpp


OBJS=main.o arma.o

TARGET=arma

all:$(TARGET)
$(TARGET): $(OBJS)
	$(CPP) -o $(TARGET) $(CPPFLAGS) $(LIBS) $(OBJS)

clean:
	rm *.o $(TARGET)
	

install:
	cp $(TARGET) $(APPDIR)

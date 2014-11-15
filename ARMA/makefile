include $(ZHJKDIR)/include/makefile.tux

OBJS=main.o arma.o

TARGET=arma

all:$(TARGET)
$(TARGET): $(OBJS)
	$(CPP) -o $(TARGET) $(CPPFLAGS) $(OBJS) $(LIBS)
#	-lstrtools  -lmyoracomm


clean:
	rm *.lis *.cc *.o $(TARGET)
	

install:
	cp $(TARGET) $(APPDIR)
	
	
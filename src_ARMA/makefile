include $(ZHJKDIR)/include/makefile.comm

OBJS=main.o arma.o

TARGET=arma

all:$(TARGET)
$(TARGET): $(OBJS)
	$(CPP) -o $(TARGET) $(CPPFLAGS) $(LIBS) $(OBJS) \
#	-lstrtools  -lmyoracomm


clean:
	rm *.o $(TARGET)
	

install:
	cp $(TARGET) $(APPDIR);
	
	
CXXFLAGS =	-g3 -Wall -fmessage-length=0 #-Werror

OBJS =		 Board.o util.o game.o character.o ghost.o blinky.o pinky.o inky.o pacman.o bomb.o point.o clyde.o 

LIBS = -L/usr/X11R6/lib -L/sw/lib -L/usr/sww/lib -L/usr/sww/bin -L/usr/sww/pkg/Mesa/lib -lglut -lGLU -lGL -lX11 -lfreeimage -pthread



TARGET =	game


$(TARGET):	$(OBJS) 
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

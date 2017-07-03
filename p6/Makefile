CFLAGS := -ansi -pedantic -Wall -lm -g
INCLUDE := ~/lemon/include/
LIBS := ~/lemon/lib/
LFLAGS := -lemon -lglpk
SRC := lab6.cpp
TARGET := lab

all: $(SRC)
	g++ $(CFLAGS) -I $(INCLUDE) -L $(LIBS) $(SRC) -o $(TARGET) $(LFLAGS)

run:
	./testador.sh

clean:
	rm $(TARGET)

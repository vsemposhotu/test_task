LIB_NAME = libloglib.so
PROG_NAME = Logger
CXX = g++

all: build_lib build_prog

build_lib:
	$(CXX) -fPIC -shared src/loglib.cpp -Iinclude -o $(LIB_NAME)

build_prog:
	$(CXX) src/Logger.cpp -o $(PROG_NAME) -Iinclude -L. -lloglib -lpthread

clean:
	rm -f $(LIB_NAME) $(PROG_NAME) *.txt *.o

test1:
	LD_LIBRARY_PATH=. ./$(PROG_NAME) test1.txt LOW

test2:
	LD_LIBRARY_PATH=. ./$(PROG_NAME) test2.txt
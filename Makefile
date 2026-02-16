LIB_NAME = loglib.so
PROG_NAME = Logger
CXX = g++

all: build_lib build_prog

build_lib:
	$(CXX) -fPIC -shared src/loglib.cpp -Iinclude -o $(LIB_NAME)

build_log:
	$(CXX) src/Logger.cpp -o $(PROG_NAME) -Iinclude -L. -lloglib -lpthread

clean:
	rm -f $(LIB_NAME) $(PROG_NAME) *.txt *.o
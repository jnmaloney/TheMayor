CC=emcc
SOURCES:=main.cpp $(wildcard src/*.cpp)
LDFLAGS=-O2 --llvm-opts 2
OUTPUT=out/themayor.js

#-s USE_WEBGL2=1 

all: $(SOURCES) $(OUTPUT)

$(OUTPUT): $(SOURCES) 
	$(CC) -Isrc -I/usr/local/include/ $(SOURCES) --bind -s FULL_ES3=1 -std=c++11 $(LDFLAGS) -o $(OUTPUT) -s WASM=1 -s USE_GLFW=3 -s ALLOW_MEMORY_GROWTH=1 -s ASSERTIONS=1 -s USE_WEBGL2=1 -lGLU -lGL --preload-file data

clean:
	rm $(OUTPUT)
	

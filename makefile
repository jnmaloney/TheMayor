CC=emcc
SOURCES:=main.cpp $(wildcard src/*.cpp) $(wildcard ../WebCore/src/*.cpp) $(wildcard ../WebCore/imgui/*.cpp)
LDFLAGS=-O2 --llvm-opts 2
OUTPUT=out/themayor.js
USE_IMGUI=-I../WebCore/imgui/
USE_VORBIS=-s USE_VORBIS=1
USE_LIBPNG=-s USE_LIBPNG=1 -s USE_ZLIB=1 
USE_WEBGL=-s FULL_ES3=1 -s USE_GLFW=3 -s USE_WEBGL2=1 -lGLU -lGL
USE_ENGINE_WEBCORE=-I../WebCore/include/
USE_ASYNC_FILES=-s EMTERPRETIFY=1 -s EMTERPRETIFY_ASYNC=1
#-s USE_WEBGL2=1 

all: $(SOURCES) $(OUTPUT)

$(OUTPUT): $(SOURCES)
	$(CC) -Isrc -I/usr/local/include/ $(SOURCES) $(USE_IMGUI) -std=c++1z $(USE_VORBIS) $(USE_LIBPNG) $(USE_WEBGL) $(USE_ENGINE_WEBCORE) --bind $(LDFLAGS) -o $(OUTPUT) -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s ASSERTIONS=1 --preload-file data 

clean:
	rm $(OUTPUT)
	

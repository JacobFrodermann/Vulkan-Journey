CC = g++
#CFLAGS = -std=c++17 -O2 

#LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lwayland-client++ -lwayland-cursor -lxkbcommon 

CFLAGS = -std=c++17 -O2 $(shell pkg-config --cflags glfw3 wayland-client xkbcommon)
LDFLAGS = $(shell pkg-config --libs glfw3 wayland-client xkbcommon) -lvulkan -ldl -lpthread


export LIBDECOR_PLUGIN=none
export LIBDECOR_DISABLE=1
export GLFW_WAYLAND_NO_LIBDECOR=1

.PHONY: shaders
shaders:
	glslc shaders/simple_shader.frag -o shaders/simple_shader.frag.spv
	glslc shaders/simple_shader.vert -o shaders/simple_shader.vert.spv

all: 
	$(CC) $(CFLAGS) src/*.cpp -o main $(LDFLAGS)
run: shaders all
	echo iran
#	./main
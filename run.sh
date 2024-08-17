set -o errexit
g++ -o shader_example shader.cpp -lglfw -lGLEW -lGL
./shader_example

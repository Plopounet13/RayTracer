EXEC=renderer
FILES = main Camera Image Intersection Light Material Object Ray Scene Sphere utilities Vec3 Mesh
OBJECTS=$(FILES:=.o)
CXXFLAGS=-O3 -std=c++11 -Wall -fopenmp
OTHER_LDFLAGS = -L/usr/local/opt/llvm/lib -I/usr/local/opt/llvm/include -fopenmp
CXX=/usr/local/opt/llvm/bin/clang++
all: $(EXEC)

renderer: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OTHER_LDFLAGS) $(OBJECTS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJECTS)

mrproper: clean
	$(RM) $(EXEC)


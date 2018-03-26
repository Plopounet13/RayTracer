EXEC=renderer
SOURCEDIR=Sources
HEADERDIR=Includes
OBJDIR=Build
FILES = main Camera Image Intersection Light Material Object Ray Scene Sphere utilities Vec3 Mesh BBox BVH
OBJECTS=$(addprefix $(OBJDIR)/, $(FILES:=.o))
CXXFLAGS=-O3 -std=c++14 -Wall -fopenmp -I$(HEADERDIR)
OTHER_LDFLAGS = -L/usr/local/opt/llvm/lib -I/usr/local/opt/llvm/include -fopenmp
CXX=/usr/local/opt/llvm/bin/clang++
all: $(EXEC)

renderer: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OTHER_LDFLAGS) $(OBJECTS) -o $@

$(OBJDIR)/%.o: $(SOURCEDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJECTS)

mrproper: clean
	$(RM) $(EXEC)


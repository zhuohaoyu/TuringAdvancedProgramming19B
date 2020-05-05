CXX=`llvm-config --bindir`/clang
CXXFLAGS=`llvm-config --cxxflags` -shared -fPIC
LDFLAGS=`llvm-config --ldflags`

all: MyPass.so

MyPass.so: MyPass.cpp MyPass.h SimpleDataDependenceGraph.cpp SimpleDataDependenceGraph.h
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ MyPass.cpp SimpleDataDependenceGraph.cpp

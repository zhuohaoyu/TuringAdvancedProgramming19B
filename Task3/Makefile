CXX=`llvm-config --bindir`/clang
CXXFLAGS=`llvm-config --cxxflags` -shared -fPIC
LDFLAGS=`llvm-config --ldflags`

all: MyPass.so

MyPass.so: SimpleDataDependenceGraph.cpp SimpleDataDependenceGraph.h MyPass.cpp MyPass.h apriori.cpp rule.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ SimpleDataDependenceGraph.cpp MyPass.cpp apriori.cpp rule.cpp

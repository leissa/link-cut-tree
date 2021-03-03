#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>  
#include <crtdbg.h>
#include <LinkCutTree.h>
#include <vector>
#include <map>
#include "./TestUtils.cpp"

void randomLCT() {
	std::map<Node*, std::vector<Node*>> backpointers;
	std::vector<std::vector<Node*>>* v = createRandomLCT(4, 5, 10, backpointers);
	LinkCutTree<int>::printSplayTree(v->at(0)[0], &backpointers);
	for (int i = 0; i < v->size(); i++) {
		deleteNodes(&(v->at(i)));
	}
	delete v;
}

int main() {
	srand(time(NULL));
	randomLCT();
	_CrtDumpMemoryLeaks();
}
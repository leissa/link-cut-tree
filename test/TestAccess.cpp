#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>  
#include <crtdbg.h>
#include <LinkCutTree.h>
#include <vector>
#include <map>
#include "Utils.h"

void randomLCT() {
	/*
	std::map<Node*, std::vector<Node*>> backpointers;
	std::vector<Node*> v = createRandomLCT(4, 5, 10);
	updateBackpointers(v, backpointers);
	LinkCutTree<int>::printLCT(v[0], &backpointers);
	int r = rand() % v.size();
	std::cout << "\nAccess node with value " << v[r]->key << std::endl << std::endl;
	LinkCutTree<int>::access(v[r]);
	updateBackpointers(v, backpointers);
	LinkCutTree<int>::printLCT(v[0], &backpointers);
	deleteNodes(&v);
	*/
}

int main() {
	srand(time(NULL));
	randomLCT();
	_CrtDumpMemoryLeaks();
}

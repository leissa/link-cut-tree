#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <LinkCutTree.h>
#include <vector>
#include "Utils.h"

// make test classes friend of LinkCutTree instead
class TestSplay : LinkCutTree<int> {
public:
	static void randomSplay() {
		std::vector<Node*> a = createRandomSplayTree(20, 30, 0);
		Node* n = LinkCutTree<int>::createTree(-1);
		Node* temp = a[0];
		while (temp->parent) temp = temp->parent;
		temp->parent = n;
		LinkCutTree<int>::printSplayTree(a[0]);
		int r = rand() % a.size();
		std::cout << "\nSplaying Node with value " << a[r]->key << "\n" << std::endl;
		LinkCutTree<int>::splay(a[r]);
		LinkCutTree<int>::printSplayTree(a[0]);
		deleteNodes(&a);
	}
};

int main() {
	srand(time(NULL));
	TestSplay::randomSplay();
	_CrtDumpMemoryLeaks();
}





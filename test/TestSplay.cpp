#include <LinkCutTree.h>
#include <vector>
#include <ctime>
#include "./TestUtils.cpp"

// make test classes friend of LinkCutTree instead
class TestSplay : LinkCutTree<int> {
public:
	static void randomSplay() {
		std::vector<Node*> a = createRandomSplayTree(20, 30);
		LinkCutTree<int>::printSplayTree(a[0]);
		std::cout << a[25]->key << std::endl;
		LinkCutTree<int>::splay(a[25]);
		LinkCutTree<int>::printSplayTree(a[0]);
		deleteNodes(&a);
	}

};

int main() {
	TestSplay::randomSplay();
}





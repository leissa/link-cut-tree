#include <iostream>
#include <string>
#include "LinkCutTree.h"
#include "Utils.h"
#include "Node.h"

int main()
{
	LinkCutTree<int> lLct;
	std::vector<Node<int>*> lNodes;
	std::map<Node<int>*, std::vector<Node<int>*>> lBackpointers;
	for (int i = 0; i < 100; i++) {
		lNodes.push_back(lLct.createTree(i, i));
	}
	int lX, lY;
	while (true) {
		std::string lInput;
		std::cin >> lInput;
		if (std::cin.fail()) {
			exit(0);
		}
		if (lInput.compare("repr") == 0) {
			std::cin >> lX;
			updateBackpointers(lNodes, lBackpointers);
			lLct.printReprTree(lLct[lX], &lBackpointers);
		}
		else if (lInput.compare("lct") == 0) {
			std::cin >> lX;
			updateBackpointers(lNodes, lBackpointers);
			lLct.printLCT(lLct[lX], &lBackpointers);
		}
		else if (lInput.compare("link") == 0) {
			std::cin >> lX;
			std::cin >> lY;
			lLct[lX]->link(lLct[lY]);
		}
		else if (lInput.compare("ll") == 0) {
			std::cin >> lX;
			std::cin >> lY;
			lLct[lX]->linkLeft(lLct[lY]);
		}
		else if (lInput.compare("lr") == 0) {
			std::cin >> lX;
			std::cin >> lY;
			lLct[lX]->linkRight(lLct[lY]);
		}
		else if (lInput.compare("cut") == 0) {
			std::cin >> lX;
			lLct[lX]->cut();
		}
		else if (lInput.compare("expose") == 0) {
			std::cin >> lX;
			lLct[lX]->expose();
		}
		else if (lInput.compare("root") == 0) {
			std::cin >> lX;
			std::cout << lLct[lX]->findRoot()->getKey() << std::endl;
		}
		else if (lInput.compare("lca") == 0) {
			std::cin >> lX;
			std::cin >> lY;
			auto lLca = lLct[lX]->lowestCommonAncestor(lLct[lY]);
			std::cout << (lLca ? lLca->getID() : -1) << std::endl;
		}
		else if (lInput.compare("info") == 0) {
			std::cin >> lX;
			for (int i = 0; i < 4; i++) {
				std::cout << lLct[lX]->getFlag(static_cast<Node<int>::flagType>(3 - i));
			}
			std::cout << std::endl;
		}
		else if (lInput.compare("random") == 0) {
			lLct = createRandomLCT(15, &lNodes);
		}
		else if (lInput.compare("parent") == 0) {
			std::cin >> lX;
			auto lRoot = lLct[lX]->findParent();
			std::cout << (lRoot ? lRoot->getID() : -1) << std::endl;
		}
		else if (lInput.compare("printPath") == 0) {
			std::cin >> lX;
			lLct[lX]->path([](Node<int>* aNode)
				{
					std::cout << aNode->getID() << std::endl;
				});
		}
	}
}

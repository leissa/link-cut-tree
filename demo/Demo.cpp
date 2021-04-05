#include <iostream>
#include <string>
#include "Utils.h"
#include "LctNode.h"
#include "OpTreeNode.h"
#include "LinkCutTree.h"

int main()
{
	LinkCutTree<int, OpTreeNode> lLct;
	std::vector<LctNode<int>*> lNodes;
	std::map<LctNode<int>*, std::vector<LctNode<int>*>> lBackpointers;
	for (int i = 0; i < 100; i++) {
		lNodes.push_back(lLct.createTree(i, i));
	}
	int lX, lY;
	srand(time(nullptr));
	while (true) {
		std::string lInput;
		std::cin >> lInput;
		if (std::cin.fail()) {
			exit(0);
		}
		if (lInput.compare("repr") == 0) {
			std::cin >> lX;
			updateBackpointers(lNodes, lBackpointers);
			printReprTree(lLct[lX], &lBackpointers);
		}
		else if (lInput.compare("lct") == 0) {
			std::cin >> lX;
			updateBackpointers(lNodes, lBackpointers);
			printLCT(lLct[lX], &lBackpointers);
		}
		else if (lInput.compare("link") == 0) {
			std::cin >> lX;
			std::cin >> lY;
			//lLct[lX]->link(lLct[lY]);
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
		else if (lInput.compare("lo") == 0) {
			std::cin >> lX;
			std::cin >> lY;
			lLct[lX]->linkOnly(lLct[lY]);
		}
		else if (lInput.compare("cut") == 0) {
			std::cin >> lX;
			lLct[lX]->cut();
		}
		else if (lInput.compare("root") == 0) {
			std::cin >> lX;
			std::cout << lLct[lX]->findRoot()->getContent() << std::endl;
		}
		else if (lInput.compare("lca") == 0) {
			std::cin >> lX;
			std::cin >> lY;
			auto lLca = lLct[lX]->lowestCommonAncestor(lLct[lY]);
			std::cout << (lLca ? lLca->getID() : -1) << std::endl;
		}
		else if (lInput.compare("info") == 0) {
			std::cin >> lX;
			for (int i = 0; i < 6; i++) {
				std::cout << lLct[lX]->getFlag(static_cast<OpTreeNode<int>::flagType>(5 - i));
			}
			std::cout << std::endl;
		}
		else if (lInput.compare("random") == 0) {
			std::cin >> lX;
			lNodes.clear();
			//lLct = createRandomLCT(lX, &lNodes);
		}
		else if (lInput.compare("parent") == 0) {
			std::cin >> lX;
			auto lRoot = lLct[lX]->findParent();
			std::cout << (lRoot ? lRoot->getID() : -1) << std::endl;
		}
		else if (lInput.compare("printPath") == 0) {
			std::cin >> lX;
			lLct[lX]->path([](LctNode<int>* aNode)
				{
					std::cout << aNode->getID() << std::endl;
				});
		}
		else if (lInput.compare("randomJoinTree") == 0) {
			std::cin >> lX; // number of inner nodes
			// std::cin >> lY; // seed in range [0, catalan(n)[
			lNodes.clear();
			lLct = createRandomJoinTree(lX, &lNodes);
			updateBackpointers(lNodes, lBackpointers);
			printReprTree(lLct[1], &lBackpointers);
		}
		else if (lInput.compare("find") == 0) {
			std::cin >> lX;
			std::cin >> lY;
			LctNode<int>* aRes = lLct[lX]->find_if([lY](LctNode<int>* aNode) {
				if (aNode->getID() == lY) {
					return true;
				}
				else {
					return false;
				}
				});
			std::cout << (aRes ? aRes->getID() : -1) << std::endl;
		}
	}
}
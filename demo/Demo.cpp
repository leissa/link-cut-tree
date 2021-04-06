#include <iostream>
#include <string>
#include "Utils.h"
#include "LctNode.h"
#include "OpTreeNode.h"
#include "LinkCutTree.h"

template<typename T> bool query(std::string& aCmd, T& aLct, std::vector<LctNode<int>*>& aNodes, std::map<LctNode<int>*, std::vector<LctNode<int>*>>& aBackpointers) {
	int lX, lY;
	std::cin >> aCmd;
	if (aCmd.compare("repr") == 0) {
		std::cin >> lX;
		updateBackpointers(aNodes, aBackpointers);
		printReprTree(aLct[lX], &aBackpointers);
	}
	else if (aCmd.compare("lct") == 0) {
		std::cin >> lX;
		updateBackpointers(aNodes, aBackpointers);
		printLCT(aLct[lX], &aBackpointers);
	}
	else if (aCmd.compare("cut") == 0) {
		std::cin >> lX;
		aLct[lX]->cut();
	}
	else if (aCmd.compare("root") == 0) {
		std::cin >> lX;
		std::cout << aLct[lX]->findRoot()->getID() << std::endl;
	}
	else if (aCmd.compare("lca") == 0) {
		std::cin >> lX;
		std::cin >> lY;
		auto lLca = aLct[lX]->lowestCommonAncestor(aLct[lY]);
		std::cout << (lLca ? lLca->getID() : -1) << std::endl;
	}
	else if (aCmd.compare("parent") == 0) {
		std::cin >> lX;
		auto lRoot = aLct[lX]->findParent();
		std::cout << (lRoot ? lRoot->getID() : -1) << std::endl;
	}
	else if (aCmd.compare("printPath") == 0) {
		std::cin >> lX;
		aLct[lX]->path([](LctNode<int>* aNode)
			{
				std::cout << aNode->getID() << std::endl;
			});
	}
	else if (aCmd.compare("find") == 0) {
		std::cin >> lX;
		std::cin >> lY;
		LctNode<int>* aRes = aLct[lX]->find_if([lY](LctNode<int>* aNode) {
			if (aNode->getID() == lY) {
				return true;
			}
			else {
				return false;
			}
			});
		std::cout << (aRes ? aRes->getID() : -1) << std::endl;
		return true;
	}
	else {
		return true;
	}
	return false;
}

void loopDefault() {
	LinkCutTree<int> lLct;
	std::vector<LctNode<int>*> lNodes;
	std::map<LctNode<int>*, std::vector<LctNode<int>*>> lBackpointers;
	lLct = createRandomLCT(25, &lNodes);
	updateBackpointers(lNodes, lBackpointers);
	printReprTree(lLct[0], &lBackpointers);
	std::string lCmd;
	while (true) {
		int lX, lY;
		if (query(lCmd, lLct, lNodes, lBackpointers)) {
			if(lCmd.compare("link") == 0) {
				std::cin >> lX;
				std::cin >> lY;
				lLct[lX]->link(lLct[lY]);
			}else if (lCmd.compare("random") == 0) {
				std::cin >> lX;
				lNodes.clear();
				lLct = createRandomLCT(lX, &lNodes);
				updateBackpointers(lNodes, lBackpointers);
				printReprTree(lLct[0], &lBackpointers);
			}
			else {
				std::cout << "invalid command" << std::endl;
			}
		}
		std::cin.ignore(INT_MAX, '\n');
	}
}

void loopOpTree() {
	LinkCutTree<int, OpTreeNode> lLct;
	std::vector<LctNode<int>*> lNodes;
	std::map<LctNode<int>*, std::vector<LctNode<int>*>> lBackpointers;
	std::string lCmd;
	lLct = createRandomJoinTree(25, &lNodes);
	updateBackpointers(lNodes, lBackpointers);
	printReprTree(lLct[1], &lBackpointers);
	while (true) {
		int lX, lY;
		if (query(lCmd, lLct, lNodes, lBackpointers)) {
			if (lCmd.compare("ll") == 0) {
				std::cin >> lX;
				std::cin >> lY;
				lLct[lX]->linkLeft(lLct[lY]);
			}
			else if (lCmd.compare("lr") == 0) {
				std::cin >> lX;
				std::cin >> lY;
				lLct[lX]->linkRight(lLct[lY]);
			}
			else if (lCmd.compare("lo") == 0) {
				std::cin >> lX;
				std::cin >> lY;
				lLct[lX]->linkOnly(lLct[lY]);
			}
			else if (lCmd.compare("info") == 0) {
				std::cin >> lX;
				for (int i = 0; i < 6; i++) {
					std::cout << lLct[lX]->getFlag(static_cast<OpTreeNode<int>::flagType>(5 - i));
				}
				std::cout << std::endl;
			}
			else if (lCmd.compare("randomJoinTree") == 0) {
				std::cin >> lX; // number of inner nodes
				lNodes.clear();
				lLct = createRandomJoinTree(lX, &lNodes);
				updateBackpointers(lNodes, lBackpointers);
				printReprTree(lLct[1], &lBackpointers);
			}
			else if (lCmd.compare("joinTree") == 0) {
				std::cin >> lX; // number of inner nodes
				std::cin >> lY; // seed
				lNodes.clear();
				lLct = createRandomJoinTree(lX, &lNodes, lY);
				updateBackpointers(lNodes, lBackpointers);
				printReprTree(lLct[1], &lBackpointers);
			}
			else {
				std::cout << "invalid command" << std::endl;
			}
		}
		std::cin.ignore(INT_MAX, '\n');
	}
}

int main()
{
	srand(time(nullptr));
	int lArg;
	while (true) {
		std::cout << "1 - LctNode type nodes\n2 - OpTreeNode type nodes" << std::endl;
		std::cin >> lArg;
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
		}
		else {
			switch (lArg) {
			case 1:
				std::cout << std::endl;
				loopDefault();
				break;
			case 2:
				std::cout << std::endl;
				loopOpTree();
				break;
			}
		}
		std::cout << "Invalid input" << std::endl;
	}
}
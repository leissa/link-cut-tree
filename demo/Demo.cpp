#include <iostream>
#include <string>
#include <ctime>
#include "LctUtils.h"
#include "LctNode.h"
#include "OpTreeNode.h"
#include "LinkCutTree.h"
#include "IntWrapper.h"

template<typename T> bool query(std::string& aCmd, T& aLct, std::vector<LctNode<IntWrapper>*>& aNodes, std::map<LctNode<IntWrapper>*, std::vector<LctNode<IntWrapper>*>>& aBackpointers) {
	int lX, lY;
	std::cin >> aCmd;
	if (aCmd.compare("repr") == 0) {
		std::cin >> lX;
		LctUtils::updateBackpointers(aNodes, aBackpointers);
		LctUtils::printReprTree(aLct[lX], &aBackpointers);
	}
	else if (aCmd.compare("lct") == 0) {
		std::cin >> lX;
		LctUtils::updateBackpointers(aNodes, aBackpointers);
		LctUtils::printLCT(aLct[lX], &aBackpointers);
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
		aLct[lX]->path([](LctNode<IntWrapper>* aNode)
			{
				std::cout << aNode->getID() << std::endl;
			});
	}
	else if (aCmd.compare("find") == 0) {
		std::cin >> lX;
		std::cin >> lY;
		LctNode<IntWrapper>* aRes = aLct[lX]->find_if([lY](LctNode<IntWrapper>* aNode) {
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
	else if(aCmd.compare("isDesc") == 0) {
		std::cin >> lX;
		std::cin >> lY;
		std::cout << (aLct[lX]->isDescendant(aLct[lY]) ? "true" : "false") << std::endl;
	}
	else if (aCmd.compare("vs") == 0) {
		std::cin >> lX;
		std::cout << aLct[lX]->getVirtualSize() << std::endl;
	}
	else if (aCmd.compare("rs") == 0) {
		std::cin >> lX;
		std::cout << aLct[lX]->getRealSize() << std::endl;
	}
	else {
		return true;
	}
	return false;
}

void loopDefault() {
	LinkCutTree<IntWrapper> lLct;
	std::vector<LctNode<IntWrapper>*> lNodes;
	std::map<LctNode<IntWrapper>*, std::vector<LctNode<IntWrapper>*>> lBackpointers;
	lLct = LctUtils::createRandomLCT(25, &lNodes);
	LctUtils::updateBackpointers(lNodes, lBackpointers);
	LctUtils::printReprTree(lLct[0], &lBackpointers);
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
				lLct = LctUtils::createRandomLCT(lX, &lNodes);
				LctUtils::updateBackpointers(lNodes, lBackpointers);
				LctUtils::printReprTree(lLct[0], &lBackpointers);
			}
			else {
				std::cout << "invalid command" << std::endl;
			}
		}
		std::cin.ignore(INT_MAX, '\n');
	}
}

void loopOpTree() {
	LinkCutTree<IntWrapper, OpTreeNode> lLct;
	std::vector<LctNode<IntWrapper>*> lNodes;
	std::map<LctNode<IntWrapper>*, std::vector<LctNode<IntWrapper>*>> lBackpointers;
	std::string lCmd;
	lLct = LctUtils::createRandomJoinTree(25, &lNodes);
	LctUtils::updateBackpointers(lNodes, lBackpointers);
	LctUtils::printReprTree(lLct[1], &lBackpointers);
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
					std::cout << lLct[lX]->getFlag(static_cast<OpTreeNode<IntWrapper>::flagType>(5 - i));
				}
				std::cout << std::endl;
			}
			else if (lCmd.compare("randomJoinTree") == 0) {
				std::cin >> lX; // number of inner nodes
				lNodes.clear();
				lLct = LctUtils::createRandomJoinTree(lX, &lNodes);
				LctUtils::updateBackpointers(lNodes, lBackpointers);
				LctUtils::printReprTree(lLct[1], &lBackpointers);
			}
			else if (lCmd.compare("joinTree") == 0) {
				std::cin >> lX; // number of inner nodes
				std::cin >> lY; // seed
				lNodes.clear();
				lLct = LctUtils::createRandomJoinTree(lX, &lNodes, lY);
				LctUtils::updateBackpointers(lNodes, lBackpointers);
				LctUtils::printReprTree(lLct[1], &lBackpointers);
			}
			else if (lCmd.compare("flat") == 0) {
				std::cin >> lX;
				lLct = LinkCutTree<IntWrapper, OpTreeNode>();
				lNodes.clear();
				for (int i = 0; i < lX; i++) {
					lNodes.push_back(lLct.createTree(i, i));
				}
				LctUtils::updateBackpointers(lNodes, lBackpointers);
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
		std::cout << "1 - LctNode type nodes\n2 - OpTreeNode type nodes\n3 - benchmark" << std::endl;
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
			case 3:
				break;
			}
		}
		std::cout << "Invalid input" << std::endl;
	}
}
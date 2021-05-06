#include <iostream>
#include <string>
#include <ctime>
#include "LctUtils.h"
#include "LctNode.h"
#include "OpTreeNode.h"
#include "LinkCutTree.h"
#include "TrivialTreeNode.h"
#include "TrivialTree.h"
#include <chrono>

typedef std::chrono::high_resolution_clock Clock;

template<typename T> bool query(std::string& aCmd, T& aLct, std::vector<LctNode<int>*>& aNodes, std::map<LctNode<int>*, std::vector<LctNode<int>*>>& aBackpointers) {
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
	}
	else if (aCmd.compare("isDesc") == 0) {
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
	else if (aCmd.compare("depth") == 0) {
		std::cin >> lX;
		std::cout << aLct[lX]->getDepth() << std::endl;
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
	lLct = LctUtils::createRandomLCT(25, &lNodes);
	LctUtils::updateBackpointers(lNodes, lBackpointers);
	LctUtils::printReprTree(lLct[1], &lBackpointers);
	std::string lCmd;
	while (true) {
		int lX, lY;
		if (query(lCmd, lLct, lNodes, lBackpointers)) {
			if (lCmd.compare("link") == 0) {
				std::cin >> lX;
				std::cin >> lY;
				lLct[lX]->link(lLct[lY]);
			}
			else if (lCmd.compare("random") == 0) {
				std::cin >> lX;
				lNodes.clear();
				lLct = LctUtils::createRandomLCT(lX, &lNodes);
				LctUtils::updateBackpointers(lNodes, lBackpointers);
				LctUtils::printReprTree(lLct[1], &lBackpointers);
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
	lLct = LctUtils::createJoinTree(25, &lNodes);
	LctUtils::updateBackpointers(lNodes, lBackpointers);
	LctUtils::printReprTree(lLct[1], &lBackpointers);
	while (true) {
		uint64_t lX, lY;
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
			else if (lCmd.compare("randomJoinTree") == 0) {
				std::cin >> lX; // number of inner nodes
				lNodes.clear();
				lLct = LctUtils::createJoinTree(lX, &lNodes);
				LctUtils::updateBackpointers(lNodes, lBackpointers);
				LctUtils::printReprTree(lLct[1], &lBackpointers);
			}
			else if (lCmd.compare("joinTree") == 0) {
				std::cin >> lX; // number of inner nodes
				std::cin >> lY; // seed
				lNodes.clear();
				lLct = LctUtils::createJoinTree(lX, &lNodes, lY);
				LctUtils::updateBackpointers(lNodes, lBackpointers);
				LctUtils::printReprTree(lLct[1], &lBackpointers);
			}
			else if (lCmd.compare("flat") == 0) {
				std::cin >> lX;
				lLct = LinkCutTree<int, OpTreeNode>();
				lNodes.clear();
				for (int i = 0; i < lX; i++) {
					lNodes.push_back(lLct.createTree(i, i));
				}
				LctUtils::updateBackpointers(lNodes, lBackpointers);
			}
			else if (lCmd.compare("isLeftDesc") == 0) {
				std::cin >> lX;
				std::cin >> lY;
				std::cout << (lLct[lX]->isLeftDescendant(lLct[lY]) ? "true" : "false") << std::endl;
			}
			else if (lCmd.compare("isRightDesc") == 0) {
				std::cin >> lX;
				std::cin >> lY;
				std::cout << (lLct[lX]->isRightDescendant(lLct[lY]) ? "true" : "false") << std::endl;
			}
			else {
				std::cout << "invalid command" << std::endl;
			}
		}
		std::cin.ignore(INT_MAX, '\n');
	}
}

void benchmark() {
	LinkCutTree<int> lLct;
	TrivialTree aTt;
	const int aRuns = 200;
	const int aQueries = 1000;
	const int aNodes = 1000;
	int lQueries[aQueries];
	int lPerm[aNodes];
	long long lSum1 = 0;
	long long lSum2 = 0;
	std::default_random_engine generator;
	std::exponential_distribution<double> distribution(20);
	for (int i = 0; i < aRuns; i++) {
		aTt.clear();
		lLct = LctUtils::createRandomLCT(aNodes, nullptr, &aTt);
		for (int i = 0; i < aNodes; i++) {
			lPerm[i] = i + 1;
		}
		std::shuffle(lPerm, lPerm + aNodes, generator);
		for (int i = 0; i < aQueries; i++) {
			lQueries[i] = lPerm[(int)(fmod(distribution(generator), 1) * aNodes)];
			//lAr[i] = rand() % aNodes + 1;
		}
		auto start = Clock::now();
		auto end = Clock::now();
		start = Clock::now();
		for (int i = 0; i < aQueries; i++) {
			lLct[lQueries[i]]->findRoot();
		}
		end = Clock::now();
		lSum1 += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		start = Clock::now();
		for (int i = 0; i < aQueries; i++) {
			aTt[lQueries[i]]->findRoot();
		}
		end = Clock::now();
		lSum2 += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	}
	std::cout << lSum1 / (aRuns * 1.0) << " ";
	std::cout << lSum2 / (aRuns * 1.0) << " " << (lSum1 * 1.0 / lSum2) << std::endl;
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
				benchmark();
				break;
			}
		}
		std::cout << "Invalid input" << std::endl;
	}
}
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
			else if (lCmd.compare("pruefer") == 0) {
				std::cin >> lX;
				lNodes.clear();
				lLct = LctUtils::pruefer(lX, &lNodes);
				LctUtils::updateBackpointers(lNodes, lBackpointers);
				LctUtils::printReprTree(lLct[1], &lBackpointers);
			}
			else if (lCmd.compare("ternary") == 0) {
				std::cin >> lX;
				lNodes.clear();
				lLct = LctUtils::ternary(lX, &lNodes);
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

auto runLct(LinkCutTree<int>& aLct, int aQueries[], int aNodeCount = 10000) {
	auto lStart = Clock::now();
	for (int i = 0; i < aNodeCount; i++) {
		aLct[aQueries[i]]->findRoot();
	}
	auto lEnd = Clock::now();
	return std::chrono::duration_cast<std::chrono::microseconds>(lEnd - lStart).count();
}

auto runTrivial(TrivialTree& aTt, int aQueries[], int aNodeCount = 10000) {
	auto lStart = Clock::now();
	for (int i = 0; i < aNodeCount; i++) {
		aTt[aQueries[i]]->findRoot();
	}
	auto lEnd = Clock::now();
	return std::chrono::duration_cast<std::chrono::microseconds>(lEnd - lStart).count();
}

void testRandomNonBinary() {
	LinkCutTree<int> lLct;
	TrivialTree lTt;
	int lNodes[] = { 50, 100, 200, 400, 800, 1600, 3200, 6400, 12800, 25600 };
	//int lNodes[] = { 1000, 4000, 8000, 12000, 16000, 20000, 24000, 28000 };
	int* lQueries;
	long long lSumLinkCut = 0;
	long long lSumTrivial = 0;
	int lRunCount;
	const int lTimeLimit = 60;
	int lQueryCount;
	for (int i = 0; i < 10; i++) {
		lQueryCount = lNodes[i] * 2;
		lRunCount = 0;
		std::cout << lNodes[i] << std::endl;
		auto lStart = Clock::now();
		while (std::chrono::duration_cast<std::chrono::seconds>(Clock::now() - lStart).count() < lTimeLimit) {
			lQueries = new int[lQueryCount];
			lTt.clear();
			lLct = LctUtils::pruefer(lNodes[i], nullptr, &lTt);
			for (int k = 0; k < lQueryCount; k++) {
				lQueries[k] = rand() % lNodes[i] + 1;
			}
			lSumLinkCut += runLct(lLct, lQueries, lQueryCount);
			lSumTrivial += runTrivial(lTt, lQueries, lQueryCount);
			delete[] lQueries;
			lRunCount++;
		}
		std::cout << lSumLinkCut / (lRunCount * 1.0) << " ";
		std::cout << lSumTrivial / (lRunCount * 1.0) << " " << (lSumLinkCut * 1.0 / lSumTrivial) <<
			" " << lRunCount << std::endl;
	}
}

void testTernary() {
	LinkCutTree<int> lLct;
	TrivialTree lTt;
	int lDepth[] = { 3, 4, 5, 6, 7, 8, 9 };
	int* lQueries;
	long long lSumLinkCut = 0;
	long long lSumTrivial = 0;
	int lRunCount;
	int lTimeLimit = 60;
	int lNodes;
	for (int i = 0; i < 7; i++) {
		lLct = LctUtils::ternary(lDepth[i], nullptr, &lTt);
		lRunCount = 0;
		std::cout << lDepth[i] << std::endl;
		auto lStart = Clock::now();
		while (std::chrono::duration_cast<std::chrono::seconds>(Clock::now() - lStart).count() < lTimeLimit) {
			lNodes = (3 * pow(3, lDepth[i]) - 1) / 2;
			lQueries = new int[10000];
			for (int k = 0; k < 10000; k++) {
				lQueries[k] = rand() % lNodes + 1;
			}
			lSumLinkCut += runLct(lLct, lQueries);
			lSumTrivial += runTrivial(lTt, lQueries);
			delete[] lQueries;
			lRunCount++;
		}
		std::cout << lSumLinkCut / (lRunCount * 1.0) << " ";
		std::cout << lSumTrivial / (lRunCount * 1.0) << " " << (lSumLinkCut * 1.0 / lSumTrivial) <<
			" " << lRunCount << std::endl;
	}
}

void benchmark() {
	testRandomNonBinary();
	testTernary();
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
				continue;
			}
		}
		std::cout << "Invalid input" << std::endl;
	}
}
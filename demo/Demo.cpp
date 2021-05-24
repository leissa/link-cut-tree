#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <random>
#include "LctUtils.h"
#include "LctNode.h"
#include "OpTreeNode.h"
#include "LinkCutTree.h"
#include "TrivialTreeNode.h"
#include "TrivialTree.h"

typedef std::chrono::high_resolution_clock Clock;

const int TIME_LIMIT = 240;
const int MIN_RUN_COUNT = 50;

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
			else if (lCmd.compare("binary") == 0) {
				std::cin >> lX;
				lNodes.clear();
				lLct = LctUtils::binary(lX, &lNodes);
				LctUtils::updateBackpointers(lNodes, lBackpointers);
				LctUtils::printReprTree(lLct[1], &lBackpointers);
			}
			else if (lCmd.compare("unbalanced") == 0) {
				std::cin >> lX;
				lNodes.clear();
				lLct = LctUtils::unbalancedBinary(lX, &lNodes);
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

void printRes(std::vector<int> aNodes, std::vector<double> aRes, std::vector<double> aLctPerf, std::vector<double> aTrivialPerf) {
	std::cout << "x= [";
	for (int i = 0; i < aNodes.size(); i++) {
		std::cout << aNodes.at(i) << (i == aNodes.size() - 1 ? "" : ",");
	}
	std::cout << "]" << std::endl;
	std::cout << "y = [";
	for (int i = 0; i < aRes.size(); i++) {
		std::cout << aRes.at(i) << (i == aRes.size() - 1 ? "" : ",");
	}
	std::cout << "]" << std::endl;
	std::cout << "y = [";
	for (int i = 0; i < aRes.size(); i++) {
		std::cout << aLctPerf.at(i) << (i == aRes.size() - 1 ? "" : ",");
	}
	std::cout << "]" << std::endl;
	std::cout << "y = [";
	for (int i = 0; i < aRes.size(); i++) {
		std::cout << aTrivialPerf.at(i) << (i == aRes.size() - 1 ? "" : ",");
	}
	std::cout << "]" << std::endl;
}

auto runLct(LinkCutTree<int>& aLct, int aQueries[], int aNodeCount) {
	auto lStart = Clock::now();
	for (int i = 0; i < aNodeCount; i++) {
		aLct[aQueries[i]]->findRoot();
	}
	auto lEnd = Clock::now();
	return std::chrono::duration_cast<std::chrono::microseconds>(lEnd - lStart).count();
}

auto runTrivial(TrivialTree& aTt, int aQueries[], int aNodeCount) {
	auto lStart = Clock::now();
	for (int i = 0; i < aNodeCount; i++) {
		aTt[aQueries[i]]->findRoot();
	}
	auto lEnd = Clock::now();
	return std::chrono::duration_cast<std::chrono::microseconds>(lEnd - lStart).count();
}

void testRandomNonBinary() {
	std::cout << "random non-binary" << std::endl;
	LinkCutTree<int> lLct;
	TrivialTree lTt;
	std::vector<int> lNodes = { 50, 100, 200, 400, 800, 1600, 3200, 6400, 12800, 25600 };
	for (int i = 2; i <= 32; i += 2) {
		lNodes.push_back(i * 1000);
	}
	std::vector<double> lRes(lNodes.size());
	std::vector<double> lLctPerf(lNodes.size());
	std::vector<double> lTrivialPerf(lNodes.size());
	int* lQueries;
	long long lSumLinkCut;
	long long lSumTrivial;
	int lRunCount;
	for (int i = 0; i < lNodes.size(); i++) {
		lSumLinkCut = 0;
		lSumTrivial = 0;
		lRunCount = 0;
		std::cout << lNodes[i] << std::endl;
		auto lStart = Clock::now();
		while (std::chrono::duration_cast<std::chrono::seconds>(Clock::now() - lStart).count() < TIME_LIMIT || lRunCount < MIN_RUN_COUNT) {
			lQueries = new int[lNodes[i] * 2];
			lTt.clear();
			lLct = LctUtils::pruefer(lNodes[i], nullptr, &lTt);
			for (int k = 0; k < lNodes[i] * 2; k++) {
				lQueries[k] = rand() % lNodes[i] + 1;
			}
			lSumLinkCut += runLct(lLct, lQueries, lNodes[i] * 2);
			lSumTrivial += runTrivial(lTt, lQueries, lNodes[i] * 2);
			delete[] lQueries;
			lRunCount++;
		}
		lLctPerf[i] = lSumLinkCut / (lRunCount * 1.0);
		lTrivialPerf[i] = lSumTrivial / (lRunCount * 1.0);
		lRes[i] = (lSumLinkCut * 1.0 / lSumTrivial);
	}
	printRes(lNodes, lRes, lLctPerf, lTrivialPerf);
}

void testTernary() {
	std::cout << "ternary" << std::endl;
	LinkCutTree<int> lLct;
	TrivialTree lTt;
	std::vector<int> lDepth = { 3, 4, 5, 6, 7, 8, 9, 10 };
	std::vector<double> lRes(lDepth.size());
	std::vector<double> lLctPerf(lDepth.size());
	std::vector<double> lTrivialPerf(lDepth.size());
	int* lQueries;
	int lRunCount;
	long long lSumLinkCut;
	long long lSumTrivial;
	int lNodes;
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine e(seed);
	for (int i = 0; i < lDepth.size(); i++) {
		lSumLinkCut = 0;
		lSumTrivial = 0;
		lRunCount = 0;
		lNodes = (3 * pow(3, lDepth[i]) - 1) / 2;
		lLct = LctUtils::ternary(lDepth[i], nullptr, &lTt);
		std::cout << lDepth[i] << " (" << lNodes << ")" << std::endl;
		auto lStart = Clock::now();
		while (std::chrono::duration_cast<std::chrono::seconds>(Clock::now() - lStart).count() < TIME_LIMIT || lRunCount < MIN_RUN_COUNT) {
			lQueries = new int[2 * lNodes];
			for (int k = 0; k < 2 * lNodes; k++) {
				lQueries[k] = rand() % lNodes + 1;
				//lQueries[k] = k % lNodes + 1;
			}
			//std::shuffle(lQueries, lQueries + lQueryCount, e);
			lSumLinkCut += runLct(lLct, lQueries, 2 * lNodes);
			lSumTrivial += runTrivial(lTt, lQueries, 2 * lNodes);
			delete[] lQueries;
			lRunCount++;
		}
		lLctPerf[i] = lSumLinkCut / (lRunCount * 1.0);
		lTrivialPerf[i] = lSumTrivial / (lRunCount * 1.0);
		lRes[i] = (lSumLinkCut * 1.0 / lSumTrivial);
	}
	printRes(lDepth, lRes, lLctPerf, lTrivialPerf);
}

void testFullBinary() {
	std::cout << "binary" << std::endl;
	LinkCutTree<int> lLct;
	TrivialTree lTt;
	std::vector<int> lDepth = { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	std::vector<double> lRes(lDepth.size());
	std::vector<double> lLctPerf(lDepth.size());
	std::vector<double> lTrivialPerf(lDepth.size());
	int* lQueries;
	int lRunCount;
	int lNodes;
	int lQueryCount;
	for (int i = 0; i < lDepth.size(); i++) {
		long long lSumLinkCut = 0;
		long long lSumTrivial = 0;
		lNodes = pow(2, lDepth[i] + 1) - 1;
		lLct = LctUtils::binary(lDepth[i], nullptr, &lTt);
		lQueryCount = 2 * lNodes;
		lRunCount = 0;
		std::cout << lDepth[i] << " (" << lNodes << ")" << std::endl;
		auto lStart = Clock::now();
		while (std::chrono::duration_cast<std::chrono::seconds>(Clock::now() - lStart).count() < TIME_LIMIT || lRunCount < MIN_RUN_COUNT) {
			lQueries = new int[lQueryCount];
			for (int k = 0; k < lQueryCount; k++) {
				lQueries[k] = rand() % lNodes + 1;
			}
			lSumLinkCut += runLct(lLct, lQueries, lQueryCount);
			lSumTrivial += runTrivial(lTt, lQueries, lQueryCount);
			delete[] lQueries;
			lRunCount++;
		}
		lLctPerf[i] = lSumLinkCut / (lRunCount * 1.0);
		lTrivialPerf[i] = lSumTrivial / (lRunCount * 1.0);
		lRes[i] = (lSumLinkCut * 1.0 / lSumTrivial);
	}
	printRes(lDepth, lRes, lLctPerf, lTrivialPerf);
}

void testUnbalancedBinary() {
	std::cout << "unbalanced binary" << std::endl;
	LinkCutTree<int> lLct;
	TrivialTree lTt;
	std::vector<int> lDepth = { 25, 50, 100, 200, 400, 800, 1600, 3200, 6400, 12800 };
	for (int i = 1; i <= 16; i += 1) {
		lDepth.push_back(i * 1000);
	}
	std::vector<double> lRes(lDepth.size());
	std::vector<double> lLctPerf(lDepth.size());
	std::vector<double> lTrivialPerf(lDepth.size());
	int* lQueries;
	int lRunCount;
	int lNodes;
	long long lSumLinkCut;
	long long lSumTrivial;
	for (int i = 0; i < lDepth.size(); i++) {
		lSumLinkCut = 0;
		lSumTrivial = 0;
		lNodes = 2 * lDepth[i] + 1;
		lLct = LctUtils::unbalancedBinary(lDepth[i], nullptr, &lTt);
		lRunCount = 0;
		std::cout << lDepth[i] << " (" << lNodes << ")" << std::endl;
		auto lStart = Clock::now();
		while (std::chrono::duration_cast<std::chrono::seconds>(Clock::now() - lStart).count() < TIME_LIMIT || lRunCount < MIN_RUN_COUNT) {
			lQueries = new int[2 * lNodes];
			for (int k = 0; k < 2 * lNodes; k++) {
				lQueries[k] = rand() % lNodes + 1;
			}
			lSumLinkCut += runLct(lLct, lQueries, 2 * lNodes);
			lSumTrivial += runTrivial(lTt, lQueries, 2 * lNodes);
			delete[] lQueries;
			lRunCount++;
		}
		lLctPerf[i] = lSumLinkCut / (lRunCount * 1.0);
		lTrivialPerf[i] = lSumTrivial / (lRunCount * 1.0);
		lRes[i] = (lSumLinkCut * 1.0 / lSumTrivial);
	}
	printRes(lDepth, lRes, lLctPerf, lTrivialPerf);
}

void testDegenerate() {
	std::cout << "degenerate" << std::endl;
	LinkCutTree<int> lLct;
	TrivialTree lTt;
	std::vector<int> lNodes = { 50, 100, 200, 400, 800, 1600, 3200, 6400, 12800, 25600 };
	for (int i = 2; i <= 32; i += 2) {
		lNodes.push_back(i * 1000);
	}
	std::vector<double> lRes(lNodes.size());
	std::vector<double> lLctPerf(lNodes.size());
	std::vector<double> lTrivialPerf(lNodes.size());
	int* lQueries;
	int lRunCount;
	for (int i = 0; i < lNodes.size(); i++) {
		long long lSumLinkCut = 0;
		long long lSumTrivial = 0;
		lLct = LctUtils::createLeftDeepTree(lNodes[i], &lTt);
		lRunCount = 0;
		std::cout << lNodes[i] << std::endl;
		auto lStart = Clock::now();
		while (std::chrono::duration_cast<std::chrono::seconds>(Clock::now() - lStart).count() < TIME_LIMIT || lRunCount < MIN_RUN_COUNT) {
			lQueries = new int[lNodes[i] * 2];
			for (int k = 0; k < lNodes[i] * 2; k++) {
				lQueries[k] = rand() % lNodes[i] + 1;
			}
			lSumLinkCut += runLct(lLct, lQueries, lNodes[i] * 2);
			lSumTrivial += runTrivial(lTt, lQueries, lNodes[i] * 2);
			delete[] lQueries;
			lRunCount++;
		}
		lLctPerf[i] = lSumLinkCut / (lRunCount * 1.0);
		lTrivialPerf[i] = lSumTrivial / (lRunCount * 1.0);
		lRes[i] = (lSumLinkCut * 1.0 / lSumTrivial);
	}
	printRes(lNodes, lRes, lLctPerf, lTrivialPerf);
}

void benchmark() {
	testRandomNonBinary();
	testTernary();
	testFullBinary();
	testUnbalancedBinary();
	testDegenerate();
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
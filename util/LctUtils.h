#ifndef LINK_CUT_TREE_UTILS_H
#define LINK_CUT_TREE_UTILS_H

#include <vector>
#include <random>
#include <map>
#include <math.h>
#include <ctime>
#include "cstdint"
#include "LctNode.h"
#include "LinkCutTree.h"
#include "OpTreeNode.h"
#include "TrivialTree.h"
#include "TrivialTreeNode.h"

class LctUtils {
public:
	static uint64_t nCk(int aN, int aK) {
		uint64_t** lArray = new uint64_t * [aN + 1];
		for (int i = 0; i < aN + 1; i++) {
			lArray[i] = new uint64_t[aK + 1];
		}
		for (int i = 0; i < aN + 1; i++) {
			for (int j = 0; j < aK + 1; j++) {
				if (j == 0 || j == aN) {
					lArray[i][j] = 1;
				}
				else if (j > i) {
					lArray[i][j] = 0;
				}
				else {
					lArray[i][j] = lArray[i - 1][j - 1] + lArray[i - 1][j];
				}
			}
		}
		uint64_t res = lArray[aN][aK];
		for (int i = 0; i < aN; ++i) {
			delete[] lArray[i];
		}
		delete[] lArray;
		return res;
	}

	// ballot(0,0,n) = catalan(n)
	// calculate number of paths from (i,j) to (2n,0) for n inner nodes
	static uint64_t ballot(int i, int j, int n) {
		return ((j + 1) * nCk(2 * n - i + 1, (2 * n - i + j) / 2 + 1)) / (2 * n - i + 1);
	}

	static LinkCutTree<int, OpTreeNode> createJoinTree(int aInnerNodes, std::vector<LctNode<int>*>* aNodes = nullptr, uint64_t aSeed = -1) {
		uint64_t lCatalan = ballot(0, 0, aInnerNodes);
		if (aSeed == uint64_t(-1)) {
			aSeed = (((uint64_t)rand() << 32) | rand()) % lCatalan;
		}
		else {
			aSeed -= 1;
		}
		std::cout << aSeed + 1 << " / " << lCatalan << std::endl;
		LinkCutTree<int, OpTreeNode> lLct;
		OpTreeNode<int>* lCurrent = lLct.createTree(1, 1);
		if (aNodes) {
			aNodes->push_back(lCurrent);
		}
		bool lLeft = true;
		int lNoParOpen = 1;
		int lNoParClose = 0;
		uint64_t lPaths = 0; // holds the number of possible paths to (2n,0) from current position offset by (1,1)
		// location in grid is (lNoParOpen + lNoParClose, lNoParOpen - lNoParClose)
		int lNodeCount = 1;
		for (int i = 2; i <= 2 * aInnerNodes; i++) {
			lPaths = ballot(lNoParOpen + lNoParClose + 1, lNoParOpen - lNoParClose + 1, aInnerNodes);
			if (lPaths <= aSeed) {
				aSeed = aSeed - lPaths;
				lNoParClose++;
				if (!lLeft) {
					lCurrent = lCurrent->findParent(); // can not be null
					while (lCurrent->_flags[OpTreeNode<int>::HAS_RIGHT_CHILD]) {
						lCurrent = lCurrent->findParent();
					}
				}
				lLeft = false; // tree after closing par. must be right child
			}
			else {
				lNodeCount++;
				if (aNodes) {
					aNodes->push_back(lLct.createTree(lNodeCount, lNodeCount));
				}
				else {
					lLct.createTree(lNodeCount, lNodeCount);
				}
				if (lLeft) {
					lLct[lNodeCount]->linkLeft(lCurrent);
				}
				else {
					lLct[lNodeCount]->linkRight(lCurrent);
				}
				lLeft = true;
				lCurrent = lLct[lNodeCount];
				lNoParOpen++;
			}
		}
		return lLct;
	}

	static LinkCutTree<int> createRandomLCT(int aNodeCount, std::vector<LctNode<int>*>* aNodes = nullptr, TrivialTree* aTrivialTree = nullptr) {
		LinkCutTree<int> lLct;
		if (aNodes) {
			aNodes->clear();
			aNodes->push_back(lLct.createTree(1, 1));
		}
		else {
			lLct.createTree(1, 1);
		}
		if (aTrivialTree) {
			aTrivialTree->createTree(1);
		}
		for (int i = 1; i < aNodeCount; i++) {
			if (aNodes) {
				aNodes->push_back(lLct.createTree(i + 1, i + 1));
			}
			else {
				lLct.createTree(i + 1, i + 1);
			}
			int r = rand() % i + 1;
			lLct[i + 1]->link(lLct[r]);
			if (aTrivialTree) {
				aTrivialTree->createTree(i + 1);
				(*aTrivialTree)[i + 1]->link((*aTrivialTree)[r]);
			}
		}
		return lLct;
	}

	static LinkCutTree<int> pruefer(int aNodeCount, std::vector<LctNode<int>*>* aNodes = nullptr, TrivialTree* aTrivialTree = nullptr) {
		if (aNodes) {
			aNodes->clear();
		}
		std::vector<int> lCode(aNodeCount - 2);
		for (int i = 0; i < aNodeCount - 2; i++) {
			lCode[i] = (rand() % aNodeCount) + 1;
		}
		std::vector<int> lDegree(aNodeCount, 1);
		LinkCutTree<int> lLCT;
		for (int i = 0; i < aNodeCount - 2; i++) {
			lDegree[lCode[i] - 1]++;
		}
		for (int i = 0; i < aNodeCount; i++) {
			if (aNodes) {
				aNodes->push_back(lLCT.createTree(i + 1, i + 1));
			}
			else {
				lLCT.createTree(i + 1, i + 1);
			}
			if (aTrivialTree) {
				aTrivialTree->createTree(i + 1);
			}
		}
		for (int i = 0; i < aNodeCount - 2; i++) {
			for (int j = 0; j < aNodeCount; j++) {
				if (lDegree[j] == 1 && lDegree[lCode[i] - 1] > 1) {
					lLCT[j + 1]->link(lLCT[lCode[i]]);
					if (aTrivialTree) {
						(*aTrivialTree)[j + 1]->link((*aTrivialTree)[lCode[i]]);
					}
					lDegree[j]--;
					lDegree[lCode[i] - 1]--;
				}
			}
		}
		for (int i = 0; i < aNodeCount; i++) {
			if (lDegree[i] == 1) {
				lLCT[aNodeCount]->link(lLCT[i + 1]);
				break;
			}
		}
		return lLCT;
	}

	static void addTernaryChildren(int aDepth, LctNode<int>* aParent, LinkCutTree<int>& l, TrivialTree* aTrivialTree) {
		if (aDepth == 0) {
			return;
		}
		else {
			int lNoMiddleChild = aParent->getContent() * 3;
			l[lNoMiddleChild - 1]->link(aParent);
			l[lNoMiddleChild]->link(aParent);
			l[lNoMiddleChild + 1]->link(aParent);
			if (aTrivialTree) {
				(*aTrivialTree)[lNoMiddleChild - 1]->link((*aTrivialTree)[aParent->getContent()]);
				(*aTrivialTree)[lNoMiddleChild]->link((*aTrivialTree)[aParent->getContent()]);
				(*aTrivialTree)[lNoMiddleChild + 1]->link((*aTrivialTree)[aParent->getContent()]);
			}
			addTernaryChildren(aDepth - 1, l[lNoMiddleChild - 1], l, aTrivialTree);
			addTernaryChildren(aDepth - 1, l[lNoMiddleChild], l, aTrivialTree);
			addTernaryChildren(aDepth - 1, l[lNoMiddleChild + 1], l, aTrivialTree);
		}
	}

	static LinkCutTree<int> ternary(int aDepth, std::vector<LctNode<int>*>* aNodes = nullptr, TrivialTree* aTrivialTree = nullptr) {
		if (aNodes) {
			aNodes->clear();
		}
		LinkCutTree<int> lLCT;
		int lNodeCount = (3 * pow(3, aDepth) - 1) / 2;
		for (int i = 1; i <= lNodeCount; i++) {
			if (aNodes) {
				aNodes->push_back(lLCT.createTree(i, i));
			}
			else {
				lLCT.createTree(i, i);
			}
			if (aTrivialTree) {
				aTrivialTree->createTree(i);
			}
		}
		addTernaryChildren(aDepth, lLCT[1], lLCT, aTrivialTree);
		return lLCT;
	}

	static void addBinaryChildren(int aDepth, LctNode<int>* aParent, LinkCutTree<int>& l, TrivialTree* aTrivialTree) {
		if (aDepth == 0) {
			return;
		}
		else {
			int lLeftChild = aParent->getContent() * 2;
			l[lLeftChild]->link(aParent);
			l[lLeftChild + 1]->link(aParent);
			if (aTrivialTree) {
				(*aTrivialTree)[lLeftChild]->link((*aTrivialTree)[aParent->getContent()]);
				(*aTrivialTree)[lLeftChild + 1]->link((*aTrivialTree)[aParent->getContent()]);
			}
			addBinaryChildren(aDepth - 1, l[lLeftChild], l, aTrivialTree);
			addBinaryChildren(aDepth - 1, l[lLeftChild + 1], l, aTrivialTree);
		}
	}

	static LinkCutTree<int> binary(int aDepth, std::vector<LctNode<int>*>* aNodes = nullptr, TrivialTree* aTrivialTree = nullptr) {
		if (aNodes) {
			aNodes->clear();
		}
		LinkCutTree<int> lLCT;
		int lNodeCount = pow(2, aDepth + 1) - 1;
		for (int i = 1; i <= lNodeCount; i++) {
			if (aNodes) {
				aNodes->push_back(lLCT.createTree(i, i));
			}
			else {
				lLCT.createTree(i, i);
			}
			if (aTrivialTree) {
				aTrivialTree->createTree(i);
			}
		}
		addBinaryChildren(aDepth, lLCT[1], lLCT, aTrivialTree);
		return lLCT;
	}

	static LinkCutTree<int> unbalancedBinary(int aDepth, std::vector<LctNode<int>*>* aNodes = nullptr, TrivialTree* aTrivialTree = nullptr) {
		if (aNodes) {
			aNodes->clear();
		}
		LinkCutTree<int> lLCT;
		int lNodeCount = 2 * aDepth + 1;
		for (int i = 1; i <= lNodeCount; i++) {
			if (aNodes) {
				aNodes->push_back(lLCT.createTree(i, i));
			}
			else {
				lLCT.createTree(i, i);
			}
			if (aTrivialTree) {
				aTrivialTree->createTree(i);
			}
		}
		for (int i = 2; i <= lNodeCount; i++) {
			if (i % 2 == 0) {
				lLCT[i]->link(lLCT[i - 1]);
				if (aTrivialTree) {
					(*aTrivialTree)[i]->link((*aTrivialTree)[i - 1]);
				}
			}
			else {
				lLCT[i]->link(lLCT[i - 2]);
				if (aTrivialTree) {
					(*aTrivialTree)[i]->link((*aTrivialTree)[i - 2]);
				}
			}
		}
		return lLCT;
	}

	static LinkCutTree<int> createLeftDeepTree(int aNodeCount, TrivialTree* aTrivialTree) {
		LinkCutTree<int> lLct;
		lLct.createTree(1, 1);
		aTrivialTree->createTree(1);
		for (int i = 1; i < aNodeCount; i++) {
			lLct.createTree(i + 1, i + 1);
			lLct[i + 1]->link(lLct[i]);
			aTrivialTree->createTree(i + 1);
			(*aTrivialTree)[i + 1]->link((*aTrivialTree)[i]);
		}
		return lLct;
	}

	static void updateBackpointers(std::vector<LctNode<int>*>& aNodes,
		std::map<LctNode<int>*, std::vector<LctNode<int>*>>& aBackpointers)
	{
		aBackpointers.clear();
		for (size_t i = 0; i < aNodes.size(); i++) {
			if (aNodes[i]->isRoot() && aNodes[i]->_parent) {
				if (aBackpointers.count(aNodes[i]->_parent) == 0) {
					aBackpointers[aNodes[i]->_parent] = std::vector<LctNode<int>*>();
				}
				aBackpointers[aNodes[i]->_parent].push_back(aNodes[i]);
			}
		}
	}

	static void deleteNodes(std::vector<LctNode<int>*>* v) {
		for (size_t i = 0; i < v->size(); i++) {
			delete v->at(i);
		}
	}

	template<typename T> static void printSplayTreeRecursive(const std::string& aPrefix, LctNode<T>* aNode,
		bool aLeft, std::map<LctNode<T>*, std::vector<LctNode<T>*>>* aBackpointers)
	{
		std::cout << aPrefix << (aLeft ? "|--" : "|--") << aNode->getID() << (aNode->isRoot() ? "r" : "");
		if (aNode->_parent) {
			std::cout << "(" << aNode->_parent->getID() << ")";
		}
		std::cout << std::endl;

		if (aNode->_right) {
			printSplayTreeRecursive(aPrefix + (aLeft ? "|    " : "     "), aNode->_right, false, aBackpointers);
		}
		if (aBackpointers && aBackpointers->count(aNode)) {
			for (size_t i = 0; i < aBackpointers->at(aNode).size(); i++) {
				printSplayTreeRecursive(aPrefix + "~    ", aBackpointers->at(aNode).at(i), true, aBackpointers);
			}
		}
		if (aNode->_left) {
			printSplayTreeRecursive(aPrefix + (aLeft ? "|    " : "     "), aNode->_left, true, aBackpointers);
		}
	}

	template<typename T> static void printSplayTree(LctNode<T>* aNode,
		std::map<LctNode<T>*, std::vector<LctNode<T>*>>* aBackpointers)
	{
		while (!aNode->isRoot()) {
			aNode = aNode->_parent;
		}
		printSplayTreeRecursive("", aNode, false, aBackpointers);
	}

	template<typename T> static void printLCT(LctNode<T>* aNode,
		std::map<LctNode<T>*, std::vector<LctNode<T>*>>* aBackpointers)
	{
		while (aNode->_parent) {
			aNode = aNode->_parent;
		}
		printSplayTreeRecursive("", aNode, false, aBackpointers);
	}

	template<typename T> static int printReprTree(LctNode<T>* aNode,
		std::map<LctNode<T>*, std::vector<LctNode<T>*>>* aBackpointers, bool aDiscoverParent = true, int aDepth = 0)
	{
		while (aDiscoverParent && aNode->_parent) {
			aNode = aNode->_parent;
		}
		if (aNode->_left) {
			aDepth = printReprTree(aNode->_left, aBackpointers, false, aDepth);
		}
		std::cout << std::string(aDepth * 4, ' ') << aNode->getID();
		OpTreeNode<T>* p;
		p = dynamic_cast<OpTreeNode<T>*>(aNode);
		if (p != nullptr) {
			if (p->_flags[OpTreeNode<T>::IS_LEFT_CHILD]) {
				std::cout << " (l)";
			}
			else if (p->_flags[OpTreeNode<T>::IS_RIGHT_CHILD]) {
				std::cout << " (r)";
			}
			else if (p->_flags[OpTreeNode<T>::IS_ONLY_CHILD]) {
				std::cout << " (o)";
			}
		}
		std::cout << std::endl;
		aDepth++;
		if (aBackpointers && aBackpointers->count(aNode)) {
			for (size_t i = 0; i < aBackpointers->at(aNode).size(); i++) {
				printReprTree(aBackpointers->at(aNode).at(i), aBackpointers, false, aDepth);
			}
		}
		if (aNode->_right) {
			aDepth = printReprTree(aNode->_right, aBackpointers, false, aDepth);
		}
		return aDepth;
	}
};

#endif

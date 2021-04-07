#ifndef LINK_CUT_TREE_UTILS_H
#define LINK_CUT_TREE_UTILS_H

#include <vector>
#include <random>
#include <map>
#include <ctime>
#include "cstdint"
#include "LctNode.h"
#include "LinkCutTree.h"
#include "OpTreeNode.h"

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

	static LinkCutTree<int, OpTreeNode> createRandomJoinTree(int aInnerNodes, std::vector<LctNode<int>*>* aNodes = nullptr, uint64_t aSeed = -1) {
		uint64_t lCatalan = ballot(0, 0, aInnerNodes);
		if (aSeed == -1) {
			aSeed = (((uint64_t)rand() << 32) | rand()) % lCatalan;
		}
		std::cout << aSeed << " / " << lCatalan << std::endl;
		LinkCutTree<int, OpTreeNode> lLCT;
		OpTreeNode<int>* lCurrent = lLCT.createTree(1, 1);
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
					while (lCurrent->getFlag(OpTreeNode<int>::HAS_RIGHT_CHILD)) {
						lCurrent = lCurrent->findParent();
					}
				}
				lLeft = false; // tree after closing par. must be right child
			}
			else {
				lNodeCount++;
				if (aNodes) {
					aNodes->push_back(lLCT.createTree(lNodeCount, lNodeCount));
				}
				else {
					lLCT.createTree(lNodeCount, lNodeCount);
				}
				if (lLeft) {
					lLCT[lNodeCount]->linkLeft(lCurrent);
				}
				else {
					lLCT[lNodeCount]->linkRight(lCurrent);
				}
				lLeft = true;
				lCurrent = lLCT[lNodeCount];
				lNoParOpen++;
			}
		}
		return lLCT;
	}

	static LinkCutTree<int> createRandomLCT(int aNodeCount, std::vector<LctNode<int>*>* aNodes = nullptr) {
		srand(time(nullptr));
		aNodes->clear();
		LinkCutTree<int> aLCT;
		aLCT.createTree(0, 0);
		for (int i = 1; i < aNodeCount; i++) {
			if (aNodes) {
				aNodes->push_back(aLCT.createTree(i, i));
			}
			else {
				aLCT.createTree(i, i);
			}
			aLCT[i]->link(aLCT[rand() % i]);
		}
		return aLCT;
	}

	static void updateBackpointers(std::vector<LctNode<int>*>& aNodes,
		std::map<LctNode<int>*, std::vector<LctNode<int>*>>& aBackpointers)
	{
		aBackpointers.clear();
		for (int i = 0; i < aNodes.size(); i++) {
			if (aNodes[i]->_isRoot && aNodes[i]->_parent) {
				if (aBackpointers.count(aNodes[i]->_parent) == 0) {
					aBackpointers[aNodes[i]->_parent] = std::vector<LctNode<int>*>();
				}
				aBackpointers[aNodes[i]->_parent].push_back(aNodes[i]);
			}
		}
	}

	static void deleteNodes(std::vector<LctNode<int>*>* v) {
		for (int i = 0; i < v->size(); i++) {
			delete v->at(i);
		}
	}

	template<typename T> static void printSplayTreeRecursive(const std::string& aPrefix, LctNode<T>* aNode,
		bool aLeft, std::map<LctNode<T>*, std::vector<LctNode<T>*>>* aBackpointers)
	{
		std::cout << aPrefix << (aLeft ? "|--" : "|--") << aNode->getID() << (aNode->_isRoot ? "r" : "");
		if (aNode->_parent) {
			std::cout << "(" << aNode->_parent->getID() << ")";
		}
		std::cout << std::endl;

		if (aNode->_right) {
			printSplayTreeRecursive(aPrefix + (aLeft ? "|    " : "     "), aNode->_right, false, aBackpointers);
		}
		if (aBackpointers && aBackpointers->count(aNode)) {
			for (int i = 0; i < aBackpointers->at(aNode).size(); i++) {
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
		while (!aNode->_isRoot) {
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
			if (p->getFlag(OpTreeNode<T>::IS_LEFT_CHILD)) {
				std::cout << " (l)";
			}
			else if (p->getFlag(OpTreeNode<T>::IS_RIGHT_CHILD)) {
				std::cout << " (r)";
			}
			else if (p->getFlag(OpTreeNode<T>::IS_ONLY_CHILD)) {
				std::cout << " (o)";
			}
		}
		std::cout << std::endl;
		aDepth++;
		if (aBackpointers && aBackpointers->count(aNode)) {
			for (int i = 0; i < aBackpointers->at(aNode).size(); i++) {
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

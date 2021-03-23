#ifndef LINK_CUT_TREE_UTILS_H
#define LINK_CUT_TREE_UTILS_H
#define ull unsigned long long

#include <vector>
#include <random>
#include <map>
#include "Node.h"
#include "LinkCutTree.h"

ull nCk(int aN, int aK) {
	ull** lArray = new ull* [aN + 1];
	for (int i = 0; i < aN + 1; i++) {
		lArray[i] = new ull[aK + 1];
	}
	for (int i = 0; i < aN + 1; i++) {
		for (int j = 0; j < aK + 1; j++) {
			if (j == 0 || j == aN) {
				lArray[i][j] = 1;
			} else if(j > i) {
				lArray[i][j] = 0;
			}
			else {
				lArray[i][j] = lArray[i - 1][j - 1] + lArray[i - 1][j];
			}
		}
	}
	ull res = lArray[aN][aK];
	for (int i = 0; i < aN; ++i) {
		delete[] lArray[i];
	}
	delete[] lArray;
	return res;
}

// ballot(0,0,n) = catalan(n)
// calculate number of paths from (i,j) to (2n,0) for n inner nodes
ull ballot(int i, int j, int n) {
	return ((j + 1) * nCk(2 * n - i + 1, (2 * n - i + j) / 2 + 1)) / (2 * n - i + 1);
}

LinkCutTree<int> createRandomJoinTree(int aInnerNodes, int aSeed = -1, std::vector<Node<int>*>* aNodes = nullptr) {
	if (aSeed == -1) {
		int lCatalan = ballot(0, 0, aInnerNodes);
		srand(time(nullptr));
		aSeed = rand() % lCatalan;
	}
	LinkCutTree<int> lLCT;
	Node<int>* lCurrent = lLCT.createTree(1, 1);
	bool lLeft = true;
	int lNoParOpen = 1;
	int lNoParClose = 0;
	ull lPaths = 0; // holds the number of possible paths to (2n,0) from current position offset by (1,1)
	// location in grid is (lNoParOpen + lNoParClose, lNoParOpen - lNoParClose)
	for (int i = 2; i <= 2 * aInnerNodes; i++) {
		lPaths = ballot(lNoParOpen + lNoParClose + 1, lNoParOpen - lNoParClose + 1, aInnerNodes);
		if (lPaths <= aSeed) {
			aSeed = aSeed - lPaths;
			lNoParClose++;
			if (!lLeft) {
				lCurrent = lCurrent->findParent(); // can not be null
				while (lCurrent->getFlag(Node<int>::HAS_RIGHT_CHILD)) {
					lCurrent = lCurrent->findParent();
				}
			}
			lLeft = false; // tree after closing par. must be right child
		}
		else {
			if (aNodes) {
				aNodes->push_back(lLCT.createTree(i, i));
			}
			else {
				lLCT.createTree(i, i);
			}
			if (lLeft) {
				lLCT[i]->linkLeft(lCurrent);
			}
			else {
				lLCT[i]->linkRight(lCurrent);
			}
			lLeft = true;
			lCurrent = lLCT[i];
			lNoParOpen++;
		}
	}
	return lLCT;
}

std::vector<Node<int>*> createRandomSplayTree(int a, int b, int offset) {
	std::vector<Node<int>*> v;
	int n = a + rand() % (b - a + 1);

	for (int i = 0; i < n; i++) {
		v.push_back(new Node<int>(i + offset));
		v[i]->setRoot(false);
	}

	std::shuffle(std::begin(v), std::end(v), std::default_random_engine(std::random_device()()));
	v.at(0)->setRoot(true);
	for (int i = 1; i < v.size(); i++) {
		Node<int>* p = v[0];
		while (true) {
			if (v[i]->getKey() > p->getKey()) {
				if (!p->right()) {
					p->setRight(v[i]);
					v[i]->setParent(p);
					break;
				}
				else {
					p = p->right();
				}
			}
			else {
				if (!p->left()) {
					p->setLeft(v[i]);
					v[i]->setParent(p);
					break;
				}
				else {
					p = p->left();
				}
			}
		}
	}
	return v;
}

LinkCutTree<int> createRandomLCT(int aNodeCount, std::vector<Node<int>*>* aNodes = nullptr) {
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
	//TODO random links and cuts
	return aLCT;
}

void updateBackpointers(std::vector<Node<int>*>& aNodes,
	std::map<Node<int>*, std::vector<Node<int>*>>& aBackpointers)
{
	aBackpointers.clear();
	for (int i = 0; i < aNodes.size(); i++) {
		if (aNodes[i]->isRoot() && aNodes[i]->parent()) {
			if (aBackpointers.count(aNodes[i]->parent()) == 0) {
				aBackpointers[aNodes[i]->parent()] = std::vector<Node<int>*>();
			}
			aBackpointers[aNodes[i]->parent()].push_back(aNodes[i]);
		}
	}
}

void deleteNodes(std::vector<Node<int>*>* v) {
	for (int i = 0; i < v->size(); i++) {
		delete v->at(i);
	}
}

template<typename T> void printSplayTreeRecursive(const std::string& aPrefix, Node<T>* aNode,
	bool aLeft, std::map<Node<T>*, std::vector<Node<T>*>>* aBackpointers)
{
	std::cout << aPrefix << (aLeft ? "|--" : "|--") << aNode->getID() << (aNode->isRoot() ? "r" : "");
	if (aNode->parent()) {
		std::cout << "(" << aNode->parent()->getID() << ")";
	}
	std::cout << std::endl;

	if (aNode->right()) {
		printSplayTreeRecursive(aPrefix + (aLeft ? "|    " : "     "), aNode->right(), false, aBackpointers);
	}
	if (aBackpointers && aBackpointers->count(aNode)) {
		for (int i = 0; i < aBackpointers->at(aNode).size(); i++) {
			printSplayTreeRecursive(aPrefix + "~    ", aBackpointers->at(aNode).at(i), true, aBackpointers);
		}
	}
	if (aNode->left()) {
		printSplayTreeRecursive(aPrefix + (aLeft ? "|    " : "     "), aNode->left(), true, aBackpointers);
	}
}

template<typename T> void printSplayTree(Node<T>* aNode,
	std::map<Node<T>*, std::vector<Node<T>*>>* aBackpointers)
{
	while (!aNode->_isRoot) {
		aNode = aNode->parent();
	}
	printSplayTreeRecursive("", aNode, false, aBackpointers);
}

template<typename T> void printLCT(Node<T>* aNode,
	std::map<Node<T>*, std::vector<Node<T>*>>* aBackpointers)
{
	while (aNode->parent()) {
		aNode = aNode->parent();
	}
	printSplayTreeRecursive("", aNode, false, aBackpointers);
}

template<typename T> int printReprTree(Node<T>* aNode,
	std::map<Node<T>*, std::vector<Node<T>*>>* aBackpointers, bool aDiscoverParent = true, int aDepth = 0)
{
	while (aDiscoverParent && aNode->parent()) {
		aNode = aNode->parent();
	}
	if (aNode->left()) {
		aDepth = printReprTree(aNode->left(), aBackpointers, false, aDepth);
	}
	std::cout << std::string(aDepth * 4L, ' ') << aNode->getID() << std::endl;
	aDepth++;
	if (aBackpointers && aBackpointers->count(aNode)) {
		for (int i = 0; i < aBackpointers->at(aNode).size(); i++) {
			printReprTree(aBackpointers->at(aNode).at(i), aBackpointers, false, aDepth);
		}
	}
	if (aNode->right()) {
		aDepth = printReprTree(aNode->right(), aBackpointers, false, aDepth);
	}
	return aDepth;
}

#endif

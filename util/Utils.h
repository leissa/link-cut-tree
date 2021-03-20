#ifndef LINK_CUT_TREE_UTILS_H
#define LINK_CUT_TREE_UTILS_H
#define ull unsigned long long

#include <vector>
#include <random>
#include <map>
#include "Node.h"
#include "LinkCutTree.h"

// fails at aN >= 21 for 64 bit ull, maybe use something equivalent to biginteger
// or calculate nCk dynamically
ull factorial(int aN) {
	ull lRes = 1;
	for (int i = 2; i <= aN; i++) {
		lRes *= i;
	}
	return lRes;
}

ull nCk(int aN, int aK) {
	return factorial(aN) / (factorial(aK) * factorial(aN - aK));
}

// ballot(0,0,n) = catalan(n)
// calculate number of paths from (i,j) to (2n,0) for n inner nodes
ull ballot(int i, int j, int n) {
	return ((j + 1) * nCk(2 * n - i + 1, 0.5 * (2 * n - i + j) + 1)) / (2 * n - i + 1);
}

LinkCutTree<int> createRandomJoinTree(int aInnerNodes, int aSeed = -1, std::vector<Node<int>*>* aNodes = nullptr) {
	if (aSeed == -1) {
		int lCatalan = ballot(0, 0, aInnerNodes);
		srand(time(nullptr));
		aSeed = rand() % lCatalan;
	}
	LinkCutTree<int> lLCT;
	Node<int>* current = lLCT.createTree(1, 1);
	Node<int>* temp = nullptr;
	bool left = true;
	int lNoParOpen = 1;
	int lNoParClose = 0;
	int lPaths = 0; // holds the number of possible paths to (2n,0) from current position offset by (1,1)
	// location in grid is (lNoParOpen + lNoParClose, lNoParOpen - lNoParClose)
	for (int i = 2; i <= 2 * aInnerNodes; i++) {
		lPaths = ballot(lNoParOpen + lNoParClose + 1, lNoParOpen - lNoParClose + 1, aInnerNodes);
		if (lPaths <= aSeed) {
			aSeed = aSeed - lPaths;
			lNoParClose++;
			if (!left) {
				current = current->findParent(); // can not be null
				while (current->getFlag(Node<int>::HAS_RIGHT_CHILD)) {
					current = current->findParent();
				}
			}
			left = false; // tree after closing par. must be right child
		}
		else {
			if (aNodes) {
				aNodes->push_back(lLCT.createTree(i, i));
			}
			else {
				lLCT.createTree(i, i);
			}
			if (left) {
				lLCT[i]->linkLeft(current);
			}
			else {
				lLCT[i]->linkRight(current);
			}
			left = true;
			current = lLCT[i];
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

#endif

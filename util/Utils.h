#ifndef LINK_CUT_TREE_UTILS_H
#define LINK_CUT_TREE_UTILS_H

#include <vector>
#include <random>
#include <map>
#include "Node.h"
#include "LinkCutTree.h"

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
			aLCT.createTree(i);
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

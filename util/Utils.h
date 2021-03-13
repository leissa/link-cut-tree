#ifndef LINK_CUT_TREE_UTILS_H
#define LINK_CUT_TREE_UTILS_H

#include <Node.h>
#include <vector>
#include <random>
#include <map>

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

std::vector<Node<int>*> createRandomLCT(int n, int a, int b) {
	std::vector<Node<int>*> v = createRandomSplayTree(a, b, 0);
	for (int i = 1; i < n; i++) {
		std::vector<Node<int>*> temp = createRandomSplayTree(a, b, i * 10);
		temp[0]->setParent(v[rand() % v.size()]);
		v.insert(v.end(), temp.begin(), temp.end());
	}
	return v;
}

void updateBackpointers(std::vector<Node<int>*>& v, std::map<Node<int>*, std::vector<Node<int>*>>& b) {
	b.clear();
	for (int i = 0; i < v.size(); i++) {
		if (v[i]->isRoot() && v[i]->parent()) {
			if (b.count(v[i]->parent()) == 0) {
				b[v[i]->parent()] = std::vector<Node<int>*>();
			}
			b[v[i]->parent()].push_back(v[i]);
		}
	}
}

void deleteNodes(std::vector<Node<int>*>* v) {
	for (int i = 0; i < v->size(); i++) {
		delete v->at(i);
	}
}

#endif

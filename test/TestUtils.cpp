#include <LinkCutTree.h>
#include <vector>
#include <algorithm>
#include <random>

using Node = LinkCutTree<int>::Node;

std::vector<Node*> createRandomSplayTree(int a, int b) {
	std::vector<Node*> v;
	int n = a + rand() % (b - a + 1);

	for (int i = 0; i < n; i++) {
		v.push_back(LinkCutTree<int>::createTree(i));
		v[i]->isRoot = false;
	}

	std::shuffle(std::begin(v), std::end(v), std::default_random_engine(std::random_device()()));
	v.at(0)->isRoot = true;
	for (int i = 1; i < v.size(); i++) {
		Node* p = v[0];
		while (true) {
			if (v[i]->key > p->key) {
				if (!p->right) {
					p->right = v[i];
					v[i]->parent = p;
					break;
				}
				else {
					p = p->right;
				}
			}
			else {
				if (!p->left) {
					p->left = v[i];
					v[i]->parent = p;
					break;
				}
				else {
					p = p->left;
				}
			}
		}
	}
	return v;
}

void deleteNodes(std::vector<Node*>* v) {
	for (int i = 0; i < v->size(); i++) {
		delete v->at(i);
	}
}


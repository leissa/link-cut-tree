#include <LinkCutTree.h>
#include <vector>
#include <random>
#include <map>

using Node = LinkCutTree<int>::Node;

std::vector<Node*> createRandomSplayTree(int a, int b, int offset) {
	std::vector<Node*> v;
	int n = a + rand() % (b - a + 1);

	for (int i = 0; i < n; i++) {
		v.push_back(LinkCutTree<int>::createTree(i + offset));
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

std::vector<Node*> createRandomLCT(int n, int a, int b) {
	std::vector<Node*> v = createRandomSplayTree(a, b, 0);
	for (int i = 1; i < n; i++) {
		std::vector<Node*> temp = createRandomSplayTree(a, b, i * 10);
		temp[0]->parent = v[rand() % v.size()];
		v.insert(v.end(), temp.begin(), temp.end());
	}
	return v;
}

void updateBackpointers(std::vector<Node*>& v, std::map<Node*, std::vector<Node*>>& b) {
	b.clear();
	for (int i = 0; i < v.size(); i++) {
		if (v[i]->isRoot && v[i]->parent) {
			if (b.count(v[i]->parent) == 0) {
				b[v[i]->parent] = std::vector<Node*>();
			}
			b[v[i]->parent].push_back(v[i]);
		}
	}
}

void deleteNodes(std::vector<Node*>* v) {
	for (int i = 0; i < v->size(); i++) {
		delete v->at(i);
	}
}


#ifndef LINK_CUT_TREE_NODE_H
#define LINK_CUT_TREE_NODE_H

#include <iostream>
#include <map>
#include <vector>

template<typename T> class Node {
public:
	Node* left, * right, * parent;
	T key;
	// if node is the root of the splay tree then treat the parent pointer as the path-parent pointer
	bool isRoot;
	Node() = default;
	Node(const T& key) :left(nullptr), right(nullptr), parent(nullptr), key(key), isRoot(true) {};

	static void rotR(Node* v) {
		v->left->parent = v->parent;
		if (v->parent) {
			if (v->parent->left == v) {
				v->parent->left = v->left;
			}
			if (v->parent->right == v) {
				v->parent->right = v->left;
			}
		}
		v->parent = v->left;
		if (v->left->right) {
			v->left->right->parent = v;
			v->left = v->left->right;
		}
		else {
			v->left = nullptr;
		}
		v->parent->right = v;
		if (v->isRoot) {
			v->isRoot = false;
			v->parent->isRoot = true;
		}
	}

	static void rotL(Node* v) {
		v->right->parent = v->parent;
		if (v->parent) {
			if (v->parent->right == v) {
				v->parent->right = v->right;
			}
			if (v->parent->left == v) {
				v->parent->left = v->right;
			}
		}
		v->parent = v->right;
		if (v->right->left) {
			v->right->left->parent = v;
			v->right = v->right->left;
		}
		else {
			v->right = nullptr;
		}
		v->parent->left = v;
		if (v->isRoot) {
			v->isRoot = false;
			v->parent->isRoot = true;
		}
	}

	static void splay(Node* v) {
		while (!v->isRoot) {
			// zig
			if (v->parent->isRoot) {
				if (v->parent->left == v) {
					rotR(v->parent);
				}
				else {
					rotL(v->parent);
				}
			}
			// lefthanded zigzig 
			else if (v->parent->left == v && v->parent->parent->left == v->parent) {
				rotR(v->parent->parent);
				rotR(v->parent);
			}
			// righthanded zigzig
			else if (v->parent->right == v && v->parent->parent->right == v->parent) {
				rotL(v->parent->parent);
				rotL(v->parent);
			}
			// lefthanded zigzag
			else if (v->parent->right == v && v->parent->parent->left == v->parent) {
				rotL(v->parent);
				rotR(v->parent);
			}
			// righthanded zigzag
			else {
				rotR(v->parent);
				rotL(v->parent);
			}
		}
	}

public:
	static void access(Node* v) {
		splay(v);
		if (v->right) {
			v->right->isRoot = true; // change v->right's parent pointer to a path-parent pointer
			v->right = nullptr;
		}
		while (v->parent) {
			splay(v->parent);
			if (v->parent->right) {
				v->parent->right->isRoot = true;
			}
			v->parent->right = v;
			v->isRoot = false;
			splay(v); // single rotation around v->parent giving v the next path-parent pointer
		}
	}

	static Node* findRoot(Node* v) {
		access(v);
		while (v->left) {
			v = v->left;
		}
		splay(v);
		return v;
	}

	static bool link(Node* v, Node* w) {
		access(v);
		if (v->left) {
			return false; // v is not the root of its represented tree
		}
		access(w);
		v->left = w;
		w->parent = v;
		w->isRoot = false;
		return true;
	}

	static void cut(Node* v) {
		access(v);
		if (v->left) { // if v is root of the represented tree it has no left child after access and cut does nothing
			v->left->isRoot = true;
			v->left->parent = nullptr; // v->left is on preferred path
			v->left = nullptr;
		}
	}
};

#endif

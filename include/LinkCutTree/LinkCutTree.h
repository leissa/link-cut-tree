#ifndef LINK_CUT_TREE_H
#define LINK_CUT_TREE_H

#include <iostream>

template<typename T> class LinkCutTree {
public:
	struct Node {
		Node* left, * right, * parent;
		T key;
		// if node is the root of the splay tree then treat the parent pointer as the path-parent pointer
		bool isRoot;
		Node(const T& key) :left(nullptr), right(nullptr), parent(nullptr), key(key), isRoot(true) {};
	};

	// maybe map key to pointers and do not expose node pointers, or do both
	Node* createTree(const T& key) {
		return new Node(key);
	}

	Node* findRoot(Node* v);

	void link(Node* v, Node* w);

	void cut(Node* v);

private:
	void rotR(Node* v) {
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

	void rotL(Node* v) {
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

	void splay(Node* v) {
		Node* temp = v;
		while (!temp->isRoot) {
			temp = temp->parent;
		}
		// pathParent pointer is null only if root is the root of the represented tree
		Node* pathParent = temp->parent;
		while (v->parent != pathParent) { // v is not root yet
			// zig
			if (v->parent->parent == pathParent) {
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

	void access(Node* v);

	void printSplayTree(const std::string& prefix, const Node* node, bool isLeft)
	{
		std::cout << prefix << "|--" << node->key << (node->isRoot ? "r" : "");
		if (node->parent) {
			std::cout << "(" << node->parent->key << ")";
		}
		std::cout << std::endl;

		if (node->left) {
			printSplayTree(prefix + (isLeft ? "|    " : "     "), node->left, true);
		}
		if (node->right) {
			printSplayTree(prefix + (isLeft ? "|    " : "     "), node->right, false);
		}
	}

	void printSplayTree(const Node* node) {
		while (!node->isRoot) {
			node = node->parent;
		}
		printSplayTree("", node, false);
	}
};

#endif

#ifndef LINK_CUT_TREE_H
#define LINK_CUT_TREE_H

#include <iostream>
#include <map>
#include <vector>

template<typename T> class LinkCutTree {
public:
	struct Node {
		Node* left, * right, * parent;
		T key;
		// if node is the root of the splay tree then treat the parent pointer as the path-parent pointer
		bool isRoot;
		bool operator<(const Node& o)  const { return this > &o; };
		Node(const T& key) :left(nullptr), right(nullptr), parent(nullptr), key(key), isRoot(true) {};
	};

	// maybe map key to pointers and do not expose node pointers, or do both
	static Node* createTree(const T& key) {
		return new Node(key);
	}

protected:
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

	static void access(Node* v) {
		splay(v);
		if (v->right) {
			v->right->isRoot = true; // change v->right's parent pointer to a path-parent pointer
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

public:
	static Node* findRoot(Node* v) {
		access(v);
		while (v->left) {
			v = v->left;
		}
		splay(v);
		return v;
	}

	static void link(Node* v, Node* w) {
		while (v->parent) {
			v = v->parent;
		}
		access(v); // check why not splay(v)
		access(w);
		v->left = w;
		w->parent = v;
		w->isRoot = false;
	}

	static void cut(Node* v) {
		access(v);
		if (v->left) { // if v is root of the represented tree it has no left child after access and cut does nothing
			v->left->isRoot = true;
			v->left->parent = nullptr; // v->left is on preferred path
			v->left = nullptr;
		}
	}

	static void printSplayTree(const std::string& prefix, Node* node,
		bool isLeft, std::map<Node*, std::vector<Node*>>* b = nullptr)
	{
		std::cout << prefix << (isLeft ? "|->" : "|-<") << node->key << (node->isRoot ? "r" : "");
		if (node->parent) {
			std::cout << "(" << node->parent->key << ")";
		}
		std::cout << std::endl;

		if (node->right) {
			printSplayTree(prefix + (isLeft ? "|    " : "     "), node->right, false, b);
		}

		if (b) {
			if (b->count(node)) {
				for (int i = 0; i < b->at(node).size(); i++) {
					printSplayTree(prefix + "~    ", b->at(node).at(i), true, b);
				}
			}
		}
		if (node->left) {
			printSplayTree(prefix + (isLeft ? "|    " : "     "), node->left, true, b);
		}
	}

	static void printSplayTree(Node* node, std::map<Node*, std::vector<Node*>>* b = nullptr) {
		while (!node->isRoot) {
			node = node->parent;
		}
		printSplayTree("", node, false, b);
	}
};

#endif

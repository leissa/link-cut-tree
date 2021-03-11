#ifndef LINK_CUT_TREE_NODE_H
#define LINK_CUT_TREE_NODE_H

#include <map>
#include <vector>

template<typename T> class Node {
public:
	Node* left, * right, * parent;
	T key;
	bool isRoot; // refers to aux tree
	Node() = default;
	Node(const T& key);

	void expose();
	bool link(Node* other);
	void cut();
	Node* findRoot();
	Node* lowestCommonAncestor(Node* other);

private:
	void rotR();
	void rotL();
	void splay();
};

template<typename T> Node<T>::Node(const T& key)
	: left(nullptr), right(nullptr), parent(nullptr), key(key), isRoot(true) {
}

template<typename T> void Node<T>::expose() {
	splay();
	if (right) {
		right->isRoot = true; // change right's parent pointer to a path-parent pointer
		right = nullptr;
	}
	while (parent) {
		parent->splay();
		if (parent->right) {
			parent->right->isRoot = true;
		}
		parent->right = this;
		isRoot = false;
		splay(); // single rotation around parent giving v the next path-parent pointer
	}
}

template<typename T> Node<T>* Node<T>::findRoot() {
	expose();
	Node* v = this;
	while (v->left) {
		v = v->left;
	}
	v->splay();
	return v;
}

template<typename T> Node<T>* Node<T>::lowestCommonAncestor(Node* other) {
	Node* root = this->findRoot();
	Node* otherRoot = other->findRoot();
	if (root != otherRoot) {
		return nullptr;
	}
	else if (root == this || other == this) {
		return this;
	}
	else if (root == other) {
		return other;
	}
	else {
		this->expose();
		other->expose();
		Node* lca = this;
		while (!lca->isRoot) {
			lca = lca->parent;
		}
		return lca->parent;
	}
}

template<typename T> bool Node<T>::link(Node<T>* other) {
	expose();
	if (left) {
		return false; // v is not the root of its represented tree
	}
	other->expose();
	left = other;
	other->parent = this;
	other->isRoot = false;
	return true;
}

template<typename T> void Node<T>::cut() {
	expose();
	if (left) { // if v is root of the represented tree it has no left child after expose and cut does nothing
		left->isRoot = true;
		left->parent = nullptr; // left is on preferred path
		left = nullptr;
	}
}

template<typename T> void Node<T>::rotR() {
	left->parent = parent;
	if (parent) {
		if (parent->left == this) {
			parent->left = left;
		}
		if (parent->right == this) {
			parent->right = left;
		}
	}
	parent = left;
	if (left->right) {
		left->right->parent = this;
		left = left->right;
	}
	else {
		left = nullptr;
	}
	parent->right = this;
	if (isRoot) {
		isRoot = false;
		parent->isRoot = true;
	}
}

template<typename T> void Node<T>::rotL() {
	right->parent = parent;
	if (parent) {
		if (parent->right == this) {
			parent->right = right;
		}
		if (parent->left == this) {
			parent->left = right;
		}
	}
	parent = right;
	if (right->left) {
		right->left->parent = this;
		right = right->left;
	}
	else {
		right = nullptr;
	}
	parent->left = this;
	if (isRoot) {
		isRoot = false;
		parent->isRoot = true;
	}
}

template<typename T> void Node<T>::splay() {
	while (!isRoot) {
		// zig
		if (parent->isRoot) {
			if (parent->left == this) {
				parent->rotR();
			}
			else {
				parent->rotL();
			}
		}
		// lefthanded zigzig 
		else if (parent->left == this && parent->parent->left == parent) {
			parent->parent->rotR();
			parent->rotR();
		}
		// righthanded zigzig
		else if (parent->right == this && parent->parent->right == parent) {
			parent->parent->rotL();
			parent->rotL();
		}
		// lefthanded zigzag
		else if (parent->right == this && parent->parent->left == parent) {
			parent->rotL();
			parent->rotR();
		}
		// righthanded zigzag
		else {
			parent->rotR();
			parent->rotL();
		}
	}
}

#endif

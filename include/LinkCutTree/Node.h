#ifndef LINK_CUT_TREE_NODE_H
#define LINK_CUT_TREE_NODE_H

#include <map>
#include <vector>
#include <bitset>

template<typename T> class Node {
public:
	Node* left, * right, * parent;
	T key;
	bool isRoot; // refers to aux tree
	Node() = default;
	Node(const T& key);

	void expose();
	bool link(Node* other, bool positional = false, bool placeLeft = true);
	void cut();
	Node* findRoot();
	Node* lowestCommonAncestor(Node* other);

	enum flagType { // maybe only include in dedicated subclass and restrict regular link
		IS_LEFT_CHILD,
		IS_RIGHT_CHILD,
		HAS_LEFT_CHILD,
		HAS_RIGHT_CHILD,
	};
	void setFlag(flagType type, bool value = 1);
	bool getFlag(flagType type);

private:
	std::bitset<4> _flags;
	void rotR();
	void rotL();
	void splay();
};

template<typename T> Node<T>::Node(const T& key)
	: left(nullptr), right(nullptr), parent(nullptr), key(key), isRoot(true), _flags(0) {
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

/*
* Find the lowest common ancestor between this Node and Node other.
* If this and other are not on the same represented tree nullptr is returned.
*/
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

template<typename T> void Node<T>::setFlag(flagType type, bool value) {
	_flags.set(type, value);
}

template<typename T> bool Node<T>::getFlag(flagType type) {
	return _flags[type];
}

/*
* Link the represented tree that has this node as its root to an arbitrary node in
* another tree. If this is not the root of the represened tree that it belongs to,
* false is returned. This method assumes that this and other are not on the same
* represented tree. Returns true on success.
*/
template<typename T> bool Node<T>::link(Node<T>* other, bool positional, bool placeLeft) {
	if (positional && other->getFlag(HAS_LEFT_CHILD) && placeLeft ||
		other->getFlag(HAS_RIGHT_CHILD) && !placeLeft) {
		return false;
	}
	expose();
	if (left) {
		return false; // v is not the root of its represented tree
	}
	other->expose();
	left = other;
	other->parent = this;
	other->isRoot = false;
	if (positional) {
		if (placeLeft) {
			other->setFlag(HAS_LEFT_CHILD);
			this->setFlag(IS_LEFT_CHILD);
		}
		else {
			other->setFlag(HAS_RIGHT_CHILD);
			this->setFlag(IS_RIGHT_CHILD);
		}
	}
	return true;
}

/*
* Remove the subtree of the represented tree that has this node as its root,
* creating a new represented tree. If this is already the root of the represented
* tree that this belongs to, this method has no effect on the represented tree.
*/
template<typename T> void Node<T>::cut() {
	expose();
	if (left) { // if v is root of the represented tree it has no left child after expose and cut does nothing
		left->isRoot = true;
		if (left->getFlag(HAS_LEFT_CHILD) && this->getFlag(IS_LEFT_CHILD)) {
			left->setFlag(HAS_LEFT_CHILD, 0);
			this->setFlag(IS_LEFT_CHILD, 0);
		}
		if (left->getFlag(HAS_RIGHT_CHILD) && this->getFlag(IS_RIGHT_CHILD)) {
			left->setFlag(HAS_RIGHT_CHILD, 0);
			this->setFlag(IS_RIGHT_CHILD, 0);
		}
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

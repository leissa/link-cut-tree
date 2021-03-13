#ifndef LINK_CUT_TREE_NODE_H
#define LINK_CUT_TREE_NODE_H

#include <map>
#include <vector>
#include <bitset>

template<typename T> class Node {
public:
	Node() = default;
	Node(const T& key);

	T& getKey();
	bool isRoot() const;
	void setRoot(bool aValue);

	Node* left() const; // look for alt. getter/setter model where setters are not effectively redundant
	Node* right() const; // or return pointers to const and disallow setting pointers from outside Node class
	Node* parent() const; // -> and change util class accordingly

	void setLeft(Node* aLeft);
	void setRight(Node* aRight);
	void setParent(Node* aParent);

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
	void setFlag(flagType type, bool value = true);
	bool getFlag(flagType type);

private:
	Node* _left, * _right, * _parent;
	T _key;
	bool _isRoot; // refers to aux tree
	std::bitset<4> _flags;
	void rotR();
	void rotL();
	void splay();
};

template<typename T> Node<T>::Node(const T& key)
	: _left(nullptr), _right(nullptr), _parent(nullptr), _key(key), _isRoot(true), _flags(0) {
}

template<typename T> void Node<T>::expose() {
	splay();
	if (_right) {
		_right->_isRoot = true; // change right's parent pointer to a path-parent pointer
		_right = nullptr;
	}
	while (_parent) {
		_parent->splay();
		if (_parent->_right) {
			_parent->_right->_isRoot = true;
		}
		_parent->_right = this;
		_isRoot = false;
		splay(); // single rotation around parent giving v the next path-parent pointer
	}
}

template<typename T> Node<T>* Node<T>::findRoot() {
	expose();
	Node* v = this;
	while (v->_left) {
		v = v->_left;
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
		while (!lca->_isRoot) {
			lca = lca->_parent;
		}
		return lca->_parent;
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
	if (_left) {
		return false; // v is not the root of its represented tree
	}
	other->expose();
	_left = other;
	other->_parent = this;
	other->_isRoot = false;
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
	if (_left) { // if v is root of the represented tree it has no left child after expose and cut does nothing
		_left->_isRoot = true;
		if (_left->getFlag(HAS_LEFT_CHILD) && this->getFlag(IS_LEFT_CHILD)) {
			_left->setFlag(HAS_LEFT_CHILD, 0);
			this->setFlag(IS_LEFT_CHILD, 0);
		}
		if (_left->getFlag(HAS_RIGHT_CHILD) && this->getFlag(IS_RIGHT_CHILD)) {
			_left->setFlag(HAS_RIGHT_CHILD, 0);
			this->setFlag(IS_RIGHT_CHILD, 0);
		}
		_left->_parent = nullptr; // left is on preferred path
		_left = nullptr;
	}
}

template<typename T> void Node<T>::rotR() {
	_left->_parent = _parent;
	if (_parent) {
		if (_parent->_left == this) {
			_parent->_left = _left;
		}
		if (_parent->_right == this) {
			_parent->_right = _left;
		}
	}
	_parent = _left;
	if (_left->_right) {
		_left->_right->_parent = this;
		_left = _left->_right;
	}
	else {
		_left = nullptr;
	}
	_parent->_right = this;
	if (_isRoot) {
		_isRoot = false;
		_parent->_isRoot = true;
	}
}

template<typename T> void Node<T>::rotL() {
	_right->_parent = _parent;
	if (_parent) {
		if (_parent->_right == this) {
			_parent->_right = _right;
		}
		if (_parent->_left == this) {
			_parent->_left = _right;
		}
	}
	_parent = _right;
	if (_right->_left) {
		_right->_left->_parent = this;
		_right = _right->_left;
	}
	else {
		_right = nullptr;
	}
	_parent->_left = this;
	if (_isRoot) {
		_isRoot = false;
		_parent->_isRoot = true;
	}
}

template<typename T> void Node<T>::splay() {
	while (!_isRoot) {
		// zig
		if (_parent->_isRoot) {
			if (_parent->_left == this) {
				_parent->rotR();
			}
			else {
				_parent->rotL();
			}
		}
		// lefthanded zigzig 
		else if (_parent->_left == this && _parent->_parent->_left == _parent) {
			_parent->_parent->rotR();
			_parent->rotR();
		}
		// righthanded zigzig
		else if (_parent->_right == this && _parent->_parent->_right == _parent) {
			_parent->_parent->rotL();
			_parent->rotL();
		}
		// lefthanded zigzag
		else if (_parent->_right == this && _parent->_parent->_left == _parent) {
			_parent->rotL();
			_parent->rotR();
		}
		// righthanded zigzag
		else {
			_parent->rotR();
			_parent->rotL();
		}
	}
}

template<typename T> Node<T>* Node<T>::left() const {
	return _left;
}

template<typename T> Node<T>* Node<T>::right() const {
	return _right;
}

template<typename T> Node<T>* Node<T>::parent() const {
	return _parent;
}

template<typename T> void Node<T>::setLeft(Node* aLeft) {
	_left = aLeft;
}

template<typename T> void Node<T>::setRight(Node* aRight) {
	_right = aRight;
}

template<typename T> void Node<T>::setParent(Node* aParent) {
	_parent = aParent;
}

template<typename T> T& Node<T>::getKey() {
	return _key;
}

template<typename T> bool Node<T>::isRoot() const {
	return _isRoot;
}

template<typename T> void Node<T>::setRoot(bool aValue) {
	_isRoot = aValue;
}

#endif

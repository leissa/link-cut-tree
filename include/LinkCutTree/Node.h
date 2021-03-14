#ifndef LINK_CUT_TREE_NODE_H
#define LINK_CUT_TREE_NODE_H

#include <map>
#include <vector>
#include <bitset>

template<typename T> class Node {
public:
	Node() = default;
	Node(const T& key, int aID = _idCounter++);

	T& getKey();
	int getID();
	static int _idCounter;
	void expose();

	// the following methods refer to the auxiliary tree
	bool isRoot() const;
	void setRoot(bool aValue);

	Node* left() const; // look for alt. getter/setter model where setters are not effectively redundant
	Node* right() const; // or return pointers to const and disallow setting pointers from outside Node class
	Node* parent() const; // -> and change util class accordingly

	void setLeft(Node* aLeft);
	void setRight(Node* aRight);
	void setParent(Node* aParent);

	// the following refer to the represented tree (including all types of flags)
	bool link(Node* aOther);
	bool linkLeft(Node* aOther);
	bool linkRight(Node* aOther);

	void cut();

	Node* findRoot();
	Node* findParent();
	Node* lowestCommonAncestor(Node* aOther);

	enum flagType { // maybe only include in dedicated subclass and restrict regular link
		IS_LEFT_CHILD,
		IS_RIGHT_CHILD,
		HAS_LEFT_CHILD,
		HAS_RIGHT_CHILD,
	};
	void setFlag(flagType aType, bool aValue = true);
	bool getFlag(flagType aType);

private:
	Node* _left, * _right, * _parent;
	T _key;
	int _id;
	bool _isRoot;
	std::bitset<4> _flags;
	void rotR();
	void rotL();
	void splay();
};

template<typename T> Node<T>::Node(const T& aKey, int aID) : _left(nullptr), _right(nullptr),
_parent(nullptr), _key(aKey), _isRoot(true), _flags(0), _id(aID) {
}

template<typename T> int Node<T>::_idCounter = 0;

template<typename T> int Node<T>::getID() {
	return _id;
}

template<typename T> void Node<T>::expose() {
	splay();
	if (_right) {
		_right->_isRoot = true;
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
	Node* lRoot = this;
	while (lRoot->_left) {
		lRoot = lRoot->_left;
	}
	lRoot->splay();
	return lRoot;
}

/*
* Find the lowest common ancestor between this Node and Node other.
* If this and other are not on the same represented tree nullptr is returned.
*/
template<typename T> Node<T>* Node<T>::lowestCommonAncestor(Node* aOther) {
	Node* lRoot = this->findRoot();
	if (lRoot != aOther->findRoot()) {
		return nullptr;
	}
	else if (lRoot == this || aOther == this) {
		return this;
	}
	else if (lRoot == aOther) {
		return aOther;
	}
	else {
		this->expose();
		aOther->expose();
		Node* lLca = this;
		while (!lLca->_isRoot) {
			lLca = lLca->_parent;
		}
		return lLca->_parent;
	}
}

template<typename T> void Node<T>::setFlag(flagType aType, bool aValue) {
	_flags.set(aType, aValue);
}

template<typename T> bool Node<T>::getFlag(flagType aType) {
	return _flags[aType];
}

/*
* Link the represented tree that has this node as its root to an arbitrary node in
* another tree. If this is not the root of the represened tree that it belongs to,
* false is returned. This method assumes that this and other are not on the same
* represented tree. Returns true on success.
*/
template<typename T> bool Node<T>::link(Node<T>* aOther) {
	expose();
	if (_left) {
		return false; // v is not the root of its represented tree
	}
	aOther->expose();
	_left = aOther;
	aOther->_parent = this;
	aOther->_isRoot = false;
	return true;
}

template<typename T> bool Node<T>::linkLeft(Node<T>* aOther) {
	if (aOther->getFlag(HAS_LEFT_CHILD) || !link(aOther)) {
		return false;
	}
	else {
		aOther->setFlag(HAS_LEFT_CHILD);
		this->setFlag(IS_LEFT_CHILD);
		return true;
	}
}

template<typename T> bool Node<T>::linkRight(Node<T>* aOther) {
	if (aOther->getFlag(HAS_RIGHT_CHILD) || !link(aOther)) {
		return false;
	}
	else {
		aOther->setFlag(HAS_RIGHT_CHILD);
		this->setFlag(IS_RIGHT_CHILD);
		return true;
	}
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
		// TODO: fix flags using findparent
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

#ifndef LINK_CUT_TREE_NODE_H
#define LINK_CUT_TREE_NODE_H

template<typename T> class LctNode {
public:
	LctNode(const T& aContent, int aID = idCounter++);

	const T& getContent() const;
	T& getContent();
	int getID();

	virtual bool link(LctNode* aOther);
	virtual void cut();

	virtual LctNode* findRoot();
	virtual LctNode* findParent();
	virtual LctNode* lowestCommonAncestor(LctNode* aOther);
	virtual bool isDescendant(LctNode* aOther);
	template<typename F> void path(F aFunction);
	template<typename F> LctNode<T>* find_if(F aFunction);

	int getVirtualSize();
	int getRealSize();
	int lengthToRoot();

	friend class LctUtils;
	static int idCounter;

protected:
	LctNode* _left, * _right, * _parent;
	T _content;
	int _id;
	void rotR();
	void rotL();
	void splay();
	void expose();
	bool isRoot();

	int _sizeVirtual;
	int _sizeSubtree;
	int _sumVirtual;

	// call from inheriting method
	virtual void update_aggregate(LctNode<T>* aLeft, LctNode<T>* aRight);
	virtual void update_aggregate_expose(LctNode<T>* aNewChild, LctNode<T>* aFormerChild);
	virtual void update_aggregate_link(LctNode<T>* aNewChild);
};

template<typename T> LctNode<T>::LctNode(const T& aContent, int aID) : _left(nullptr), _right(nullptr),
_parent(nullptr), _content(aContent), _id(aID), _sizeVirtual(0), _sizeSubtree(1), _sumVirtual(0) {}

template<typename T> int LctNode<T>::idCounter = 0;

template<typename T> int LctNode<T>::getID() {
	return _id;
}

template<typename T> void LctNode<T>::expose() {
	splay();
	while (_parent) {
		_parent->splay();
		_parent->update_aggregate_expose(this, _parent->_right);
		_parent->_right = this;
		splay(); // single rotation around parent giving v the next path-parent pointer
	}
}

template<typename T> LctNode<T>* LctNode<T>::findRoot() {
	expose();
	LctNode* lRoot = this;
	while (lRoot->_left) {
		lRoot = lRoot->_left;
	}
	lRoot->splay();
	return lRoot;
}

template<typename T> LctNode<T>* LctNode<T>::findParent() {
	LctNode* lTemp = this;
	if (_left) { // parent must be found in left subtree
		lTemp = _left;
		while (lTemp->_right) {
			lTemp = lTemp->_right;
		}
		return lTemp;
	}
	else { // parent is either on path from this to root (of aux) or the parent of root (which can be null)
		if (_parent) {
			while (true) {
				if (lTemp->isRoot()) {
					return lTemp->_parent;
				}
				else {
					if (lTemp->_parent->_right == lTemp) {
						return lTemp->_parent;
					}
					else {
						lTemp = lTemp->_parent;
					}
				}
			}
		}
		else { // there exists no left subtree and lTemp has no (path)parent -> lTemp is root of repr. tree
			return nullptr;
		}
	}
}

/*
* Find the lowest common ancestor between this Node and Node other.
* If this and other are not on the same represented tree nullptr is returned.
*/
template<typename T> LctNode<T>* LctNode<T>::lowestCommonAncestor(LctNode* aOther) {
	if (this == aOther) {
		return this;
	}
	else if (findRoot() != aOther->findRoot()) {
		return nullptr;
	}
	else {
		this->expose();
		aOther->expose();
		if (isRoot()) { // zig or zig-zag has been performed before slice, also this cant be root if on same splay as aOther
			return _parent;
		}
		else if (_parent->isRoot() && _parent->_parent != nullptr) { // zig-zig has been performed
			return _parent->_parent;
		}
		else {
			// this must be on same splay tree as aOther and has depth of 1 or 2 (depending on zig/zig-zag or zig-zig)
			if (_parent->_left == this) { // same test for zig/zig-zag and zig-zig, if true, this is in left subtree of aOther
				return this;
			}
			else { // this is in right subtree of aOther
				return aOther;
			}
		}
	}
}

/**
* function object aFunction must have signature
*	void aFunction(Node* aNode)
* calls aFunction on every node on the path from this to the root of the repr. tree
*/
template<typename T> template<typename F> void LctNode<T>::path(F aFunction)
{
	LctNode* lTemp = this;
	do {
		aFunction(lTemp);
	} while ((lTemp = lTemp->findParent()) != nullptr);
}

/**
* function object aFunction must have signature
*	bool aFunction(Node* aNode)
* calls aFunction on every node on the path from this to the root of the repr. tree
* and returns a pointer to the first node for which aFunction evaluates to true
*/
template<typename T> template<typename F> LctNode<T>* LctNode<T>::find_if(F aFunction)
{
	LctNode* lTemp = this;
	do {
		if (aFunction(lTemp)) {
			return lTemp;
		}
	} while ((lTemp = lTemp->findParent()) != nullptr);
	return nullptr;
}

/**
* Return true if node this is a descendant of node aOther.
* If this equals aOther true is returned.
*/
template<typename T> bool LctNode<T>::isDescendant(LctNode<T>* aOther) {
	expose();
	aOther->expose();
	if (isRoot() || (_parent->isRoot() && _parent->_parent != nullptr)) {
		// this is not on preferred path with root -> node in left subtree of this has been exposed when aOther was exposed
		// this only happens if this and aOther have an lca that is not this -> aOther is not a descendant
		return false;
	}
	else { // this and aOther are on same preferred path -> test if this is higher or lower than aOther
		// -> same as checking if this has been rotated into the left or right subtree of aOther
		if (_parent->_left == this) { // this is in the left subtree of aOther -> higher in the tree -> not a descendant
			return false;
		}
		else {
			return true;
		}
	}
}

/*
* Link the represented tree that has this node as its root to an arbitrary node in
* another tree. If this is not the root of the represened tree that it belongs to,
* false is returned. This method assumes that this and other are not on the same
* represented tree. Returns true on success.
*/
template<typename T> bool LctNode<T>::link(LctNode<T>* aOther) {
	expose();
	if (_left) {
		return false; // v is not the root of its represented tree
	}
	aOther->expose();
	_parent = aOther;
	aOther->update_aggregate_link(this);
	aOther->update_aggregate(aOther->_left, aOther->_right);
	return true;
}

template<typename T> bool LctNode<T>::isRoot() {
	return !_parent || (_parent->_left != this && _parent->_right != this);
}

/*
* Remove the subtree of the represented tree that has this node as its root,
* creating a new represented tree. If this is already the root of the represented
* tree that this belongs to, this method has no effect on the represented tree.
*/
template<typename T> void LctNode<T>::cut() {
	expose();
	if (_left) { // if v is root of the represented tree it has no left child after expose and cut does nothing
		_left->_parent = nullptr; // left is on preferred path
		_left = nullptr;
		update_aggregate(nullptr, _right);
	}
}

/**
*         P            P
*        /            /
*       X            B
*      / \    ->    / \
*     B   C        D   X
*    / \              / \
*   D   E            E   C
*
* Rotate the tree around the edge connecting X's left child(B) to this(X).
* This assumes that X has a left child.
*/
template<typename T> void LctNode<T>::rotR() {
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
	update_aggregate(_left, _right);
	_parent->update_aggregate(_parent->_left, _parent->_right);
}

/**
*         P              P
*        /              /
*       X              B
*      / \     ->     / \
*     C   B          X   E
*        / \        / \
*       D   E      C   D
*
* Rotate the tree around the edge connecting X's right child(B) to this(X).
* This assumes that X has a right child.
*/
template<typename T> void LctNode<T>::rotL() {
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
	update_aggregate(_left, _right);
	_parent->update_aggregate(_parent->_left, _parent->_right);
}

/**
* Move this node to the root of its tree by repeatedly performing left and right rotations
* around the edge connecting this to its parent based on the relative positioning between
* this, the parent of this and the grandparent of this
*/
template<typename T> void LctNode<T>::splay() {
	while (!isRoot()) {
		// zig
		if (_parent->isRoot()) {
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

template<typename T> void LctNode<T>::update_aggregate(LctNode<T>* left, LctNode<T>* right) {
	_sizeSubtree = (left ? left->_sizeSubtree : 0) + (right ? right->_sizeSubtree : 0) + _sizeVirtual + 1;
	_sumVirtual = (left ? left->_sumVirtual : 0) + (right ? right->_sumVirtual : 0) + _sizeVirtual;
}

template<typename T> void LctNode<T>::update_aggregate_expose(LctNode<T>* aNewChild, LctNode<T>* aFormerChild) {
	if (aNewChild) {
		_sizeVirtual -= aNewChild->_sizeSubtree;
		_sumVirtual -= aNewChild->_sizeSubtree;
	}
	if (aFormerChild) {
		_sizeVirtual += aFormerChild->_sizeSubtree;
		_sumVirtual += aFormerChild->_sizeSubtree;
	}
}

template<typename T> void LctNode<T>::update_aggregate_link(LctNode<T>* aNewChild) {
	_sizeVirtual += aNewChild->_sizeSubtree;
	_sumVirtual += aNewChild->_sizeSubtree;
}

// return the size of the subtree of the virtual tree rooted at this
template<typename T> int LctNode<T>::getVirtualSize() // no expose necessary because invariant does not depend on info higher up in the virtual tree
{
	return _sizeSubtree;
}

// return size of the subtree of the represented tree rooted at this
template<typename T> int LctNode<T>::getRealSize()
{
	expose(); // needs expose because possible right subtrees of ancestors contain nodes that are descendants of this
	return (_right ? _right->_sizeSubtree : 0) + _sizeVirtual + 1;
}

template<typename T> int LctNode<T>::lengthToRoot() {
	expose();
	return _left ? (_left->_sizeSubtree - _left->_sumVirtual) : 0;
}

template<typename T> const T& LctNode<T>::getContent() const {
	return _content;
}

template<typename T> T& LctNode<T>::getContent() {
	return _content;
}

#endif

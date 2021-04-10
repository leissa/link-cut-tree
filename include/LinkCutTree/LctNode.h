#ifndef LINK_CUT_TREE_NODE_H
#define LINK_CUT_TREE_NODE_H

/**
* type T must implement methods
*	void update_aggregate(IntWrapper* aLeft, IntWrapper* aRight)
*	void update_aggregate_expose(IntWrapper* aNewChild, IntWrapper* aFormerChild)
*	void update_aggregate_link(IntWrapper* aNewChild)
*/
template<typename T> class LctNode {
public:
	LctNode();
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

	friend class LctUtils;
	static int idCounter;

protected:
	LctNode* _left, * _right, * _parent;
	T _content;
	int _id;
	bool _isRoot;
	void rotR();
	void rotL();
	void splay();
	void expose();
};

template<typename T> LctNode<T>::LctNode()
	: _left(nullptr), _right(nullptr), _parent(nullptr), _content(T()), _isRoot(true), _id(idCounter++) {}

template<typename T> LctNode<T>::LctNode(const T& aContent, int aID) : _left(nullptr), _right(nullptr),
_parent(nullptr), _content(aContent), _isRoot(true), _id(aID) {}

template<typename T> int LctNode<T>::idCounter = 0;

template<typename T> int LctNode<T>::getID() {
	return _id;
}

template<typename T> void LctNode<T>::expose() {
	splay();
	if (_right) {
		_content.update_aggregate_expose(nullptr, &(_right->_content));
		_content.update_aggregate(_left ? &(_left->_content) : nullptr, nullptr);
		_right->_isRoot = true;
		_right = nullptr;
	}
	else {
		_content.update_aggregate(_left ? &(_left->_content) : nullptr, nullptr);
	}
	while (_parent) {
		_parent->splay();
		if (_parent->_right) {
			_parent->_right->_isRoot = true;
		}
		_parent->_content.update_aggregate_expose(&(this->_content), _parent->_right ? &(_parent->_right->_content) : nullptr);
		_parent->_content.update_aggregate(_parent->_left ? &(_parent->_left->_content) : nullptr, &(this->_content));
		_parent->_right = this;
		_isRoot = false;
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
				if (lTemp->_isRoot) {
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
	if (aOther->isDescendant(this)) { // also checks for equality
		return this;
	}
	LctNode* lRoot = this->findRoot();
	if (lRoot != aOther->findRoot()) {
		return nullptr;
	}
	else if (lRoot == this) {
		return this;
	}
	else if (lRoot == aOther) {
		return aOther;
	}
	else {
		this->expose();
		aOther->expose();
		LctNode* lLca = this;
		while (!lLca->_isRoot) {
			lLca = lLca->_parent;
		}
		return lLca->_parent;
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
	LctNode* lTemp = this;
	do {
		if (lTemp == aOther) {
			return true;
		}
	} while ((lTemp = lTemp->findParent()) != nullptr);
	return false;
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
	aOther->_content.update_aggregate_link(&(this->_content));
	aOther->_content.update_aggregate(aOther->_left ? &(aOther->_left->_content) : nullptr, aOther->_right ? &(aOther->_right->_content) : nullptr);
	return true;
}

/*
* Remove the subtree of the represented tree that has this node as its root,
* creating a new represented tree. If this is already the root of the represented
* tree that this belongs to, this method has no effect on the represented tree.
*/
template<typename T> void LctNode<T>::cut() {
	expose();
	if (_left) { // if v is root of the represented tree it has no left child after expose and cut does nothing
		_left->_isRoot = true;
		_left->_parent = nullptr; // left is on preferred path
		_left = nullptr;
		_content.update_aggregate(nullptr, _right ? &(_right->_content) : nullptr);
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
	if (_isRoot) {
		_isRoot = false;
		_parent->_isRoot = true;
	}
	_content.update_aggregate(_left ? &(_left->_content) : nullptr, _right ? &(_right->_content) : nullptr);
	_parent->_content.update_aggregate(_parent->_left ? &(_parent->_left->_content) : nullptr, _parent->_right ? &(_parent->_right->_content) : nullptr);
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
	if (_isRoot) {
		_isRoot = false;
		_parent->_isRoot = true;
	}
	_content.update_aggregate(_left ? &(_left->_content) : nullptr, _right ? &(_right->_content) : nullptr);
	_parent->_content.update_aggregate(_parent->_left ? &(_parent->_left->_content) : nullptr, _parent->_right ? &(_parent->_right->_content) : nullptr);
}

/**
* Move this node to the root of its tree by repeatedly performing left and right rotations
* around the edge connecting this to its parent based on the relative positioning between
* this, the parent of this and the grandparent of this
*/
template<typename T> void LctNode<T>::splay() {
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

template<typename T> const T& LctNode<T>::getContent() const {
	return _content;
}

template<typename T> T& LctNode<T>::getContent() {
	return _content;
}

#endif

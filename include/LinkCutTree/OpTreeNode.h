#ifndef OP_TREE_NODE_H
#define OP_TREE_NODE_H

#include "LctNode.h"
#include <vector>

template<typename T> class OpTreeNode : public LctNode<T> {
public:
    using Node = LctNode<T>;
	OpTreeNode(const T& aContent, int aID = Node::idCounter++);

	OpTreeNode* findRoot();
	OpTreeNode* findParent();
	OpTreeNode* findChild();
	OpTreeNode* lowestCommonAncestor(Node* aOther);

	enum flagType {
		IS_LEFT_CHILD,
		IS_RIGHT_CHILD,
		IS_ONLY_CHILD,
		HAS_LEFT_CHILD,
		HAS_RIGHT_CHILD,
		HAS_ONLY_CHILD
	};

	void cut();
	bool link(Node* aOther);

	bool isLeftDescendant(OpTreeNode<T>* aOther);
	bool isRightDescendant(OpTreeNode<T>* aOther);

	bool linkLeft(OpTreeNode* aOther);
	bool linkRight(OpTreeNode* aOther);
	bool linkOnly(OpTreeNode* aOther);

	friend class LctUtils;
private:
	std::vector<bool> _flags;
};

template<typename T> OpTreeNode<T>::OpTreeNode(const T& aContent, int aID) : LctNode<T>(aContent, aID), _flags(std::vector<bool>(6, false)) {}

template<typename T> void OpTreeNode<T>::cut() {
	Node::expose();
	if (Node::_left) { // if v is root of the represented tree it has no left child after expose and cut does nothing
		OpTreeNode* lParent = findParent();
		if (lParent->_flags[HAS_LEFT_CHILD] && _flags[IS_LEFT_CHILD]) {
			lParent->_flags[HAS_LEFT_CHILD] = 0;
			this->_flags[IS_LEFT_CHILD] = 0;
		}
		if (lParent->_flags[HAS_RIGHT_CHILD] && _flags[IS_RIGHT_CHILD]) {
			lParent->_flags[HAS_RIGHT_CHILD] = 0;
			_flags[IS_RIGHT_CHILD] = 0;
		}
		if (lParent->_flags[HAS_ONLY_CHILD]) {
			lParent->_flags[HAS_ONLY_CHILD] = 0;
			this->_flags[IS_ONLY_CHILD] = 0;
		}
		static_cast<OpTreeNode<T>*>(Node::_left)->_parent = nullptr; // left is on preferred path
        Node::_left = nullptr;
        Node::update_aggregate();
	}
}

template<typename T> bool OpTreeNode<T>::link(Node*) {
	return false;
}

template<typename T> bool OpTreeNode<T>::linkLeft(OpTreeNode<T>* aOther) {
	if (aOther->_flags[HAS_ONLY_CHILD] || aOther->_flags[HAS_LEFT_CHILD]) {
		return false;
	}
	else {
		if (Node::link(aOther)) {
			aOther->_flags[HAS_LEFT_CHILD] = 1;
			this->_flags[IS_LEFT_CHILD] = 1;
			return true;
		}
		else {
			return false;
		}
	}
}

template<typename T> bool OpTreeNode<T>::linkRight(OpTreeNode<T>* aOther) {
	if (aOther->_flags[HAS_ONLY_CHILD] || aOther->_flags[HAS_RIGHT_CHILD]) {
		return false;
	}
	else {
		if (Node::link(aOther)) {
			aOther->_flags[HAS_RIGHT_CHILD] = 1;
			this->_flags[IS_RIGHT_CHILD] = 1;
			return true;
		}
		else {
			return false;
		}
	}
}

template<typename T> bool OpTreeNode<T>::linkOnly(OpTreeNode<T>* aOther) {
	if (aOther->_flags[HAS_LEFT_CHILD] || aOther->_flags[HAS_RIGHT_CHILD] || aOther->_flags[HAS_ONLY_CHILD]) {
		return false;
	}
	else {
		if (Node::link(aOther)) {
			_flags[IS_ONLY_CHILD] = 1;
			aOther->_flags[HAS_ONLY_CHILD] = 1;
			return true;
		}
		else {
			return false;
		}
	}
}

template<typename T> bool OpTreeNode<T>::isLeftDescendant(OpTreeNode<T>* aOther) {
	if (Node::isDescendant(aOther)) {
		// child must exist, either this or node on path from this to aOther
		if (aOther->findChild()->_flags[IS_LEFT_CHILD]) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

template<typename T> bool OpTreeNode<T>::isRightDescendant(OpTreeNode<T>* aOther) {
	if (Node::isDescendant(aOther)) {
		if (aOther->findChild()->_flags[IS_RIGHT_CHILD]) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

template<typename T> OpTreeNode<T>* OpTreeNode<T>::findRoot()
{
	return static_cast<OpTreeNode<T>*>(Node::findRoot());
}

template<typename T> OpTreeNode<T>* OpTreeNode<T>::findParent()
{
	return static_cast<OpTreeNode<T>*>(Node::findParent());
}
template<typename T> OpTreeNode<T>* OpTreeNode<T>::findChild()
{
	return static_cast<OpTreeNode<T>*>(Node::findChild());
}

template<typename T> OpTreeNode<T>* OpTreeNode<T>::lowestCommonAncestor(Node* aOther)
{
	return static_cast<OpTreeNode<T>*>(Node::lowestCommonAncestor(aOther));
}

#endif

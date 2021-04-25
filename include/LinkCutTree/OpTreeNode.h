#ifndef OP_TREE_NODE_H
#define OP_TREE_NODE_H

#include "LctNode.h"
#include <bitset>

template<typename T> class OpTreeNode : public LctNode<T> {
public:
	OpTreeNode() = default;
	OpTreeNode(const T& aContent, int aID = idCounter++);

	OpTreeNode* findRoot();
	OpTreeNode* findParent();
	OpTreeNode* lowestCommonAncestor(LctNode<T>* aOther);

	enum flagType {
		IS_LEFT_CHILD,
		IS_RIGHT_CHILD,
		IS_ONLY_CHILD,
		HAS_LEFT_CHILD,
		HAS_RIGHT_CHILD,
		HAS_ONLY_CHILD
	};
	void setFlag(flagType aType, bool aValue = true);
	bool getFlag(flagType aType);

	void cut();

	bool linkLeft(OpTreeNode* aOther);
	bool linkRight(OpTreeNode* aOther);
	bool linkOnly(OpTreeNode* aOther);
private:
	std::bitset<6> _flags;
	using LctNode::link;
};

template<typename T> OpTreeNode<T>::OpTreeNode(const T& aContent, int aID) : LctNode<T>(aContent, aID), _flags(0) {}

template<typename T> void OpTreeNode<T>::cut() {
	expose();
	if (_left) { // if v is root of the represented tree it has no left child after expose and cut does nothing
		OpTreeNode* lParent = findParent();
		if (lParent->getFlag(HAS_LEFT_CHILD) && getFlag(IS_LEFT_CHILD)) {
			lParent->setFlag(HAS_LEFT_CHILD, 0);
			this->setFlag(IS_LEFT_CHILD, 0);
		}
		if (lParent->getFlag(HAS_RIGHT_CHILD) && getFlag(IS_RIGHT_CHILD)) {
			lParent->setFlag(HAS_RIGHT_CHILD, 0);
			this->setFlag(IS_RIGHT_CHILD, 0);
		}
		if (lParent->getFlag(HAS_ONLY_CHILD)) {
			lParent->setFlag(HAS_ONLY_CHILD, 0);
			this->setFlag(IS_ONLY_CHILD, 0);
		}
		dynamic_cast<OpTreeNode<T>*>(_left)->_parent = nullptr; // left is on preferred path
		_left = nullptr;
	}
}


template<typename T> void OpTreeNode<T>::setFlag(flagType aType, bool aValue) {
	_flags.set(aType, aValue);
}

template<typename T> bool OpTreeNode<T>::getFlag(flagType aType) {
	return _flags[aType];
}

template<typename T> bool OpTreeNode<T>::linkLeft(OpTreeNode<T>* aOther) {
	if (aOther->getFlag(HAS_ONLY_CHILD) || aOther->getFlag(HAS_LEFT_CHILD) || !link(aOther)) {
		return false;
	}
	else {
		aOther->setFlag(HAS_LEFT_CHILD);
		this->setFlag(IS_LEFT_CHILD);
		return true;
	}
}

template<typename T> bool OpTreeNode<T>::linkRight(OpTreeNode<T>* aOther) {
	if (aOther->getFlag(HAS_ONLY_CHILD) || aOther->getFlag(HAS_RIGHT_CHILD) || !link(aOther)) {
		return false;
	}
	else {
		aOther->setFlag(HAS_RIGHT_CHILD);
		this->setFlag(IS_RIGHT_CHILD);
		return true;
	}
}

template<typename T> bool OpTreeNode<T>::linkOnly(OpTreeNode<T>* aOther) {
	if (aOther->getFlag(HAS_LEFT_CHILD) || aOther->getFlag(HAS_RIGHT_CHILD) || aOther->getFlag(HAS_ONLY_CHILD)) {
		return false;
	}
	else {
		if (link(aOther)) {
			setFlag(IS_ONLY_CHILD);
			aOther->setFlag(HAS_ONLY_CHILD);
			return true;
		}
		else {
			return false;
		}
	}
}

template<typename T> OpTreeNode<T>* OpTreeNode<T>::findRoot()
{
	return dynamic_cast<OpTreeNode<T>*>(LctNode<T>::findRoot());
}

template<typename T> OpTreeNode<T>* OpTreeNode<T>::findParent()
{
	return dynamic_cast<OpTreeNode<T>*>(LctNode<T>::findParent());
}

template<typename T> OpTreeNode<T>* OpTreeNode<T>::lowestCommonAncestor(LctNode<T>* aOther)
{
	return dynamic_cast<OpTreeNode<T>*>(LctNode<T>::lowestCommonAncestor(aOther));
}

#endif
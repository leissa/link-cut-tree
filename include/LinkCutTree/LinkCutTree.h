#ifndef LINK_CUT_TREE_H
#define LINK_CUT_TREE_H

#include <map>
#include "LctNode.h"

template<typename V, template<typename> class T = LctNode> class LinkCutTree {
public:
	LinkCutTree<V, T>();
	T<V>* createTree(const V& aContent, int id = T<V>::idCounter++);
	T<V>* operator[](int aID);

private:
	std::map<int, T<V>> _nodes;
};

template<typename V, template<typename> class T> LinkCutTree<V, T>::LinkCutTree() : _nodes(std::map<int, T<V>>()) {}

template<typename V, template<typename> class T> T<V>* LinkCutTree<V, T>::createTree(const V& aContent, int aID)
{
	if (!this->_nodes.count(aID)) {
		_nodes.insert(std::make_pair(aID, T<V>(aContent, aID)));
	}
	return &(this->_nodes.find(aID)->second);
}

template<typename V, template<typename> class T> T<V>* LinkCutTree<V, T>::operator[](int aID)
{
	if (this->_nodes.count(aID)) {
		return &(this->_nodes.find(aID)->second);
	}
	else {
		return nullptr;
	}
}

#endif

#ifndef LINK_CUT_TREE_H
#define LINK_CUT_TREE_H

#include <map>
#include "LctNode.h"

template<typename V, template<typename> class T = LctNode> class LinkCutTree {
public:
	LinkCutTree<V, T>();
	T<V>* createTree(const V& aContent, int id = T<V>::idCounter++);
	T<V>* operator[](const V& aContent);

private:
	std::map<V, T<V>> _nodes;
};

template<typename V, template<typename> class T> LinkCutTree<V, T>::LinkCutTree() : _nodes(std::map<V, T<V>>()) {}

template<typename V, template<typename> class T> T<V>* LinkCutTree<V, T>::createTree(const V& aContent, int aID)
{
	if (!this->_nodes.count(aContent)) {
		_nodes.insert(std::make_pair(aContent, T<V>(aContent, aID)));
	}
	return &(this->_nodes.find(aContent)->second);
}

template<typename V, template<typename> class T> T<V>* LinkCutTree<V, T>::operator[](const V& aContent)
{
	if (this->_nodes.count(aContent)) {
		return &(this->_nodes.find(aContent)->second);
	}
	else {
		return nullptr;
	}
}

#endif

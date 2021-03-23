#ifndef LINK_CUT_TREE_H
#define LINK_CUT_TREE_H

#include <map>
#include <vector>
#include "Node.h"

template<typename V, template<typename> class T = Node> class LinkCutTree {
public:
	LinkCutTree<V, T>();
	T<V>* createTree(const V& aKey, int id = T<V>::idCounter++);
	T<V>* operator[](const V& aKey);

private:
	std::map<V, T<V>> _nodes;
};

template<typename V, template<typename> class T> LinkCutTree<V, T>::LinkCutTree() : _nodes(std::map<V, T<V>>()) {}

template<typename V, template<typename> class T> T<V>* LinkCutTree<V, T>::createTree(const V& aKey, int aID)
{
	if (!this->_nodes.count(aKey)) {
		_nodes.insert(std::make_pair(aKey, T<V>(aKey, aID)));
	}
	return &(this->_nodes[aKey]);
}

template<typename V, template<typename> class T> T<V>* LinkCutTree<V, T>::operator[](const V& aKey)
{
	if (this->_nodes.count(aKey)) {
		return &(this->_nodes[aKey]);
	}
	else {
		return nullptr;
	}
}

#endif

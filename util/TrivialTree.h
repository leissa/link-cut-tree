#ifndef TRIVIAL_TREE
#define TRIVIAL_TREE

#include "TrivialTreeNode.h"

class TrivialTree {
public:
	TrivialTree();
	TrivialTreeNode* createTree(int id = idCounter++);
	TrivialTreeNode* operator[](const int id);
	void clear();

private:
	static int idCounter;
	std::map<int, TrivialTreeNode> _nodes;
};

TrivialTree::TrivialTree() :_nodes(std::map<int, TrivialTreeNode>()) {}

TrivialTreeNode* TrivialTree::createTree(int id)
{
	if (!this->_nodes.count(id)) {
		_nodes.insert(std::make_pair(id, TrivialTreeNode(id)));
	}
	return &(this->_nodes.find(id)->second);
}

TrivialTreeNode* TrivialTree::operator[](const int id)
{
	if (this->_nodes.count(id)) {
		return &(this->_nodes.find(id)->second);
	}
	else {
		return nullptr;
	}
}


void TrivialTree::clear() {
	_nodes.clear();
}

#endif

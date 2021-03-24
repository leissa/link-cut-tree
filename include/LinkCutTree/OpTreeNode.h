#ifndef OP_TREE_NODE_H
#define OP_TREE_NODE_H

#include "Node.h"

template<typename T> class OpTreeNode : public Node<T> {
public:
	using Node::Node;
private:
};

#endif
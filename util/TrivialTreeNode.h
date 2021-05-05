#ifndef TRIVIAL_TREE_NODE
#define TRIVIAL_TREE_NODE

class TrivialTreeNode {
public:
	TrivialTreeNode(int id);
	TrivialTreeNode* findRoot();
	void link(TrivialTreeNode* aParent);
	int getID();
private:
	TrivialTreeNode* _parent;
	int _id;
};

TrivialTreeNode::TrivialTreeNode(int id) : _parent(nullptr), _id(id) {}

TrivialTreeNode* TrivialTreeNode::findRoot() {
	TrivialTreeNode* lCurrent = this;
	while (lCurrent->_parent) {
		lCurrent = lCurrent->_parent;
	}
	return lCurrent;
}

void TrivialTreeNode::link(TrivialTreeNode* aParent) {
	_parent = aParent;
}

int TrivialTreeNode::getID() {
	return _id;
}

#endif

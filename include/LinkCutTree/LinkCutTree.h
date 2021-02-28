#ifndef LINK_CUT_TREE_H
#define LINK_CUT_TREE_H

template<typename T> class LinkCutTree {
public:
	struct Node {
		Node* left, * right, * parent;
		T key;
		// if node is the root of the splay tree then treat the parent pointer as the path-parent pointer
		bool isRoot;
		Node(const T& key) :left(nullptr), right(nullptr), parent(nullptr), key(key), isRoot(true) {};
	};
	// maybe map key to pointers and do not expose node pointers, or do both
	Node& createTree(const T& key) {
		return Node(key);
	}
	Node* findRoot(Node& v);
	void link(Node& v, Node& w);
	void cut(Node& v);

public:
	void rotR(Node* v) {
		v->left->parent = v->parent;
		if (v->parent) {
			if (v->parent->left == v) {
				v->parent->left = v->left;
			}
			else {
				v->parent->right = v->left;
			}
		}
		v->parent = v->left;
		if (v->left->right) {
			v->left->right->parent = v;
			v->left = v->left->right;
		}
		else {
			v->left = nullptr;
		}
		v->parent->right = v;
	}

	void rotL(Node* v) {
		v->right->parent = v->parent;
		if (v->parent) {
			if (v->parent->right == v) {
				v->parent->right = v->right;
			}
			else {
				v->parent->left = v->right;
			}
		}
		v->parent = v->right;
		if (v->right->left) {
			v->right->left->parent = v;
			v->right = v->right->left;
		}
		else {
			v->right = nullptr;
		}
		v->parent->left = v;
	}

	// make sure to set isRoot on root of tree with v to false before rotations
	void splay(Node* v) {
		Node* root = v;
		while (!root->isRoot) {
			root = root->parent;
		}
		root->isRoot = false;
		// pathParent pointer is null only if root is the root of the represented tree
		Node* pathParent = root->parent;
		while (v->parent != pathParent) { // v is not root yet
			// zig
			if (v->parent->parent == pathParent) {
				if (v->parent->left == v) {
					rotR(v->parent);
				}
				else {
					rotL(v->parent);
				}
			}
			// lefthanded zigzig 
			else if (v->parent->left == v && v->parent->parent->left == v->parent) {
				rotR(v->parent->parent);
				rotR(v->parent);
			}
			// righthanded zigzig
			else if (v->parent->right == v && v->parent->parent->right == v->parent) {
				rotL(v->parent->parent);
				rotL(v->parent);
			}
			// lefthanded zigzag
			else if (v->parent->right == v && v->parent->parent->left == v->parent) {
				rotL(v->parent);
				rotR(v->parent);
			}
			// righthanded zigzag
			else {
				rotR(v->parent);
				rotL(v->parent);
			}
		}
		v->isRoot = true;
	}
	void expose(Node& v);
};

#endif
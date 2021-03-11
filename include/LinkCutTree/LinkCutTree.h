#ifndef LINK_CUT_TREE_H
#define LINK_CUT_TREE_H

#include <iostream>
#include <map>
#include <vector>
#include "Node.h"

template<typename T> class LinkCutTree {
public:
	LinkCutTree<T>();
	Node<T>* createTree(const T& key);
	Node<T>* operator[](const T& key);

	static void printSplayTree(const std::string& prefix, Node<T>* node,
		bool isLeft, std::map<Node<T>*, std::vector<Node<T>*>>* b = nullptr);

	static void printSplayTree(Node<T>* node, std::map<Node<T>*, std::vector<Node<T>*>>* b = nullptr);

	static void printLCT(Node<T>* node, std::map<Node<T>*, std::vector<Node<T>*>>* b = nullptr);

	static int printReprTree(Node<T>* node, std::map<Node<T>*, std::vector<Node<T>*>>* b = nullptr,
		bool t = true, int depth = 0);

private:
	std::map<T, Node<T>> nodes;
};

template<typename T> LinkCutTree<T>::LinkCutTree() : nodes(std::map<T, Node<T>>()) {}

template<typename T> Node<T>* LinkCutTree<T>::createTree(const T& key)
{
	if (!this->nodes.count(key)) {
		nodes.insert(std::make_pair(key, Node<T>(key)));
	}
	return &(this->nodes[key]);
}

template<typename T> Node<T>* LinkCutTree<T>::operator[](const T& key)
{
	if (this->nodes.count(key)) {
		return &(this->nodes[key]);
	}
	else {
		return nullptr;
	}
}

template<typename T> void LinkCutTree<T>::printSplayTree(const std::string& prefix, Node<T>* node,
	bool isLeft, std::map<Node<T>*, std::vector<Node<T>*>>* b)
{
	std::cout << prefix << (isLeft ? "|--" : "|--") << node->key << (node->isRoot ? "r" : "");
	if (node->parent) {
		std::cout << "(" << node->parent->key << ")";
	}
	std::cout << std::endl;

	if (node->right) {
		printSplayTree(prefix + (isLeft ? "|    " : "     "), node->right, false, b);
	}

	if (b && b->count(node)) {
		for (int i = 0; i < b->at(node).size(); i++) {
			printSplayTree(prefix + "~    ", b->at(node).at(i), true, b);
		}
	}
	if (node->left) {
		printSplayTree(prefix + (isLeft ? "|    " : "     "), node->left, true, b);
	}
}

template<typename T> void LinkCutTree<T>::printSplayTree(Node<T>* node,
	std::map<Node<T>*, std::vector<Node<T>*>>* b)
{
	while (!node->isRoot) {
		node = node->parent;
	}
	printSplayTree("", node, false, b);
}

template<typename T> void LinkCutTree<T>::printLCT(Node<T>* node,
	std::map<Node<T>*, std::vector<Node<T>*>>* b)
{
	while (node->parent) {
		node = node->parent;
	}
	printSplayTree("", node, false, b);
}

template<typename T> int LinkCutTree<T>::printReprTree(Node<T>* node,
	std::map<Node<T>*, std::vector<Node<T>*>>* b, bool t, int depth)
{
	while (t && node->parent) {
		node = node->parent;
	}
	if (node->left) {
		depth = printReprTree(node->left, b, false, depth);
	}
	std::cout << std::string(depth * 4L, ' ') << node->key << std::endl;
	depth++;
	if (b && b->count(node)) {
		for (int i = 0; i < b->at(node).size(); i++) {
			printReprTree(b->at(node).at(i), b, false, depth);
		}
	}
	if (node->right) {
		depth = printReprTree(node->right, b, false, depth);
	}
	return depth;
}

#endif

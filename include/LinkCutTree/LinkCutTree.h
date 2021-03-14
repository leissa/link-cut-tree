#ifndef LINK_CUT_TREE_H
#define LINK_CUT_TREE_H

#include <iostream>
#include <map>
#include <vector>
#include "Node.h"

template<typename T> class LinkCutTree {
public:
	LinkCutTree<T>();
	Node<T>* createTree(const T& aKey, int id = Node<T>::idCounter++);
	Node<T>* operator[](const T& aKey);

	static void printSplayTree(Node<T>* aNode,
		std::map<Node<T>*, std::vector<Node<T>*>>* aBackpointers = nullptr);

	static void printLCT(Node<T>* aNode, std::map<Node<T>*, std::vector<Node<T>*>>* aBackpointers = nullptr);

	static int printReprTree(Node<T>* aNode, std::map<Node<T>*, std::vector<Node<T>*>>* aBackpointers = nullptr,
		bool aDiscoverParent = true, int aDepth = 0);

private:
	std::map<T, Node<T>> _nodes;

	static void printSplayTreeRecursive(const std::string& aPrefix, Node<T>* aNode,
		bool aLeft, std::map<Node<T>*, std::vector<Node<T>*>>* aBackpointers = nullptr);
};

template<typename T> LinkCutTree<T>::LinkCutTree() : _nodes(std::map<T, Node<T>>()) {}

template<typename T> Node<T>* LinkCutTree<T>::createTree(const T& aKey, int aID)
{
	if (!this->_nodes.count(aKey)) {
		_nodes.insert(std::make_pair(aKey, Node<T>(aKey, aID)));
	}
	return &(this->_nodes[aKey]);
}

template<typename T> Node<T>* LinkCutTree<T>::operator[](const T& aKey)
{
	if (this->_nodes.count(aKey)) {
		return &(this->_nodes[aKey]);
	}
	else {
		return nullptr;
	}
}

template<typename T> void LinkCutTree<T>::printSplayTreeRecursive(const std::string& aPrefix, Node<T>* aNode,
	bool aLeft, std::map<Node<T>*, std::vector<Node<T>*>>* aBackpointers)
{
	std::cout << aPrefix << (aLeft ? "|--" : "|--") << aNode->getID() << (aNode->isRoot() ? "r" : "");
	if (aNode->parent()) {
		std::cout << "(" << aNode->parent()->getID() << ")";
	}
	std::cout << std::endl;

	if (aNode->right()) {
		printSplayTreeRecursive(aPrefix + (aLeft ? "|    " : "     "), aNode->right(), false, aBackpointers);
	}
	if (aBackpointers && aBackpointers->count(aNode)) {
		for (int i = 0; i < aBackpointers->at(aNode).size(); i++) {
			printSplayTreeRecursive(aPrefix + "~    ", aBackpointers->at(aNode).at(i), true, aBackpointers);
		}
	}
	if (aNode->left()) {
		printSplayTreeRecursive(aPrefix + (aLeft ? "|    " : "     "), aNode->left(), true, aBackpointers);
	}
}

template<typename T> void LinkCutTree<T>::printSplayTree(Node<T>* aNode,
	std::map<Node<T>*, std::vector<Node<T>*>>* aBackpointers)
{
	while (!aNode->_isRoot) {
		aNode = aNode->parent();
	}
	printSplayTreeRecursive("", aNode, false, aBackpointers);
}

template<typename T> void LinkCutTree<T>::printLCT(Node<T>* aNode,
	std::map<Node<T>*, std::vector<Node<T>*>>* aBackpointers)
{
	while (aNode->parent()) {
		aNode = aNode->parent();
	}
	printSplayTreeRecursive("", aNode, false, aBackpointers);
}

template<typename T> int LinkCutTree<T>::printReprTree(Node<T>* aNode,
	std::map<Node<T>*, std::vector<Node<T>*>>* aBackpointers, bool aDiscoverParent, int aDepth)
{
	while (aDiscoverParent && aNode->parent()) {
		aNode = aNode->parent();
	}
	if (aNode->left()) {
		aDepth = printReprTree(aNode->left(), aBackpointers, false, aDepth);
	}
	std::cout << std::string(aDepth * 4L, ' ') << aNode->getID() << std::endl;
	aDepth++;
	if (aBackpointers && aBackpointers->count(aNode)) {
		for (int i = 0; i < aBackpointers->at(aNode).size(); i++) {
			printReprTree(aBackpointers->at(aNode).at(i), aBackpointers, false, aDepth);
		}
	}
	if (aNode->right()) {
		aDepth = printReprTree(aNode->right(), aBackpointers, false, aDepth);
	}
	return aDepth;
}

#endif

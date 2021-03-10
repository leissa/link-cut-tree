#ifndef LINK_CUT_TREE_H
#define LINK_CUT_TREE_H

#include <iostream>
#include <map>
#include <vector>
#include "Node.h"

template<typename T> class LinkCutTree {
private:
	std::map<T, Node<T>> nodes;
public:
	LinkCutTree<T>() : nodes(std::map<T, Node<T>>()) {}

	Node<T>* createTree(const T& key) {
		if (!this->nodes.count(key)) {
			nodes.insert(std::make_pair(key, Node<T>(key)));
		}
		return &(this->nodes[key]);
	}

	Node<T>* operator[](const T& key) {
		if (this->nodes.count(key)) {
			return &(this->nodes[key]);
		}
		else {
			return nullptr;
		}
	}

	static void printSplayTree(const std::string& prefix, Node<T>* node,
		bool isLeft, std::map<Node<T>*, std::vector<Node<T>*>>* b = nullptr)
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

	static void printSplayTree(Node<T>* node, std::map<Node<T>*, std::vector<Node<T>*>>* b = nullptr) {
		while (!node->isRoot) {
			node = node->parent;
		}
		printSplayTree("", node, false, b);
	}

	static void printLCT(Node<T>* node, std::map<Node<T>*, std::vector<Node<T>*>>* b = nullptr) {
		while (node->parent) {
			node = node->parent;
		}
		printSplayTree("", node, false, b);
	}

	static int printReprTree(Node<T>* node, std::map<Node<T>*, std::vector<Node<T>*>>* b = nullptr,
		bool t = true, int depth = 0)
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
};

#endif

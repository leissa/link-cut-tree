#include "LinkCutTree.h"
#include <iostream>
#include <string>
#include "Utils.h"
#include "Node.h"

int main()
{
	LinkCutTree<int> l;
	std::vector<Node<int>*> v;
	std::map<Node<int>*, std::vector<Node<int>*>> backpointers;
	for (int i = 0; i < 100; i++) {
		v.push_back(l.createTree(i));
	}
	int x, y;
	while (true) {
		std::string input;
		std::cin >> input;
		if (std::cin.fail()) {
			exit(0);
		}
		if (input.compare("repr") == 0) {
			std::cin >> x;
			updateBackpointers(v, backpointers);
			l.printReprTree(l[x], &backpointers);
		}
		else if (input.compare("lct") == 0) {
			std::cin >> x;
			updateBackpointers(v, backpointers);
			l.printLCT(l[x], &backpointers);
		}
		else if (input.compare("link") == 0) {
			std::cin >> x;
			std::cin >> y;
			l[x]->link(l[y]);
		}
		else if (input.compare("ll") == 0) {
			std::cin >> x;
			std::cin >> y;
			l[x]->link(l[y], true, true);
		}
		else if (input.compare("lr") == 0) {
			std::cin >> x;
			std::cin >> y;
			l[x]->link(l[y], true, false);
		}
		else if (input.compare("cut") == 0) {
			std::cin >> x;
			l[x]->cut();
		}
		else if (input.compare("expose") == 0) {
			std::cin >> x;
			l[x]->expose();
		}
		else if (input.compare("root") == 0) {
			std::cin >> x;
			std::cout << l[x]->findRoot()->key << std::endl;
		}
		else if (input.compare("lca") == 0) {
			std::cin >> x;
			std::cin >> y;
			auto lca = l[x]->lowestCommonAncestor(l[y]);
			std::cout << (lca ? lca->key : -1) << std::endl;
		}
		else if (input.compare("info") == 0) {
			std::cin >> x;
			for (int i = 0; i < 4; i++) {
				std::cout << l[x]->getFlag(static_cast<Node<int>::flagType>(3-i));
			}
			std::cout << std::endl;
		}
	}
}

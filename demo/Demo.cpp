#include "LinkCutTree.h"
#include <iostream>
#include <string>
#include "Utils.h"

int main()
{
	using Node = LinkCutTree<int>::Node;
	LinkCutTree<int> l;
	std::vector<Node*> v;
	std::map<Node*, std::vector<Node*>> backpointers;
	for (int i = 0; i < 100; i++) {
		v.push_back(l.createManagedTree(i));
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
			l.link(l[x], l[y]);
		}
		else if (input.compare("cut") == 0) {
			std::cin >> x;
			l.cut(l[x]);
		}
		else if (input.compare("access") == 0) {
			std::cin >> x;
			l.access(l[x]);
		}
		else if (input.compare("root") == 0) {
			std::cin >> x;
			std::cout << l.findRoot(l[x])->key << std::endl;
		}
	}
}

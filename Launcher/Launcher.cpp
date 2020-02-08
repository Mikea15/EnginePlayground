
#include "Launcher.h"
#include "Game.h"
#include "Systems/BST.h"
int main(int argc, char* argv[])
{
	// StubState1 stubState;
	// Game game(&stubState);
	// return game.Execute();

	//       50
	//       / \
	//     30  70
	//    / \  / \
	//   20 40 60 80
	container::BST<int> tree(50);
	tree.insert(30);
	tree.insert(20);
	tree.insert(40);
	tree.insert(70);
	tree.insert(60);
	tree.insert(80);

	std::cout << "Inorder traversal of the given tree \n";
	tree.traverseInOrder();

	std::cout << "\n";
	tree.erase(50);

	tree.traverseBreadthFirst();

	getchar();

	return 0;
}
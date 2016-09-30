#pragma once
#include "cocos2d.h"
#include "GameState.h"

class GameNode
{
public:
	//CONSTRUCTORS / DESTRUCTORS
	GameNode(GameState* gameState, bool larvaTurn, int levelInTree);
	GameNode();
	~GameNode();

	//METHODS USED FOR BUILDING TREE
	void buildGameTree(GameNode* head, int& depth);
	void addAllPossibleChildren();

	//GETTERS
	GameState* getGameState();
	int getHeuristicValue();
	static std::vector<GameNode*> getNodesVector();
	static std::vector<int> getBottomLevelChildrenIndexes();
	int getNodesVectorIndex();
	int getParentIndex();
	std::vector<int>* getChildrenIndexes();
	int getTreeLevel();
	bool isLarvaTurn();

	//SETTERS
	void setHeuristicValue(int& value);
	void setIsLarvaTurn(bool larvaTurn);
	void setParentIndex(int& parentIndex);
	void setNodesVectorIndex(int& size_);
	void setTreeLevel(int treeLevel);

	//STATIC
	static void reset(); // ---> clears the nodesVector so we can build a new tree
	static std::vector<GameNode*> nodesVector;

	void calculateHeuristic();
	bool larvaWon(int position);

private:
	//Every GameNode has a gamestate which holds data on the larva and birds positions
	GameState* gameState; 

	//Every GameNode holds the nodesVector indexes of its children
	std::vector<int>* childrenIndexes;

	//The nodesVector index of the GameNode's parent
	int parentIndex;

	//default heuristic value is -1
	int heuristicValue;

	//whether its the larva's (MAX) turn or not at a given node
	bool larvaTurn;

	//The nodeVector indexes of the bottom level nodes in the tree
	static std::vector<int> bottomLevelChildrenIndexes;

	//The node's index in the nodeVector
	int nodesVectorIndex;

	//The node's level in the tree --> 0 is top
	int levelInTree;
};

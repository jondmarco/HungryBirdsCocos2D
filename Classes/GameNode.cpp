#pragma once
#include "GameNode.h"
#include <iostream>
#include <fstream>

std::vector<GameNode*> GameNode::nodesVector;
std::vector<int> GameNode::bottomLevelChildrenIndexes; 

GameNode::GameNode(GameState* gameState, bool larvaTurn,int levelInTree)
	: heuristicValue(-1)
{
	this->gameState = gameState;
	this->larvaTurn = larvaTurn;
	int size_ = GameNode::nodesVector.size();
	setNodesVectorIndex(size_);
	childrenIndexes = new std::vector<int>;
	this->levelInTree = levelInTree;
}

GameNode::GameNode()
{
}

GameNode::~GameNode()
{

}

void GameNode::setNodesVectorIndex(int& size_)
{
	nodesVectorIndex = size_;
}

GameState* GameNode::getGameState()
{
	return gameState;
}

void GameNode::addAllPossibleChildren()
{
	int c_index; //nodesVector index of the nodes child
	//its the larva's turn to play at this node
	if (larvaTurn)
	{
		bool _larvaTurn = false; //for the nodes children, the turn is changed
		std::vector<int> temp;
		temp = this->gameState->getPossibleMoves(LARVA); //the possible moves the larva can make

		//add children for larva moves
		int i = 0;
		for (std::vector<int>::iterator it = temp.begin(); it < temp.end(); it++, i++)
		{
			//for each possible move the larva can make, create a node and add it to the nodesVector
			GameNode::nodesVector.push_back(new GameNode(new GameState(temp.at(i), gameState->getBirdPosition(1), gameState->getBirdPosition(2),
				gameState->getBirdPosition(3), gameState->getBirdPosition(4)), _larvaTurn, (levelInTree + 1)));
			
			//the current node adds the newly created node's index into its childrenIndexes vector
			c_index = GameNode::nodesVector.size() - 1;
			childrenIndexes->push_back(c_index);

			//The newly created node has its parent index set to the current node's index
			int index = getNodesVectorIndex();
			GameNode::nodesVector.at(GameNode::nodesVector.size() - 1)->setParentIndex(index);
		}
	}
	//its the birds turn to play at this node
	else
	{
		bool _larvaTurn = true; //for the nodes children, the turn is changed
		std::vector<int> temp(this->gameState->getPossibleMoves(BIRD1)); //The possible moves Bird1 can make

		//add children for bird 1 moves
		int i = 0;
		if (temp.size() != 0)
		{
			for (std::vector<int>::iterator it = temp.begin(); it < temp.end(); it++, i++)
			{
				//for each possible move the bird1 can make, create a node and add it to the nodesVector
				GameNode::nodesVector.push_back(new GameNode(new GameState(gameState->getLarvaPosition(), temp.at(i), gameState->getBirdPosition(2),
					gameState->getBirdPosition(3), gameState->getBirdPosition(4)), _larvaTurn, (levelInTree + 1)));

				//the current node adds the newly created node's index into its childrenIndexes vector
				c_index = GameNode::nodesVector.size() - 1;
				childrenIndexes->push_back(c_index);

				//The newly created node has its parent index set to the current node's index
				int index = getNodesVectorIndex();
				GameNode::nodesVector.at(GameNode::nodesVector.size() - 1)->setParentIndex(index);
			}
		}
	
		
		std::vector<int> temp2(this->gameState->getPossibleMoves(BIRD2));

		//add children for bird 2 moves
		i = 0;
		if (temp2.size() != 0)
		{
			for (std::vector<int>::iterator it = temp2.begin(); it < temp2.end(); it++, i++)
			{
				//for each possible move the bird2 can make, create a node and add it to the nodesVector
				GameNode::nodesVector.push_back(new GameNode(new GameState(gameState->getLarvaPosition(), gameState->getBirdPosition(1), temp2.at(i),
					gameState->getBirdPosition(3), gameState->getBirdPosition(4)), _larvaTurn, (levelInTree + 1)));

				//the current node adds the newly created node's index into its childrenIndexes vector
				c_index = GameNode::nodesVector.size() - 1;
				childrenIndexes->push_back(c_index);

				//The newly created node has its parent index set to the current node's index
				int index = getNodesVectorIndex();
				GameNode::nodesVector.at(GameNode::nodesVector.size() - 1)->setParentIndex(index);
			}
		}
		

		std::vector<int> temp3(this->gameState->getPossibleMoves(BIRD3));

		//add children for bird 3 moves
		i = 0;
		if (temp3.size() != 0)
		{
			for (std::vector<int>::iterator it = temp3.begin(); it < temp3.end(); it++, i++)
			{
				//for each possible move the bird3 can make, create a node and add it to the nodesVector
				GameNode::nodesVector.push_back(new GameNode(new GameState(gameState->getLarvaPosition(), gameState->getBirdPosition(1), gameState->getBirdPosition(2),
					temp3.at(i), gameState->getBirdPosition(4)), _larvaTurn, (levelInTree + 1)));

				//the current node adds the newly created node's index into its childrenIndexes vector
				c_index = GameNode::nodesVector.size() - 1;
				childrenIndexes->push_back(c_index);

				//The newly created node has its parent index set to the current node's index
				int index = getNodesVectorIndex();
				GameNode::nodesVector.at(GameNode::nodesVector.size() - 1)->setParentIndex(index);
			}
		}

		std::vector<int> temp4(this->gameState->getPossibleMoves(BIRD4));

		//add children for bird 4 moves
		i = 0;
		if (temp4.size() != 0)
		{
			for (std::vector<int>::iterator it = temp4.begin(); it < temp4.end(); it++, i++)
			{
				//for each possible move the bird4 can make, create a node and add it to the nodesVector
				GameNode::nodesVector.push_back(new GameNode(new GameState(gameState->getLarvaPosition(), gameState->getBirdPosition(1), gameState->getBirdPosition(2),
					gameState->getBirdPosition(3), temp4.at(i)), _larvaTurn, (levelInTree + 1)));

				//the current node adds the newly created node's index into its childrenIndexes vector
				c_index = GameNode::nodesVector.size() - 1;
				childrenIndexes->push_back(c_index);

				//The newly created node has its parent index set to the current node's index
				int index = getNodesVectorIndex();
				GameNode::nodesVector.at(GameNode::nodesVector.size() - 1)->setParentIndex(index);
			}
		}
		
	}

}

std::vector<int>* GameNode::getChildrenIndexes()
{
	return childrenIndexes;
}

int GameNode::getParentIndex()
{
	return parentIndex;
}

void GameNode::setParentIndex(int& parentIndex)
{
	this->parentIndex = parentIndex;
}

//build the game tree starting from a head
//we recursively call this method to build a subtree for each of the heads children at 1 depth lower
void GameNode::buildGameTree(GameNode* head, int& depth)
{

	//log the current node and its level
	cocos2d::Value treeLevel(head->getTreeLevel());
	cocos2d::Value larvaPosition(head->getGameState()->getLarvaPosition());
	cocos2d::Value bird1Pos(head->getGameState()->getBirdPosition(1));
	cocos2d::Value bird2Pos(head->getGameState()->getBirdPosition(2));
	cocos2d::Value bird3Pos(head->getGameState()->getBirdPosition(3));
	cocos2d::Value bird4Pos(head->getGameState()->getBirdPosition(4));

	cocos2d::log("Level %s: Larva: %s Bird1: %s Bird2: %s Bird3: %s Bird4: %s", treeLevel.getDescription().c_str(), larvaPosition.getDescription().c_str(),
		bird1Pos.getDescription().c_str(), bird2Pos.getDescription().c_str(), bird3Pos.getDescription().c_str(), bird4Pos.getDescription().c_str());

	//were building a new tree
	if (nodesVector.size() == 0) {	
		nodesVector.push_back(head);
	}
	
	//base case: we reached the maximum depth --> we dont explore children for this node
	if (levelInTree - depth == 0)
	{
		bottomLevelChildrenIndexes.push_back(head->getNodesVectorIndex());
		return;
	}
	
	//add all possible children gamestates to nodesVector
	//each nodes children indexes are stored in the vector childrenIndexes
	head->addAllPossibleChildren();

	int i = 0;
	for (std::vector<int>::iterator it = head->getChildrenIndexes()->begin(); it < head->getChildrenIndexes()->end(); it++)
	{
		//build a subtree for each child recursively at a lower depth
		int newDepth = depth - 1;
		buildGameTree(nodesVector.at(head->getChildrenIndexes()->at(i)), newDepth);
		i++;
	}
}

//returns the heuristic value of the node
int GameNode::getHeuristicValue()
{
	//if default value --> calculate it
	if (heuristicValue == -1)
	{
		calculateHeuristic();
	}

	return heuristicValue;
}

void GameNode::setHeuristicValue(int& value)
{
	this->heuristicValue = value;
}

std::vector<GameNode*> GameNode::getNodesVector()
{
	return GameNode::nodesVector;
}

//get the nodesVector indexes of the nodes that are at the lowest level
std::vector<int> GameNode::getBottomLevelChildrenIndexes()
{
	return GameNode::bottomLevelChildrenIndexes;
}

int GameNode::getNodesVectorIndex()
{
	return nodesVectorIndex;
}

//reset the game node class so we can build a new tree
void GameNode::reset()
{
	nodesVector.clear();
	bottomLevelChildrenIndexes.clear();
}

//returns whether or not its the larva's (MAX) turn or not at a node
bool GameNode::isLarvaTurn()
{
	return larvaTurn;
}

void GameNode::setIsLarvaTurn(bool larvaTurn)
{
	this->larvaTurn = larvaTurn;
}

//nodes level in tree --> top is 0
int GameNode::getTreeLevel()
{
	return levelInTree;
}

void GameNode::setTreeLevel(int treeLevel)
{
	this->levelInTree = treeLevel;
}

void GameNode::calculateHeuristic() 
{
	std::vector<int> possibleMovesLarva = gameState->getPossibleMoves(LARVA);
	std::vector<int> maxValuesForPosition;
	maxValuesForPosition.reserve(possibleMovesLarva.size());
	std::vector<int> birdsPosition = gameState->getBirdsPositions();
	int larvaPos = gameState->getLarvaPosition();
	int heuristicValue_ = 0;
	for (int i = 0; i < possibleMovesLarva.size(); i++)
	{
		int minVal = -1000;
		for (int k = 0; k < birdsPosition.size(); k++)
		{
			if (possibleMovesLarva[i] - birdsPosition[k] > minVal)
			{
				minVal = possibleMovesLarva[i] - birdsPosition[k];
			}
		}
		maxValuesForPosition.push_back(minVal);
	}
	
	for (int i = 0; i < maxValuesForPosition.size(); i++)
	{
		heuristicValue_ += maxValuesForPosition[i];
	}

	int returnVal = (heuristicValue_ + larvaPos);
	setHeuristicValue(returnVal);
}

bool GameNode::larvaWon(int position)
{
	return (position >= 56 && position <= 63);
}

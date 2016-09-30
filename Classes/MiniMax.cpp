#pragma once
#include "MiniMax.h"

GameState* MiniMax::findBestPossibleMove(GameState* currentGameState, int& depth, bool turn)
{
	cocos2d::log("Starting to build tree");
	GameNode* head = new GameNode(currentGameState, turn, 0);
	head->buildGameTree(head, depth);
	std::vector<int> temp = GameNode::getBottomLevelChildrenIndexes();
	return execute(temp, GameNode::nodesVector.at(temp.at(0))->isLarvaTurn());
}

//to execute minimax, we start at the bottom level of the tree
//we then compare the values of the nodes that are siblings
//we then take the max or min for each group of sibling 
//we then recursively call this function with the parent level moving the heuristic values up at each level
GameState* MiniMax::execute(std::vector<int> currentLevelIndexes, bool turn)
{
	bool max = turn; //if were maximizing or not
	bool reachedHead = false;
	std::vector<int> nextLevelIndexes;
	int siblingGroup = 1;

	for (int i = 0; i < currentLevelIndexes.size(); i++)
	{
		int start = i;
		//as long as subsequent nodes have the same parent --> were looking for siblings
		if (currentLevelIndexes.size() > 1 && i != currentLevelIndexes.size() - 1)
		{
			//whatever the value of i is after this loop, we will know that nodes from [start] to [i] are siblings
			while (GameNode::nodesVector.at(currentLevelIndexes.at(i))->getParentIndex()
				== GameNode::nodesVector.at(currentLevelIndexes.at(i + 1))->getParentIndex())
			{
				i++;
				if (i == currentLevelIndexes.size() - 1)
				{
					break;
				}
			}
		}

		//the heuristic value that we will set for the parent of the nodes in the range [start, i]
		int value = GameNode::getNodesVector().at(currentLevelIndexes.at(i))->getHeuristicValue();

		cocos2d::Value treeLevel(GameNode::getNodesVector().at(currentLevelIndexes.at(start))->getTreeLevel());
		cocos2d::Value sGroup(siblingGroup);

		cocos2d::log("Level: %s Sibling group: %s", treeLevel.getDescription().c_str(), sGroup.getDescription().c_str());
		siblingGroup++;

		//for each node that shares the same parent --> all the nodes between [start, i], maximize or minimize
		for (int k = start; k <= i; k++)
		{
			cocos2d::Value hValue(GameNode::getNodesVector().at(currentLevelIndexes.at(k))->getHeuristicValue());
			cocos2d::log("Heuristic value: %s", hValue.getDescription().c_str());
			//were maximizing
			//if the node were at has a gamesate where its the birds turn
			//this means we need to bring the max to the level above
			max = !GameNode::getNodesVector().at(currentLevelIndexes.at(k))->isLarvaTurn();
			if (max)
			{
				if (GameNode::getNodesVector().at(currentLevelIndexes.at(k))->getHeuristicValue() > value)
				{
					value = GameNode::getNodesVector().at(currentLevelIndexes.at(k))->getHeuristicValue();
				}
			}
			//were minimizing
			else
			{
				if (GameNode::getNodesVector().at(currentLevelIndexes.at(k))->getHeuristicValue() < value)
				{
					value = GameNode::getNodesVector().at(currentLevelIndexes.at(k))->getHeuristicValue();
				}
			}
		}

		//set the parents heuristic value
		int index = GameNode::getNodesVector().at(currentLevelIndexes.at(start))->getParentIndex();
		if (index >= 0)
		{
			cocos2d::Value v(value);
			cocos2d::Value maximizing(max);
			cocos2d::log("Maximizing: %s Value brought up to level above: %s", maximizing.getDescription().c_str(), v.getDescription().c_str());
	
			GameNode::getNodesVector().at(index)->setHeuristicValue(value);


		}

		int nextLevelIndex = GameNode::getNodesVector().at(currentLevelIndexes.at(start))->getParentIndex();

		//reached head
		if (nextLevelIndex == 0)
		{
			reachedHead = true;
		}

		//add the parent to the next level vector
		//we recursively execute this function for each level bottom up until we reach the head
		nextLevelIndexes.push_back(nextLevelIndex);
	}

	//reached head
	if (reachedHead)
	{
		/*
		if (max)
		{
			max = false; //previously level was max so the head has to minimize
		}
		else
		{
			max = true; //previously level was min so the head has to maximize
		}
		*/

		std::vector<int> childrenIndexes = *(GameNode::getNodesVector().at(0)->getChildrenIndexes());
		int value = GameNode::getNodesVector().at(childrenIndexes.at(0))->getHeuristicValue();
		GameState* returnState = GameNode::getNodesVector().at(childrenIndexes.at(0))->getGameState();

		int i = 0;
		//iterate through the heads children to find the gamestate to play
		for (std::vector<int>::iterator it = childrenIndexes.begin(); it < childrenIndexes.end(); it++)
		{
			//were maximizing
			if (max) {
				//we chose the child that has the largest heuristic value
				if (GameNode::getNodesVector().at(childrenIndexes.at(i))->getHeuristicValue() > value)
				{
					value = GameNode::getNodesVector().at(childrenIndexes.at(i))->getHeuristicValue();
					returnState = GameNode::getNodesVector().at(childrenIndexes.at(i))->getGameState();
					GameNode::nodesVector.at(0)->setHeuristicValue(value);
				}
			}
			//were minimizing
			else
			{
				//we chose the child that has the smallest heuristic value
				if (GameNode::getNodesVector().at(childrenIndexes.at(i))->getHeuristicValue() < value)
				{
					value = GameNode::getNodesVector().at(childrenIndexes.at(i))->getHeuristicValue();
					returnState = GameNode::getNodesVector().at(childrenIndexes.at(i))->getGameState();
					GameNode::nodesVector.at(0)->setHeuristicValue(value);
				}
			}

			i++;
		}
		cocos2d::Value hValue(GameNode::nodesVector.at(0)->getHeuristicValue());
		cocos2d::log("Level: 0");
		cocos2d::log("Heuristic value: %s", hValue.getDescription().c_str());

		//return the new state of the game
		return returnState;
	}

	//change max to min and vice versa for the next level
	/*
	if (max)
	{
		max = false;
	}
	else
	{
		max = true;
	}
	*/

	//recursively evaluate for each tree level bottom to top until we reach the head
	return execute(nextLevelIndexes, max);
}

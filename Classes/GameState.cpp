#pragma once
#include "GameState.h"

GameState::GameState(int larvaPosition, int b1Position, int b2Position, int b3Position, int b4Position)
{
	gameStatePositions.push_back(larvaPosition);
	gameStatePositions.push_back(b1Position);
	gameStatePositions.push_back(b2Position);
	gameStatePositions.push_back(b3Position);
	gameStatePositions.push_back(b4Position);
}

GameState::GameState()
{

}

GameState::~GameState()
{

}

std::vector<int> GameState::getGameStatePositions()
{
	return gameStatePositions;
}

int GameState::getLarvaPosition()
{
	return gameStatePositions.at(0);
}

int GameState::getBirdPosition(int birdNumber)
{
	return gameStatePositions.at(birdNumber);
}

std::vector<int> GameState::getBirdsPositions()
{
	std::vector<int> returnVector;
	returnVector.push_back(gameStatePositions.at(1));
	returnVector.push_back(gameStatePositions.at(2));
	returnVector.push_back(gameStatePositions.at(3));
	returnVector.push_back(gameStatePositions.at(4));
	return returnVector;
}

std::vector<int> GameState::getPossibleMoves(PlayerType playerType)
{
	switch (playerType)
	{
	case LARVA:
		return getLarvaPossibleMoves();
		break;
	case BIRD1:
		return getBirdPossibleMoves(1);
		break;
	case BIRD2:
		return getBirdPossibleMoves(2);
		break;
	case BIRD3:
		return getBirdPossibleMoves(3);
		break;
	case BIRD4:
		return getBirdPossibleMoves(4);
		break;
	}
}

std::vector<int> GameState::getLarvaPossibleMoves()
{
	std::vector<int> positionsOfCellsInRange;
	int position = getLarvaPosition();

	int topLeft = position - 9;
	int bottomLeft = position + 7;
	int topRight = position - 7;
	int bottomRight = position + 9;

 	if (!atFirstColumn(position))
	{
		if ((topLeft >= 0) && (topLeft < 64) && (!isOccupied(topLeft)))
		{
			positionsOfCellsInRange.push_back(topLeft);
		}

		if ((bottomLeft >= 0) && (bottomLeft < 64) && (!isOccupied(bottomLeft)))
		{
			positionsOfCellsInRange.push_back(bottomLeft);
		}
	}

	if (!atLastColumn(position))
	{
		if ((topRight >= 0) && (topRight < 64) && (!isOccupied(topRight)))
		{
			positionsOfCellsInRange.push_back(topRight);
		}

		if ((bottomRight >= 0) && (bottomRight < 64) && (!isOccupied(bottomRight)))
		{
			positionsOfCellsInRange.push_back(bottomRight);
		}
	}
	return positionsOfCellsInRange;
}

std::vector<int> GameState::getBirdPossibleMoves(int birdNumber)
{
	std::vector<int> positionsOfCellsInRange;
	int position = getBirdPosition(birdNumber);

	//at last row; position 0 to 7
	if (position >= 0 && position <= 7)
	{
		return positionsOfCellsInRange;
	}

	int topLeft = position - 9;
	int topRight = position - 7;

	if (!atFirstColumn(position) && (topLeft >= 0) && (topLeft < 64) && (!isOccupied(topLeft)))
	{
		positionsOfCellsInRange.push_back(topLeft);
	}

	if (!atLastColumn(position) && (topRight >= 0) && (topRight < 64) && (!isOccupied(topRight)))
	{
		positionsOfCellsInRange.push_back(topRight);
	}

	return positionsOfCellsInRange;
}

bool GameState::atFirstColumn(int& position)
{
	return (position % 8 == 0);
}

bool GameState::atLastColumn(int& position)
{
	return (position % 8 == 7);
}

bool GameState::larvaWon()
{
	int birdsNotAvailable = 0;
	for (int i = 1; i < 5; ++i)
	{
		//at last row
		if (gameStatePositions.at(i) >= 0 && gameStatePositions.at(i) <= 7)
		{
			birdsNotAvailable++;
		}
	}

	if (getPossibleMoves(BIRD1).size() == 0)
	{
		birdsNotAvailable++;
	}

	if (getPossibleMoves(BIRD2).size() == 0)
	{
		birdsNotAvailable++;
	}

	if (getPossibleMoves(BIRD3).size() == 0)
	{
		birdsNotAvailable++;
	}

	if (getPossibleMoves(BIRD4).size() == 0)
	{
		birdsNotAvailable++;
	}

	return ((getLarvaPosition() >= 56 && getLarvaPosition() <= 63) || birdsNotAvailable == 4);
}

bool GameState::birdsWon()
{
	return (getPossibleMoves(LARVA).size() == 0);
}

bool GameState::isOccupied(int& position) {

	for (int i = 0; i < 5; i++)
	{
		if (gameStatePositions.at(i) == position)
		{
			return true;
		}
	}
	return false;
}

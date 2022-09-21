#include "MineSweeper.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include <random>

MineSweeper::MineSweeper()
{
    playing = false;
    bombsInitialized = false;
}

void MineSweeper::initLevel(int _size)
{
    size = _size;
    covered = std::vector<std::vector<bool>>(size);
    coveredCount = size * size;
    for (auto &&i : covered)
    {
        i.resize(size, true);
    }
    flagged = std::vector<std::vector<bool>>(size);
    flaggedCount = 0;
    for (auto &&j : flagged)
    {
        j.resize(size, false);
    }

    playing = true;
    bombsInitialized = false;
}

void MineSweeper::print()
{
    std::string text = "&";
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            switch (getField(i, j))
            {
            case Empty:
                text = "_";
                break;
            case Empty1:
                text = "1";
                break;
            case Empty2:
                text = "2";
                break;
            case Empty3:
                text = "3";
                break;
            case Empty4:
                text = "4";
                break;
            case Empty5:
                text = "5";
                break;
            case Empty6:
                text = "6";
                break;
            case Empty7:
                text = "7";
                break;
            case Empty8:
                text = "8";
                break;
            case BombExploded:
                text = "X";
                break;
            case BombUnexploded:
                text = "x";
                break;
            case Flag:
                text = "F";
                break;
            default:
            case Button:
                text = "&";
                break;
            }
            std::cout << text;
        }
        std::cout << "\n";
    }
}

FieldType MineSweeper::getField(int row, int column)
{
    if (playing && flagged[row][column])
        return Flag;
    if (!playing && isBomb(row, column))
    {
        if (!playing && row == deathX && column == deathY)
            return BombExploded;
        return BombUnexploded;
    }
    if (covered[row][column])
        return Button;
    switch (bombsNearby(row, column))
    {
    case 1:
        return Empty1;
    case 2:
        return Empty2;
    case 3:
        return Empty3;
    case 4:
        return Empty4;
    case 5:
        return Empty5;
    case 6:
        return Empty6;
    case 7:
        return Empty7;
    case 8:
        return Empty8;
    default:
    case 0:
        return Empty;
    }
}

bool MineSweeper::isRunning()
{
    return playing;
}

std::string MineSweeper::take(int row, int column)
{
    if (!inBounds(row) || !inBounds(column))
        return "Spot is outside range";
    if (!covered[row][column])
        return "Already uncovered, try again";
    if (flagged[row][column])
        return "Space already flagged";
    ensureBombs(row, column);
    if (bombs[row][column])
    {
        playing = false;
        deathX = row;
        deathY = column;
        return "You lost!";
    }
    takeRecursive(row, column);
    if (coveredCount == bombTotal)
    {
        playing = false;
        return "Wow, you win!";
    }
    return "Now covered: " + std::to_string(coveredCount) + ", bombs total: " + std::to_string(bombTotal);
}

void MineSweeper::takeNearby(int row, int column)
{
    if (flagged[row][column])
        return;
    if (covered[row][column])
        return;
    if (bombsNearby(row, column) != flagsNearby(row, column))
        return;
    for (int i = row - 1; i <= row + 1; i++)
    {
        for (int j = column - 1; j <= column + 1; j++)
        {
            if (!playing)
                return;
            if (!inBounds(i) || !inBounds(j))
                continue;
            if (i == row && j == column)
                continue;
            if (flagged[i][j])
                continue;
            take(i, j);
        }
    }
}

void MineSweeper::flag(int row, int column)
{
    if (!inBounds(row) || !inBounds(column))
        return;
    if (!covered[row][column])
        return;
    flagged[row][column] = !flagged[row][column];
    flaggedCount += (flagged[row][column]) ? 1 : -1;
}

void MineSweeper::takeRecursive(int row, int column)
{
    covered[row][column] = false;
    coveredCount--;
    if (bombsNearby(row, column) == 0)
    {
        for (int i = row - 1; i <= row + 1; i++)
        {
            for (int j = column - 1; j <= column + 1; j++)
            {
                if (!inBounds(i) || !inBounds(j))
                    continue;
                if (covered[i][j])
                    takeRecursive(i, j);
            }
        }
    }
}

int MineSweeper::numberOfBombs()
{
    return bombTotal;
}

int MineSweeper::numberOfHidden()
{
    return coveredCount;
}

int MineSweeper::numberOfFlagged()
{
    return flaggedCount;
}

int MineSweeper::bombsNearby(int row, int column)
{
    int count = 0;
    for (int i = row - 1; i < row + 2; i++)
    {
        for (int j = column - 1; j < column + 2; j++)
        {
            if (this->isBomb(i, j))
                count++;
        }
    }

    return count;
}
int MineSweeper::flagsNearby(int row, int column)
{
    int count = 0;
    for (int i = row - 1; i < row + 2; i++)
    {
        for (int j = column - 1; j < column + 2; j++)
        {
            if (!inBounds(i) || !inBounds(j))
                continue;
            if (!(i == row && j == column) && flagged[i][j])
                count++;
        }
    }

    return count;
}

bool MineSweeper::isBomb(int row, int column)
{
    if (!bombsInitialized || !inBounds(row) || !inBounds(column))
    {
        return false;
    }
    return bombs[row][column];
}

void MineSweeper::ensureBombs(int row, int column)
{
    if (bombsInitialized)
        return;

    bombs = std::vector<std::vector<bool>>(size);
    for (auto &&i : bombs)
    {
        i.resize(size, false);
    }

    bombTotal = (size * size / 5) + 1;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, size - 1);

    for (size_t i = 0; i < bombTotal; i++)
    {
        while (true)
        {
            int x = distr(gen);
            int y = distr(gen);
            if (row == x && column == y)
                continue;
            if (bombs[x][y])
                continue;
            bombs[x][y] = true;
            break;
        }
    }

    bombsInitialized = true;
}

bool MineSweeper::inBounds(int dimension)
{
    return dimension >= 0 && dimension < size;
}

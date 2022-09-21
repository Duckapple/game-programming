#ifndef MINESWEEPER_MINESWEEPER_HPP
#define MINESWEEPER_MINESWEEPER_HPP

#include <string>
#include <vector>
#include <memory>

enum FieldType
{
    Button,         // Field has not been uncovered
    Empty,          // Field has been uncovered but is empty - no bombs nearby
    Empty1,         // Field has been uncovered but is empty - 1 bombs nearby
    Empty2,         // Field has been uncovered but is empty - 2 bombs nearby
    Empty3,         // Field has been uncovered but is empty - 3 bombs nearby
    Empty4,         // Field has been uncovered but is empty - 4 bombs nearby
    Empty5,         // Field has been uncovered but is empty - 5 bombs nearby
    Empty6,         // Field has been uncovered but is empty - 6 bombs nearby
    Empty7,         // Field has been uncovered but is empty - 7 bombs nearby
    Empty8,         // Field has been uncovered but is empty - 8 bombs nearby
    BombExploded,   // Field has been uncovered and contains a exploded bomb
    BombUnexploded, // Field has been uncovered and contains a unexploded bomb (optionally used at end of game)
    Flag            // Field marked with flag
};

class MineSweeper
{
public:
    MineSweeper();

    void initLevel(int size); // setup level - randomly position bombs

    void print(); // print the level to the console

    /** Get the field value */
    FieldType getField(int row, int column);

    bool isRunning(); // Returns false if player has won or lost

    std::string take(int row, int column); // Uncovers a field
    // If row/column is outside game field, report error
    // If bombsNearby is 0 then take should be invoked on nearby fields

    /** If number of nearby flags equals number of bombs nearby, then reveal any non-flagged */
    void takeNearby(int row, int column);

    void flag(int row, int column);

    int numberOfBombs(); // Returns the number of bombs in the game

    int numberOfHidden(); // Returns the current number of hidden fields

    int numberOfFlagged();

private:
    /** Returns the number of bombs nearby (rows and column +/- 1) */
    int bombsNearby(int row, int column);

    int flagsNearby(int row, int column);

    /** Is this a bomb? Safe to use with arbitrary input */
    bool isBomb(int row, int column);

    /** Makes sure that bombs are initialized - used to delay generating bombs until player has picked a space */
    void ensureBombs(int row, int column);

    /** Recursively uncover neighbours if legal, 0 bombs nearby and not uncovered */
    void takeRecursive(int row, int column);

    /** Returns true if within bounds of 0 and size - 1, false otherwise */
    bool inBounds(int dimension);

    int size;
    bool bombsInitialized;
    int bombTotal;
    int coveredCount;
    int flaggedCount;
    /** Is the game in progress? */
    bool playing;
    int deathX, deathY;
    std::vector<std::vector<bool>> bombs;
    std::vector<std::vector<bool>> covered;
    std::vector<std::vector<bool>> flagged;
};
#endif
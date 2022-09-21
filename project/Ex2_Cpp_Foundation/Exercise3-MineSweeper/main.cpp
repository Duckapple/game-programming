#include <iostream>
#include <string>
#include "MineSweeper.hpp"

void ClearScreen()
{
    for (int i = 0; i < 7; i++)
    {
        std::cout << "\n";
    }
}

int main()
{
    using namespace std;
    int size = 0;
    int row;
    int column;

    while (size <= 1)
    {
        cout << "Choose game size: \n";
        std::cin >> size;
    }
    MineSweeper mineSweeper;
    mineSweeper.initLevel(size);

    string message = "";
    while (mineSweeper.isRunning())
    {
        ClearScreen();
        mineSweeper.print();
        cout << "\n"
             << message << endl;
        cout << "\nEnter row: ";
        std::cin >> row;
        cout << "\nEnter column: ";
        std::cin >> column;
        message = mineSweeper.take(row, column);
    }
    ClearScreen();
    mineSweeper.print();
    cout << "\n"
         << message << endl;
    return 0;
}
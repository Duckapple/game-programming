#include <iostream>
#include <ctime>

int main()
{
    using namespace std;

    cout << __DATE__ << " " << __TIME__ << "\n";
    auto t = time(nullptr);
    cout << ctime(&t) << "\n";

    int x; // waits for user input. Workaround for not closing window on Windows
    std::cin >> x;
    return 0;
}
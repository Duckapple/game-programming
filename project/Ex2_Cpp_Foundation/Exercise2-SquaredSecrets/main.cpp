#include <iostream>
#include <cmath>

int main()
{
    using namespace std;

    string input;
    cout << "Enter message to be encrypted:\n";
    std::cin >> input;

    // if (input.find(" ", 0, -1) >= 0)
    // {
    //     cout << "String cannot contain spaces!"
    //          << "\n";
    //     return 0;
    // }

    auto sideApprox = sqrt(input.length());
    auto side = round(sideApprox);
    if (sideApprox > side)
    {
        side += 1;
    }
    for (size_t i = 0; i < side; i++)
    {
        for (size_t j = 0; j < side; j++)
        {
            auto l = (j * side) + i;
            cout << (l >= input.length() ? ' ' : input.at(l));
        }
        cout << " ";
    }
    cout << "\n";

    // Examples:
    // input -> output
    // ifmanwasmeanttostayonthegroundgodwouldhavegivenusroots -> imtgdvs  fearwer  mayoogo  anouuio  ntnnlvt  wttddes  aohghn   sseoau
    // haveaniceday -> hae and via ecy
    // feedthedog -> fto ehg ee  dd
    // chillout -> clu hlt io

    int x; // waits for user input. Workaround for not closing window on Windows
    std::cin >> x;

    return 0;
}
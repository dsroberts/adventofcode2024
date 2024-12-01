#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <numeric>
#include <ranges>

int diff(int a, int b)
{
    return a > b ? a - b : b - a;
}

int main()
{
    std::ifstream f("input.txt");
    std::string line, tok;
    std::stringstream ss;
    std::vector<int> col1, col2;
    while (getline(f, line))
    {
        ss = std::stringstream(line);
        getline(ss, tok, ' ');
        col1.push_back(std::stoi(tok));
        getline(ss, tok, ' ');
        while (tok.size() == 0)
        {
            getline(ss, tok, ' ');
        }
        col2.push_back(std::stoi(tok));
    }
    f.close();

    std::sort(col1.begin(), col1.end());
    std::sort(col2.begin(), col2.end());
    std::cout << std::ranges::fold_left(std::views::transform(std::views::zip(col1, col2), [](std::tuple<int, int> a)
                                                              { return std::abs(std::get<0>(a) - std::get<1>(a)); }),
                                        0, std::plus<int>())
              << std::endl;
}

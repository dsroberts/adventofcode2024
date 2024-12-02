#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <algorithm>
#include <ranges>
#include <cstdint>
#include <functional>

bool monotonically_increasing(std::vector<int> in)
{
    std::set<int> s(in.begin(), in.end());
    std::vector<int> copied(s.begin(), s.end());
    std::ranges::sort(copied);
    return copied == in;
}

bool monotonically_decreasing(std::vector<int> in)
{
    std::set<int> s(in.begin(), in.end());
    std::vector<int> copied(s.begin(), s.end());
    std::ranges::sort(copied, std::greater<int>{});
    return copied == in;
}

bool small_diffs(std::vector<int> in)
{
    
    for (auto in_iter = in.begin(); in_iter < in.end() - 1; ++in_iter)
    {
        if ( std::abs(*in_iter - *(in_iter+1)) > 3 ) {
            return false;
        }
    }
    return true;
}

int main()
{
    std::ifstream f("input.txt");
    std::string line, tok;
    std::vector<std::vector<int>> all;
    while (getline(f, line))
    {
        all.push_back(std::vector<int>());
        auto ss = std::stringstream(line);
        while (getline(ss, tok, ' '))
        {
            all.back().push_back(std::stoi(tok));
        }
    }
    f.close();

    uint32_t safe = 0;
    for (auto i : all)
    {
        if (monotonically_increasing(i) || monotonically_decreasing(i))
        {
            safe += small_diffs(i);
        }
    }
    std::cout << safe << std::endl;
}

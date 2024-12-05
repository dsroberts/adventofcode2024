#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>

bool in_order(std::vector<int> update, int i, std::vector<int> afters)
{
    for (auto j = i + 1; j < update.size(); ++j)
    {
        if (std::find(afters.begin(), afters.end(), update[j]) == afters.end())
        {
            return false;
        }
    }
    return true;
}

int main()
{
    std::ifstream f("input.txt");
    std::string line;
    std::vector<std::string> all;
    std::map<int, std::vector<int>> afters{};
    std::vector<std::vector<int>> updates;

    bool ordering_mode = true;

    while (getline(f, line))
    {
        all.push_back(line);
        if (line.empty())
        {
            ordering_mode = false;
            continue;
        }
        if (ordering_mode)
        {
            std::string tok;
            auto ss = std::stringstream(line);
            getline(ss, tok, '|');
            int left = std::stoi(tok);
            getline(ss, tok, '|');
            int right = std::stoi(tok);
            afters[left].push_back(right);
        }
        else
        {
            updates.emplace_back();
            std::string tok;
            auto ss = std::stringstream(line);
            while (getline(ss, tok, ','))
            {
                updates.back().push_back(std::stoi(tok));
            }
        }
    }

    int sum = 0;
    for (auto update : updates)
    {
        bool ordered = true;
        for (auto i = 0; i < update.size(); i++)
        {
            ordered &= in_order(update, i, afters[update[i]]);
        }
        if (ordered)
        {
            sum += update[(update.size()-1) / 2];
        }
    }
    std::cout << sum << std::endl;
}
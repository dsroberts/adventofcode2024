#include <fstream>
#include <iostream>
#include <string>
#include <iterator>
#include <vector>

int main()
{
    std::ifstream f("input.txt");
    std::string line;
    getline(f, line);
    std::vector<int> sparse_format;
    auto fileid = 0ul;
    auto space = 0l;
    for (auto c : line)
    {
        uint32_t count = std::stol(std::string{c});
        if (space)
        {
            for (auto i = 0ul; i < count; i++)
            {
                sparse_format.push_back(-1);
            }
        }
        else
        {
            for (auto i = 0ul; i < count; i++)
            {
                sparse_format.push_back(fileid);
            }
            fileid++;
        }
        space = 1 - space;
    }

    auto cb = sparse_format.rbegin();
    for (auto c = sparse_format.begin(); c < sparse_format.end(); c++)
    {
        if (*c == -1)
        {
            std::swap(*c, *cb);
            cb++;
            while (*cb == -1)
            {
                cb++;
            }
        }
        if (std::distance(c, sparse_format.end()) < std::distance(sparse_format.rbegin(), cb) + 2)
        {
            break;
        }
    }
    auto sum = 0ul;
    for (auto i = 0ul; i < sparse_format.size(); i++)
    {
        if (sparse_format[i] != -1)
        {
            sum += (sparse_format[i] * i);
        }
    }
    std::cout << sum << std::endl;
}
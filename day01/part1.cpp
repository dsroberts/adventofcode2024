#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>

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

    auto sum = 0l;
    for (auto i = 0ul; i < col1.size(); i++)
    {
        sum += (col1[i] > col2[i] ? col1[i] - col2[i] : col2[i] - col1[i]);
    }
    std::cout << sum << std::endl;
}

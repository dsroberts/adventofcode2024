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

    auto similarity = 0l;
    auto col1_iter = col1.begin();
    auto col2_iter = col2.begin();
    while (col1_iter != col1.end() && col2_iter != col2.end())
    {
        if (*col1_iter < *col2_iter)
        {
            col1_iter++;
        }
        else if (*col2_iter < *col1_iter)
        {
            col2_iter++;
        }
        else
        {
            similarity += *col1_iter;
            col2_iter++;
        }
    }
    std::cout << similarity << std::endl;
}

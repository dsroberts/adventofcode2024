#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <numeric>

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
    std::vector<int> diffs;
    std::transform(col1.begin(), col1.end(),col2.begin(),std::back_inserter(diffs),diff);
    std::cout << std::accumulate(diffs.begin(),diffs.end(),0) << std::endl;
}

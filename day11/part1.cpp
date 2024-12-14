#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdint>
#include <cmath>

void blink(std::vector<uint64_t> *stones1, std::vector<uint64_t> *stones2)
{
    stones2->clear();
    stones2->reserve(static_cast<uint64_t>(stones1->size()) * 2ull);
    for (auto j = 0ul; j < stones1->size(); ++j)
    {
        auto v = stones1->operator[](j);
        if (v == 0ull)
        {
            stones2->push_back(1ull);
        }
        else if (static_cast<uint64_t>(std::log10(v)) % 2ull == 1ull)
        {
            auto factor = static_cast<uint64_t>(std::pow(10, (static_cast<uint64_t>(std::log10(v)) + 1) / 2ull));
            stones2->push_back(v / factor);
            stones2->push_back(v % factor);
        }
        else
        {
            stones2->push_back(v * 2024ull);
        }
    }
}

int main()
{
    std::ifstream f("input.txt");
    std::string line;
    getline(f, line);
    f.close();
    std::stringstream ss{line};
    std::string tok;
    std::vector<uint64_t> stones1, stones2;
    std::vector<uint64_t> *s1 = &stones1;
    std::vector<uint64_t> *s2 = &stones2;
    const size_t iters = 25;
    while (getline(ss, tok, ' '))
    {
        if (tok.empty())
        {
            continue;
        }
        stones1.push_back(std::stoull(tok));
    }
    for (auto i : *s1)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    for (auto i = 0ul; i < iters; ++i)
    {
        blink(s1, s2);
        if (s1 == &stones1)
        {
            s1 = &stones2;
            s2 = &stones1;
        }
        else
        {
            s2 = &stones2;
            s1 = &stones1;
        }
        std::cout << i << ": " << s1->size() << std::endl;
    }
}
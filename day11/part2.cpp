#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdint>
#include <cmath>
#include <map>

typedef std::map<std::pair<uint64_t,uint64_t>,uint64_t> cache_t;

uint64_t blink(uint64_t stone, size_t times, cache_t &cache)
{
    if (cache.find(std::pair{stone,times}) != cache.end())
    {
        return cache[std::pair{stone,times}];
    }
    if (times == 0ul)
    {
        return 1ull;
    }
    if (stone == 0ull)
    {
        cache[std::pair{stone,times}] = blink(1ull, times - 1, cache);
        return blink(1ull, times - 1, cache);
    }
    if (static_cast<uint64_t>(std::log10(stone)) % 2ull == 1ull)
    {
        auto factor = static_cast<uint64_t>(std::pow(10, (static_cast<uint64_t>(std::log10(stone)) + 1) / 2ull));
        auto v = blink(stone / factor, times - 1, cache) + blink(stone % factor, times - 1, cache);
        cache[std::pair{stone,times}] = v;
        return v;
    }
    auto v = blink(stone * 2024ull, times - 1, cache);
    cache[std::pair{stone,times}] = v;
    return v;
}

int main()
{
    std::ifstream f("input.txt");
    std::string line;
    getline(f, line);
    f.close();
    std::stringstream ss{line};
    std::string tok;
    std::vector<uint64_t> stones;
    cache_t cache;
    const size_t iters = 75;
    while (getline(ss, tok, ' '))
    {
        if (tok.empty())
        {
            continue;
        }
        stones.push_back(std::stoull(tok));
    }

    auto sum = 0ull;
    for (auto i : stones)
    {
        sum += blink(i, iters, cache);
    }
    std::cout << sum << std::endl;
}
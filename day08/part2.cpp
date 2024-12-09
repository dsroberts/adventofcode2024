#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include <algorithm>
#include <set>

typedef std::pair<int, int> coord;

struct grid_bounds
{
    int nx;
    int ny;
    bool operator()(int x, int y) { return x < nx &&
                                           x >= 0 &&
                                           y < ny &&
                                           y >= 0; }
};
coord dist(coord a, coord b)
{
    return {a.first - b.first, a.second - b.second};
}

bool find(coord a, std::vector<coord> *coords)
{
    return std::find(coords->begin(), coords->end(), a) != coords->end();
}

int main()
{
    std::ifstream f("input.txt");
    std::string line;
    std::map<char, std::vector<coord>> tower_positions;
    std::set<coord> antinodes;

    int line_ctr{0}, char_ctr{0};

    while (getline(f, line))
    {
        char_ctr = 0;
        for (auto c : line)
        {
            if (c != '.')
            {

                tower_positions[c].push_back({char_ctr, line_ctr});
                antinodes.insert({char_ctr, line_ctr});
            }

            char_ctr++;
        }
        line_ctr++;
    }
    f.close();
    grid_bounds in_grid{.nx = char_ctr, .ny = line_ctr};

    for (auto tower : tower_positions)
    {
        for (auto tpos : tower.second)
        {
            for (auto other_tpos : tower.second)
            {

                if (tpos == other_tpos)
                {
                    continue;
                }
                auto d = dist(tpos, other_tpos);
                for (auto dmul = 1l; in_grid(tpos.first + dmul * d.first, tpos.second + dmul * d.second); ++dmul)
                {
                    antinodes.insert({tpos.first + dmul * d.first, tpos.second + dmul * d.second});
                }
            }
        }
    }
    std::cout << antinodes.size() << std::endl;
}
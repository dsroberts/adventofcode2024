#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>

struct guard_coords
{
    int x;
    int y;
    int deltax;
    int deltay;
    bool operator==(const guard_coords other)
    {
        return x == other.x && y == other.y && deltax == other.deltax && deltay == other.deltay;
    }
};

void turn_right(guard_coords &c)
{
    if (c.deltax == 0)
    {
        if (c.deltay == -1)
        {
            c.deltax = 1;
        }
        else
        {
            c.deltax = -1;
        }
        c.deltay = 0;
    }
    else
    {
        if (c.deltax == -1)
        {
            c.deltay = -1;
        }
        else
        {
            c.deltay = 1;
        }
        c.deltax = 0;
    }
}

int main()
{
    std::ifstream f("input.txt");
    std::string line;
    std::vector<std::string> all;
    std::vector<guard_coords> visited;
    std::set<std::pair<int, int>> unique_visited;

    guard_coords coords, start_pos{.deltax = 0, .deltay = -1};

    while (getline(f, line))
    {
        all.push_back(line);
    }
    f.close();
    for (int i = 0; i < all.size(); i++)
    {
        for (int j = 0; j < all[i].size(); j++)
        {

            if (all[i][j] == '^')
            {
                start_pos.x = j;
                start_pos.y = i;
                visited.push_back(start_pos);
                break;
            }
        }
    }
    coords = start_pos;
    while (true)
    {
        coords.x += coords.deltax;
        coords.y += coords.deltay;
        visited.push_back(coords);
        if (coords.y + coords.deltay >= all.size() || coords.y + coords.deltay < 0 || coords.x + coords.deltax >= all[0].size() || coords.x + coords.deltax < 0)
        {
            break;
        }
        if (all[coords.y + coords.deltay][coords.x + coords.deltax] == '#')
        {
            turn_right(coords);
        }
    }
    for (auto coord : visited)
    {
        unique_visited.emplace(coord.y, coord.x);
    }
    std::cout << unique_visited.size() << std::endl;

    int loop_counter = 0;
    for (auto &obstruction_pos : unique_visited)
    {
        auto prev = all[obstruction_pos.second][obstruction_pos.first];
        all[obstruction_pos.second][obstruction_pos.first] = '#';
        coords = start_pos;
        visited.clear();
        while (true)
        {
            coords.x += coords.deltax;
            coords.y += coords.deltay;
            if (coords.y + coords.deltay >= all.size() || coords.y + coords.deltay < 0 || coords.x + coords.deltax >= all[0].size() || coords.x + coords.deltax < 0)
            {
                break;
            }
            while (all[coords.y + coords.deltay][coords.x + coords.deltax] == '#')
            {
                turn_right(coords);
            }
            if (std::find(visited.begin(), visited.end(), coords) != visited.end())
            {
                loop_counter++;
                break;
            }
            visited.push_back(coords);
        }
        all[obstruction_pos.second][obstruction_pos.first] = prev;
        std::cout << loop_counter << std::endl;
    }
    std::cout << loop_counter << std::endl;
}
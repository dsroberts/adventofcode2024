#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

struct guard_coords
{
    int x;
    int y;
    bool operator==(const guard_coords other){
        return x == other.x && y == other.y;
    }
};

void turn_right(guard_coords *delta)
{
    if (delta->x == 0)
    {
        if (delta->y == -1)
        {
            delta->x = 1;
        }
        else
        {
            delta->x = -1;
        }
        delta->y = 0;
    }
    else
    {
        if (delta->x == -1)
        {
            delta->y = -1;
        }
        else
        {
            delta->y = 1;
        }
        delta->x = 0;
    }
}

int main()
{
    std::ifstream f("input.txt");
    std::string line;
    std::vector<std::string> all;
    std::vector<guard_coords> visited;

    guard_coords coords, delta{.x = 0, .y = -1};

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
                coords.x = j;
                coords.y = i;
                visited.push_back(coords);
                break;
            }
        }
    }
    int counter = 0;
    while (true)
    {
        coords.x += delta.x;
        coords.y += delta.y;
        if (std::find(visited.begin(), visited.end(), coords) == visited.end())
        {
            visited.push_back(coords);
        }
        if (coords.y + delta.y >= all.size() || coords.y + delta.y < 0 || coords.x + delta.x >= all[0].size() || coords.x + delta.x < 0)
        {
            break;
        }
        if (all[coords.y + delta.y][coords.x + delta.x] == '#')
        {
            turn_right(&delta);
        }
    }
    std::cout << visited.size() << std::endl;
}
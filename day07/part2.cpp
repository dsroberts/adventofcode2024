#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <functional>
#include <algorithm>
#include <ranges>
#include <cstdint>

class Plus_Or_Mul
{
public:
    Plus_Or_Mul(int count) : ctr(0),
                             func_idx(0)
    {
        for (auto pipe_count = 0l; pipe_count < count + 1; pipe_count++)
        {
            for (auto star_count = 0l; star_count < count - pipe_count + 1; star_count++)
            {
                for (auto plus_count = 0l; plus_count < count - star_count - pipe_count + 1; plus_count++)
                {
                    if (pipe_count + star_count + plus_count == count)
                    {
                        std::string tmp = std::string(star_count, '*') + std::string(plus_count, '+') + std::string(pipe_count, '|');
                        do
                        {
                            ops.push_back(tmp);
                        } while (std::next_permutation(tmp.begin(), tmp.end()));
                    }
                }
            }
        }
    };
    void inc_idx()
    {
        ctr = 0;
        func_idx++;
    }
    uint64_t func(const uint64_t a, const uint64_t b)
    {

        if (ops[func_idx][ctr] == '*')
        {
            ctr++;
            return a * b;
        }
        else if (ops[func_idx][ctr] == '+')
        {
            ctr++;
            return a + b;
        }
        else
        {
            ctr++;
            std::stringstream ss;
            ss << a << b;
            return std::stoull(ss.str());
        }
    }
    auto size()
    {
        return ops.size();
    }
    auto get_op()
    {
        return ops[func_idx];
    }

private:
    int ctr;
    std::vector<std::string> ops;
    int func_idx;
};

int main()
{
    std::ifstream f("input.txt");
    std::string line;
    std::vector<uint64_t> targets;
    std::vector<std::vector<uint64_t>> vals;
    while (getline(f, line))
    {
        std::string tok;
        auto ss = std::stringstream(line);
        getline(ss, tok, ':');
        targets.emplace_back(std::stoull(tok));
        vals.emplace_back();
        while (getline(ss, tok, ' '))
        {
            if (!tok.empty())
            {
                vals.back().push_back(std::stoull(tok));
            }
        }
    }
    f.close();
    uint64_t sum = 0;
    for (auto [target, values] : std::views::zip(targets, vals))
    {
        auto funcs = Plus_Or_Mul(values.size() - 1);
        for (auto i = 0ul; i < funcs.size(); i++)
        {
            auto mul = std::ranges::fold_left(values.begin() + 1, values.end(), *values.begin(), [&funcs](uint64_t a, uint64_t b)
                                              { return funcs.func(a, b); });
            if (mul == target)
            {
                sum += mul;
                // std::cout << uint64_t(mul) << ": ";
                // auto ops = funcs.get_op();
                // for (auto i = 0u; i < ops.size(); i++)
                //{
                //     std::cout << values[i] << " " << ops[i] << " ";
                // }
                // std::cout << values.back() << std::endl;
                break;
            }
            funcs.inc_idx();
        }
    }
    std::cout << sum << std::endl;
}
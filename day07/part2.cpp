#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <functional>
#include <algorithm>
#include <ranges>
#include <cstdint>

uint64_t append(uint64_t a, uint64_t b)
{
    std::stringstream ss;
    ss << a << b;
    return std::stoull(ss.str());
}

template <class T>
class SortableOperator
{
public:
    SortableOperator(std::function<T(T, T)> op, int idx, std::string repr) : op_(op),
                                                                             idx_(idx),
                                                                             repr_(repr)
    {
    }
    T operator()(T a, T b) { return op_(a, b); };
    bool operator<(const SortableOperator<T> other) { return idx_ < other.idx_; };
    friend std::ostream &operator<<(std::ostream &out, SortableOperator<T> op)
    {
        return out << op.repr_;
    };

private:
    std::function<T(T, T)> op_;
    int idx_;
    std::string repr_;
};

class Plus_Or_Mul
{
public:
    Plus_Or_Mul(uint32_t count) : ctr_(0),
                                  func_idx_(0)
    {
        for (auto pipe_count = 0ul; pipe_count < count + 1; pipe_count++)
        {
            for (auto star_count = 0ul; star_count < count - pipe_count + 1; star_count++)
            {
                auto plus_count = count - pipe_count - star_count;

                std::vector<SortableOperator<uint64_t>> tmp{star_count, SortableOperator<uint64_t>(std::multiplies<uint64_t>{}, 1, std::string{'*'})};
                std::vector<SortableOperator<uint64_t>> tmp2{plus_count, SortableOperator<uint64_t>(std::plus<uint64_t>{}, 2, std::string{'+'})};
                std::vector<SortableOperator<uint64_t>> tmp3{pipe_count, SortableOperator<uint64_t>(append, 3, std::string{"||"})};
                std::move(tmp2.begin(), tmp2.end(), std::back_inserter(tmp));
                std::move(tmp3.begin(), tmp3.end(), std::back_inserter(tmp));
                do
                {
                    ops_.push_back(tmp);
                } while (std::next_permutation(tmp.begin(), tmp.end()));
            }
        }
    };
    void inc_idx()
    {
        ctr_ = 0;
        func_idx_++;
    }
    uint64_t func(const uint64_t a, const uint64_t b)
    {
        return (ops_[func_idx_][ctr_++](a, b));
    }
    auto size()
    {
        return ops_.size();
    }
    auto get_op()
    {
        return ops_[func_idx_];
    }

private:
    int ctr_;
    std::vector<std::vector<SortableOperator<uint64_t>>> ops_;
    int func_idx_;
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
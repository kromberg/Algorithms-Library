#ifndef KNAPSACK_H
#define KNAPSACK_H

#include <vector>
#include <list>
#include <fstream>
#include <cstdint>

class Knapsack
{
public:
    struct Item
    {
        uint32_t m_weight;
        uint32_t m_value;
        Item();
        Item(const uint32_t weight, const uint32_t value);
        Item(const Item& i);
        Item(Item&& i);
        Item& operator=(Item&& i);
    };
    typedef std::vector<Item> ItemsVec;
    typedef std::list<Item> ItemsList;
private:
    struct ItemScore
    {
        bool m_set;
        double m_val;
        uint32_t m_idx;
        ItemScore();
        ItemScore(double val);
        ItemScore(ItemScore&& itemScore);
        ItemScore& operator=(ItemScore&& itemScore);
    };
    
    uint32_t m_maxWeight;
    ItemsVec m_items;

public:
    Knapsack();
    ~Knapsack();

    bool read(std::istream& in);
    bool run(uint32_t& res, ItemsList& resList);
    bool runGreedy(uint32_t &res);
};

#endif // KNAPSACK_H
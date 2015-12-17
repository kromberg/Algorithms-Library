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
    };
    typedef std::vector<Item> ItemsVec;
    typedef std::list<Item> ItemsList;
private:
    ItemsVec m_items;

public:
    Knapsack();
    ~Knapsack();

    bool read(std::istream& in);
    bool run(const uint32_t maxWeight, uint32_t& res, ItemsList& resList = ItemsList());
};

#endif // KNAPSACK_H
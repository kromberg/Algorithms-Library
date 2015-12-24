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
        ItemScore(const uint32_t idx);
        ItemScore(const uint32_t idx, double val);
        ItemScore(ItemScore&& itemScore);
        ItemScore& operator=(ItemScore&& itemScore);
        bool operator<(const ItemScore& itemScore);
    };

    struct Entity
    {
        bool m_set;
        uint32_t m_val;
        Entity();
        Entity(const uint32_t val);
        Entity(Entity&& entity);
        Entity& operator=(Entity&& entity);
    };
    friend std::ostream& operator<< (std::ostream& out, const Entity& entity);

    uint32_t m_maxWeight;
    ItemsVec m_items;

public:
    Knapsack();
    ~Knapsack();

    bool read(std::istream& in);
    bool run(uint32_t& res, ItemsList& resList);
    bool runByValue(uint32_t& res, ItemsList& resList);
    bool runGreedy(uint32_t &res, ItemsList& resList);
};

#endif // KNAPSACK_H
/*****************************************************************************
THIS IS TEST FILE FOR CONNECTING RESULTS IN ONE FILE
******************************************************************************/
#if 0
#include <cstdint>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>

class LongestIncrSubseq
{
private:
    std::vector<uint32_t> m_elements;
public:

    LongestIncrSubseq()
    {
    }


    ~LongestIncrSubseq()
    {
    }

    bool runAlgorithm(uint32_t& res /*todo: sequence*/)
    {
        uint32_t numElements = static_cast<uint32_t>(m_elements.size());
        std::map<uint32_t, uint32_t> entities;
        entities[m_elements[0]] = 1;

        struct
        {
            bool m_set;
            uint32_t m_maxElements;
        } maxEntity;
        for (uint32_t i = 1; i < numElements; ++i)
        {
            maxEntity.m_set = false;
            for (auto it = entities.begin(); it != entities.end() && it->first < m_elements[i]; ++it)
            {
                if (!maxEntity.m_set ||
                    (it->second > maxEntity.m_maxElements))
                {
                    maxEntity.m_set = true;
                    maxEntity.m_maxElements = it->second;
                }
            }

            if (!maxEntity.m_set)
            {
                auto it = entities.find(m_elements[i]);
                if (entities.end() == it)
                {
                    entities.insert(std::make_pair(m_elements[i], 1));
                }
            }
            else
            {
                auto it = entities.find(m_elements[i]);
                if (entities.end() == it)
                {
                    entities.insert(std::make_pair(m_elements[i], maxEntity.m_maxElements + 1));
                }
                else if (it->second < maxEntity.m_maxElements + 1)
                {
                    it->second = maxEntity.m_maxElements + 1;
                }
            }
        }

        res = 0;
        for (auto& entity: entities)
        {
            if (entity.second > res)
            {
                res = entity.second;
            }
        }

        return true;
    }



    friend std::istream& operator>>(std::istream& in, LongestIncrSubseq& l);
};

std::istream& operator>>(std::istream& in, LongestIncrSubseq& l)
{
    uint32_t numElements = 0;
    in >> numElements;
    l.m_elements.resize(numElements);
    for (auto& element : l.m_elements)
    {
        in >> element;
    }
    return in;
}

void LongestIncrSubseqFunc()
{
    LongestIncrSubseq longestIncrSubseq;
    std::cin >> longestIncrSubseq;
    uint32_t res = 0;
    longestIncrSubseq.runAlgorithm(res);
    std::cout << res << std::endl;
}

int main(int argc, const char * argv[])
{
    LongestIncrSubseqFunc();

    return 0;
}
#endif 
#ifndef MAX_WIS_H
#define MAX_WIS_H

#include <vector>
#include <list>
#include <fstream>
#include <cstdint>

class MaxWIS
{
private:
    std::vector<uint32_t> m_vertices;
public:
    MaxWIS();
    ~MaxWIS();

    bool read(std::istream& in);
    bool run(uint32_t& res, std::list<std::pair<size_t, uint32_t> >& resList);
};

#endif // MAX_WIS_H
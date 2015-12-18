#ifndef SEQUENCE_ALIGNMENT_H
#define SEQUENCE_ALIGNMENT_H

#include <unordered_map>
#include <vector>
#include <string>
#include <cstdint>

#include "Matrix.h"

class SequenceAlignment
{
private:
    typedef std::unordered_map<char, uint64_t> CharToIndexMap;
    CharToIndexMap m_charToIndexMap;
    std::string m_alphabet;
    uint64_t m_numScores;
    //std::vector<uint32_t> m_scores;
    Matrix<uint32_t> m_scores;

    uint32_t m_gapScore;
    std::string m_sequnces[2];

private:
    void clear();
public:
    SequenceAlignment();
    ~SequenceAlignment();

    bool read(std::istream& in);
    bool run(
        uint32_t &scoreNeedlemanWunsch,
        std::string (&seq)[2]);
};

#endif // SEQUENCE_ALIGNMENT_H
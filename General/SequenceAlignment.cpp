#include <algorithm>

#include "SequenceAlignment.h"

SequenceAlignment::SequenceAlignment():
    m_numScores(0),
    m_gapScore(0)
{
}

SequenceAlignment::~SequenceAlignment()
{
}

void SequenceAlignment::clear()
{
    m_charToIndexMap.clear();
    m_numScores = 0;
    m_gapScore = 0;
    m_scores.reset();
}

bool SequenceAlignment::read(std::istream& in)
{
    clear();

    // read alphabet
    std::string alphabet;
    in >> m_alphabet;

    uint64_t idx = 0;
    for (auto c : m_alphabet)
    {
        m_charToIndexMap.insert(std::make_pair(c, idx++));
    }

    m_numScores = m_alphabet.size();
    m_scores.resize(m_numScores, m_numScores);

    for (uint64_t i = 0; i < m_numScores; ++i)
    {
        for (uint64_t j = 0; j < m_numScores; ++j)
        {
            in >> m_scores[i][j];
        }
    }
    in >> m_gapScore;

    in >> m_sequnces[0] >> m_sequnces[1];

    return true;
}

bool SequenceAlignment::run(
    uint32_t &scoreNeedlemanWunsch,
    std::string(&seq)[2])
{
    uint64_t size[2] = { m_sequnces[0].size() + 1, m_sequnces[1].size() + 1 };
    Matrix<uint32_t> nwScore(size[0], size[1]);
    for (uint64_t i = 0; i < size[0]; ++i)
    {
        nwScore[i][0] = static_cast<uint32_t>((i + 1) * m_gapScore);
    }
    for (uint64_t i = 0; i < size[1]; ++i)
    {
        nwScore[0][i] = static_cast<uint32_t>((i + 1) * m_gapScore);
    }

    uint32_t tmpScore[3];
    for (uint64_t i = 1; i < size[0]; ++i)
    {
        for (uint64_t j = 1; j < size[1]; ++j)
        {
            uint64_t idx1 = m_charToIndexMap[m_sequnces[0][i - 1]];
            uint64_t idx2 = m_charToIndexMap[m_sequnces[1][j - 1]];
            tmpScore[0] = nwScore[i - 1][j - 1] + m_scores[idx1][idx2];
            tmpScore[1] = nwScore[i][j - 1] + m_gapScore;
            tmpScore[2] = nwScore[i - 1][j] + m_gapScore;
            nwScore[i][j] = std::min(tmpScore[0], std::min(tmpScore[1], tmpScore[2]));
        }
    }

    scoreNeedlemanWunsch = nwScore[size[0] - 1][size[1] - 1];

    for (uint64_t i = size[0] - 1, j = size[1] - 1; (i > 0) && (j > 0);)
    {
        uint64_t idx1 = m_charToIndexMap[m_sequnces[0][i - 1]];
        uint64_t idx2 = m_charToIndexMap[m_sequnces[1][j - 1]];
        tmpScore[0] = nwScore[i - 1][j - 1] + m_scores[idx1][idx2];
        tmpScore[1] = nwScore[i][j - 1] + m_gapScore;
        tmpScore[2] = nwScore[i - 1][j] + m_gapScore;
        if (nwScore[i][j] == tmpScore[0])
        {
            seq[0].push_back(m_sequnces[0][i - 1]);
            seq[1].push_back(m_sequnces[1][j - 1]);
            --i, --j;
        }
        else if (nwScore[i][j] == tmpScore[1])
        {
            seq[0].push_back(' ');
            seq[1].push_back(m_sequnces[1][j - 1]);
            --j;
        }
        else if (nwScore[i][j] == tmpScore[2])
        {
            seq[0].push_back(m_sequnces[0][i - 1]);
            seq[1].push_back(' ');
            --i;
        }
    }
    std::reverse(seq[0].begin(), seq[0].end());
    std::reverse(seq[1].begin(), seq[1].end());

    return true;
}
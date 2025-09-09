#ifndef D7C3D529_C645_4FF9_A052_C5FA1A0DA28F
#define D7C3D529_C645_4FF9_A052_C5FA1A0DA28F

#include "star.h"

#include <utility>
#include <array>

using std::pair;
using std::array;

struct tDataType {
	usshort multipl{};
	array<uschar, 4> exp{};
};

class Transition {
    private:
        const PointType m_coloured;
        vector<vector<pair<usint, tDataType > > > m_transitionTable;

        void initTransitionTable(const vector<Star>&);
        tDataType getTransitionData(const SimpleStar&, const SimpleStar&) const;
        bool isNegligible(const tDataType&) const;
        udouble calculateTransitionValue(const tDataType&, const udouble, const udouble) const;
    public:
        Transition(const vector<Star>& stars, const PointType& col) : m_coloured{col} {initTransitionTable(stars); }
				PointType getColoured() const {return m_coloured; }
        
				void doTheTransition(vector<Star>&, const udouble, const udouble) const;
};

#endif /* D7C3D529_C645_4FF9_A052_C5FA1A0DA28F */

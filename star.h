#ifndef E4DB30DF_8C67_43A7_9561_04F6928540F5
#define E4DB30DF_8C67_43A7_9561_04F6928540F5

#include "point.h"

class SimpleStar {
    protected:
        Point m_center;
        vector<Point> m_leafs;
				vector<uschar> m_leafOutTypes;	// leafOutTypes
        usshort m_multiplicity;
				usint m_starIndex;
    public:
        Point getCenter() const {return m_center; }
        vector<Point> getLeafs() const {return m_leafs; }
				vector<uschar> getLeafOutTypes() const {return m_leafOutTypes; }
        usshort getMultiplicity() const {return m_multiplicity; }
				usint getStarIndex() const {return m_starIndex; }

				SimpleStar(const uschar, const vector<uschar>&);
        SimpleStar(const usint);
        SimpleStar();
  
				bool operator==(const SimpleStar& ss) const {return m_starIndex == ss.m_starIndex; }
				bool operator< (const SimpleStar& ss) const {return m_starIndex <  ss.m_starIndex; }

        static vector<usint> firstNeighsInt(const SimpleStar&);	// stars reachable with colouring one point

				static SimpleStar reverseSimpleStar(const SimpleStar&);
};

class Star : public SimpleStar {
    private:
        udouble m_meas;
        vector<udouble> m_diffs;
    public:
        Star(const uschar center, const vector<uschar>& lot, const udouble meas): SimpleStar{center, lot}, m_meas{meas} {}
        Star(const uschar center, const vector<uschar>& lot): Star{center, lot, 0} {}
        udouble getMeas() const {return m_meas; }
        void setMeas(udouble meas) {m_meas = meas; }
        void addDiff(udouble diff) {m_diffs.push_back(diff); }
        
				void updateMeas();
};

#endif /* E4DB30DF_8C67_43A7_9561_04F6928540F5 */

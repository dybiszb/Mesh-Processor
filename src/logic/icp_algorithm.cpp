// author: dybisz

#include "logic/icp_algoriithm.h"

//------------------------------------------------------------------------------
ICPAlgorithm::ICPAlgorithm(const vector<Vector3f> &mesh1Points,
                           const vector<Vector3f> &mesh2Points) :
        m_mesh1Points(mesh1Points),
        m_mesh2Points(mesh2Points) {
}

//------------------------------------------------------------------------------
vector<ICPResults>
ICPAlgorithm::pointToPointsICP() {
    // NOTE: Q -> P   <=>  M2 -> M1
    vector<ICPResults> results;
    vector<Vector3f> pointsP = m_mesh1Points;
    vector<Vector3f> pointsQ = m_mesh2Points;

// while
    auto pairs = getPairs();
    // assume cetroid from all points
    auto centroids = getCentroid(pointsP, pointsQ);
    // assume it works xD
    auto tyldaPairs = getTyldaPairs(pairs, centroids);

    auto A = calculateMatrixA(tyldaPairs);
    cout << A << endl;


// end

    return results;
}

//------------------------------------------------------------------------------
Matrix3f
ICPAlgorithm::calculateMatrixA(const vector<pair<Vector3f, Vector3f>>& pairs) {
    Matrix3f A = Matrix3f::Zero();
    return A;
}

//------------------------------------------------------------------------------
pair<Vector3f, Vector3f>
ICPAlgorithm::getCentroid(const vector<Vector3f> & pointsP,
                          const vector<Vector3f> & pointsQ) {
    Vector3f cenP(0.0, 0.0, 0.0);
    Vector3f cenQ(0.0, 0.0, 0.0);

    for (auto const &vertP : pointsP) cenP += vertP;
    for (auto const &vertQ : pointsQ) cenQ += vertQ;
    cenP /= (double) pointsP.size();
    cenQ /= (double) pointsQ.size();

    return std::pair<Vector3f, Vector3f>(cenP, cenQ);
}

//------------------------------------------------------------------------------
float
ICPAlgorithm::euclideanDistance(const Vector3f &v1, const Vector3f &v2) {
    Vector3f diff = v1 - v2;
    return diff.norm();
}

//------------------------------------------------------------------------------
vector<pair<Vector3f, Vector3f>>
ICPAlgorithm::getTyldaPairs(const vector<pair<Vector3f, Vector3f>> &pairs,
                            const pair<Vector3f, Vector3f> &centroids) {

    vector<pair<Vector3f, Vector3f>> tyldaPairs;

    for (auto const &pair: pairs) {
        Vector3f tyldaP = pair.first - centroids.first;
        Vector3f tyldaQ = pair.second - centroids.second;
        std::pair<Vector3f, Vector3f> tyldaPair(tyldaP, tyldaQ);
        tyldaPairs.push_back(tyldaPair);
    }

    return tyldaPairs;
};

//------------------------------------------------------------------------------
vector<pair<Vector3f, Vector3f>>
ICPAlgorithm::getPairs() {
    vector<pair<Vector3f, Vector3f>> paired;

    for (int i = 0; i < m_mesh1Points.size(); ++i) {
        float minVal = INT8_MAX;
        float minInd = -1;

        for (int j = 0; j < m_mesh2Points.size(); ++j) {
            float dist = euclideanDistance(m_mesh1Points[i], m_mesh2Points[j]);
            if (dist < minVal) {
                minVal = dist;
                minInd = j;
            }
        }

        pair<Vector3f, Vector3f> pair(m_mesh1Points[i], m_mesh2Points[minInd]);
        paired.push_back(pair);
    }

    return paired;
};


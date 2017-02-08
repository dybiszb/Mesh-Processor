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
    vector<ICPResults> results;
    vector<Vector3f> pointsP = m_mesh1Points;
    vector<Vector3f> pointsQ = m_mesh2Points;

    for (int i = 0; i < 40; ++i) {
        auto pairs = getPairs(pointsP, pointsQ);
        auto centroids = getCentroids(pointsP, pointsQ);
        auto tyldaPairs = getTyldaPairs(pairs, centroids);
        auto A = calculateMatrixA(tyldaPairs);

        // Calculate New Transformations
        JacobiSVD<Matrix3f> svdOfA(A, ComputeThinU | ComputeThinV);
        Matrix3f R = svdOfA.matrixV() * svdOfA.matrixU().transpose();
        Vector3f t = centroids.first - R * centroids.second;

        // Save Result:
        ICPResults result(R, t, centroids.first, centroids.second);
        results.push_back(result);

        pointsQ = updateVertices(pointsQ, R, t);
    }

    return results;
}

//------------------------------------------------------------------------------
ICPResults
ICPAlgorithm::pointToPointsICPStep() {
    vector<Vector3f> pointsP = m_mesh1Points;
    vector<Vector3f> pointsQ = m_mesh2Points;

    auto pairs = getPairs(pointsP, pointsQ);
    auto centroids = getCentroids(pointsP, pointsQ);
    auto tyldaPairs = getTyldaPairs(pairs, centroids);
    auto A = calculateMatrixA(tyldaPairs);

    // Calculate New Transformations
    JacobiSVD<Matrix3f> svdOfA(A, ComputeThinU | ComputeThinV);
    Matrix3f R = svdOfA.matrixV() * svdOfA.matrixU().transpose();
    Vector3f t = centroids.first - R * centroids.second;

    // Return Result:
    ICPResults result(R, t, centroids.first, centroids.second);
    return result;

}

//------------------------------------------------------------------------------
vector<Vector3f>
ICPAlgorithm::updateVertices(const vector<Vector3f> &vertices,
                             const Matrix3f &R,
                             const Vector3f &t) {
    vector<Vector3f> updated;
    for (const auto &q_i : vertices) {
        Vector3f v = R * q_i;
        v += t;
        updated.push_back(v);
    }
    return updated;
}

//------------------------------------------------------------------------------
Matrix3f
ICPAlgorithm::calculateMatrixA(const vector<pair<Vector3f, Vector3f>> &pairs) {
    Matrix3f A = Matrix3f::Zero();

    for (auto const &pair: pairs) {
        const Vector3f &p_i = pair.first;
        const Vector3f &q_i = pair.second;
        A += q_i * p_i.transpose();
    }

    return A;
}

//------------------------------------------------------------------------------
pair<Vector3f, Vector3f>
ICPAlgorithm::getCentroids(const vector<Vector3f> &pointsP,
                           const vector<Vector3f> &pointsQ) {
    Vector3f cenP(0.0, 0.0, 0.0);
    Vector3f cenQ(0.0, 0.0, 0.0);

    for (auto const &vertP : pointsP) cenP += vertP;
    for (auto const &vertQ : pointsQ) cenQ += vertQ;
    cenP /= (double) pointsP.size();
    cenQ /= (double) pointsQ.size();

    return std::pair<Vector3f, Vector3f>(cenP, cenQ);
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
ICPAlgorithm::getPairs(const vector<Vector3f> &mesh1Points,
                       const vector<Vector3f> &mesh2Points) {
    vector<pair<Vector3f, Vector3f>> paired;

    for (int i = 0; i < mesh1Points.size(); ++i) {
        float minVal = INT8_MAX;
        float minInd = -1;

        for (int j = 0; j < mesh2Points.size(); ++j) {
            float dist = (mesh1Points[i] - mesh2Points[j]).norm();
            if (dist < minVal) {
                minVal = dist;
                minInd = j;
            }
        }

        paired.push_back(pair<Vector3f, Vector3f> (mesh1Points[i],
                                                   mesh2Points[minInd]));
    }

    return paired;
};


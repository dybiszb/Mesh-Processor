// author: dybisz

#include "logic/icp_algoriithm.h"

//------------------------------------------------------------------------------
vector<ICPResults>
ICPAlgorithm::pointToPointsICP(PointsCloud m1,
                               PointsCloud m2) {
    vector<ICPResults> results;
    const vector<Vector3f>& pointsP = m1.getUpdatedVertices();

    for(int i = 0; i < 50; ++i) {

        const vector<Vector3f>& pointsQ = m2.getUpdatedVertices();

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

        m2.accumulateRotation(R);
        m2.accumulateTranslation(t);
    }

    std::reverse(results.begin(), results.end());
    return results;
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
    tyldaPairs.reserve(pairs.size());

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
    paired.reserve(mesh1Points.size());

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


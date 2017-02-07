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
//        auto centroids = getCentroids(pairs);

        Vector3f cenDiff = centroids.first - centroids.second;
//        if (cenDiff.norm() < 1.5e-08) break;
        cout << "centr - centr" << cenDiff.norm() << endl;
//        cout << "\ncentroid 1:\n" << centroids.first << "\ncentroid 2:\n" <<
//             centroids.second << endl;

        // assume it works xD
        auto tyldaPairs = getTyldaPairs(pairs, centroids);

        auto A = calculateMatrixA(tyldaPairs);

        // Calculate New Transformations
        JacobiSVD<Matrix3f> svdOfA(A, ComputeThinU | ComputeThinV);
        Matrix3f R = svdOfA.matrixV() * svdOfA.matrixU().transpose();
        Vector3f t = centroids.first - R * centroids.second;

        // Save Result:
        ICPResults result(R, t, centroids.first, centroids.second);
        results.push_back(result);

        updateVertices(pointsQ, R, t);
    }

    return results;
}

void
ICPAlgorithm::updateVertices(vector<Vector3f> &vertices,
                             const Matrix3f &R,
                             const Vector3f t) {
    for (auto &q_i : vertices) {
        q_i = R * q_i;
        q_i += t;
    }
}

//------------------------------------------------------------------------------
Matrix3f
ICPAlgorithm::calculateMatrixA(const vector<pair<Vector3f, Vector3f>> &pairs) {
    Matrix3f A = Matrix3f::Zero();

    for (auto const &pair: pairs) {
        Vector3f p_i = pair.first;
        Vector3f q_i = pair.second;
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
pair<Vector3f, Vector3f>
ICPAlgorithm::getCentroids(const vector<pair<Vector3f, Vector3f>> &pairs) {
    Vector3f cenP(0.0, 0.0, 0.0);
    Vector3f cenQ(0.0, 0.0, 0.0);

    for (auto const &pair: pairs) {
        cenP += pair.first;
        cenQ += pair.second;
    }
    cenP /= (double) pairs.size();
    cenQ /= (double) pairs.size();

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
ICPAlgorithm::getPairs(const vector<Vector3f>& mesh1points,
                       const vector<Vector3f> & mesh2Points) {
    vector<pair<Vector3f, Vector3f>> paired;

    for (int i = 0; i < mesh1points.size(); ++i) {
        float minVal = INT8_MAX;
        float minInd = -1;

        for (int j = 0; j < mesh2Points.size(); ++j) {
            float dist = euclideanDistance(m_mesh1Points[i], m_mesh2Points[j]);
            if (dist < minVal) {
                minVal = dist;
                minInd = j;
            }
        }

        pair<Vector3f, Vector3f> pair(mesh1points[i], mesh2Points[minInd]);
        paired.push_back(pair);
    }

    return paired;
};


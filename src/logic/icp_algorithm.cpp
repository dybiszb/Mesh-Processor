// author: dybisz

#include "logic/icp_algoriithm.h"

//------------------------------------------------------------------------------
vector<ICPResults>
ICPAlgorithm::pointToPointsICP(PointsCloud m1,
                               PointsCloud m2,
                               bool useNearestNeighbors,
                               int samples) {
    vector<ICPResults> results;
    const vector<Vector3f> &pointsP = m1.getUpdatedVertices();
    float averageEuclidean = 0;

    for (int i = 0; i < 100; ++i) {

        const vector<Vector3f> &pointsQ = m2.getUpdatedVertices();
        averageEuclidean = 0;

        vector<pair<Vector3f, Vector3f>> pairs = __getPairs(pointsP, pointsQ,
                                                            averageEuclidean,
                                                            samples);

        if (averageEuclidean < 0.002) break;

        auto centroids = __getCentroids(pointsP, pointsQ);
        auto tyldaPairs = __getTyldaPairs(pairs, centroids);
        auto A = __calculateMatrixA(tyldaPairs);

        // Calculate New Transformations
        JacobiSVD<Matrix3f> svdOfA(A, ComputeThinU | ComputeThinV);
        Matrix3f R = svdOfA.matrixV() * svdOfA.matrixU().transpose();
        Vector3f t = centroids.first - R * centroids.second;

        // Save Result:
        ICPResults result(R, t, centroids.first, centroids.second,
                          averageEuclidean);
        results.push_back(result);

        m2.accumulateRotation(R);
        m2.accumulateTranslation(t);

    }


    return results;
}

//------------------------------------------------------------------------------
Matrix3f
ICPAlgorithm::__calculateMatrixA(const vector<pair<Vector3f, Vector3f>> &pairs) {
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
ICPAlgorithm::__getCentroids(const vector<Vector3f> &pointsP,
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
ICPAlgorithm::__getTyldaPairs(const vector<pair<Vector3f, Vector3f>> &pairs,
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
ICPAlgorithm::__getPairs(vector<Vector3f> mesh1Points,
                         const vector<Vector3f> &mesh2Points,
                         float &averangeEuclidean,
                         int samples) {
    vector<pair<Vector3f, Vector3f>> paired;
    paired.reserve(mesh1Points.size());

    // Sub sample if necessary
    if (samples > 0 && samples < mesh1Points.size()) {
        std::random_shuffle(mesh1Points.begin(), mesh1Points.end());
        mesh1Points = vector<Vector3f>(mesh1Points.begin(), mesh1Points.begin() + samples);
    }

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
        averangeEuclidean += minVal;
        paired.push_back(pair<Vector3f, Vector3f>(mesh1Points[i],
                                                  mesh2Points[minInd]));
    }
    averangeEuclidean /= (float) mesh1Points.size();
    return paired;
};

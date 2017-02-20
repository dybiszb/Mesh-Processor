// author: dybisz

#include "logic/icp_algoriithm.h"

//------------------------------------------------------------------------------
std::vector<ICPResults>
ICPAlgorithm::pointToPointsICP(PointsCloud m1,
                               PointsCloud m2,
                               bool useNearestNeighbors,
                               int samples) {
    std::vector<ICPResults> results;
    const std::vector<vec3> &pointsP = m1.getUpdatedVertices();
    float averageEuclidean = 0;

    for (int i = 0; i < 100; ++i) {

        const std::vector<vec3> &pointsQ = m2.getUpdatedVertices();
        averageEuclidean = 0;

        std::vector<std::pair<vec3, vec3>> pairs = __getPairs(pointsP, pointsQ,
                                                    averageEuclidean,
                                                    samples);

        if (averageEuclidean < 0.002) break;

        auto centroids = __getCentroids(pointsP, pointsQ);
        auto tyldaPairs = __getTyldaPairs(pairs, centroids);
        auto A = __calculateMatrixA(tyldaPairs);

        // Calculate New Transformations
        Eigen::JacobiSVD<mat3> svdOfA(A, Eigen::ComputeThinU |
                                         Eigen::ComputeThinV);
        mat3 R = svdOfA.matrixV() * svdOfA.matrixU().transpose();
        vec3 t = centroids.first - R * centroids.second;

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
mat3
ICPAlgorithm::__calculateMatrixA(
        const std::vector<std::pair<vec3, vec3>> &pairs) {
    mat3 A = mat3::Zero();

    for (auto const &pair: pairs) {
        const vec3 &p_i = pair.first;
        const vec3 &q_i = pair.second;
        A += q_i * p_i.transpose();
    }

    return A;
}

//------------------------------------------------------------------------------
std::pair<vec3, vec3>
ICPAlgorithm::__getCentroids(const std::vector<vec3> &pointsP,
                             const std::vector<vec3> &pointsQ) {
    vec3 cenP(0.0, 0.0, 0.0);
    vec3 cenQ(0.0, 0.0, 0.0);

    for (auto const &vertP : pointsP) cenP += vertP;
    for (auto const &vertQ : pointsQ) cenQ += vertQ;
    cenP /= (double) pointsP.size();
    cenQ /= (double) pointsQ.size();

    return std::pair<vec3, vec3>(cenP, cenQ);
}

//------------------------------------------------------------------------------
std::vector<std::pair<vec3, vec3>>
ICPAlgorithm::__getTyldaPairs(const std::vector<std::pair<vec3, vec3>> &pairs,
                              const std::pair<vec3, vec3> &centroids) {

    std::vector<std::pair<vec3, vec3>> tyldaPairs;
    tyldaPairs.reserve(pairs.size());

    for (auto const &pair: pairs) {
        vec3 tyldaP = pair.first - centroids.first;
        vec3 tyldaQ = pair.second - centroids.second;
        std::pair<vec3, vec3> tyldaPair(tyldaP, tyldaQ);
        tyldaPairs.push_back(tyldaPair);
    }

    return tyldaPairs;
};

//------------------------------------------------------------------------------
std::vector<std::pair<vec3, vec3>>
ICPAlgorithm::__getPairs(std::vector<vec3> mesh1Points,
                         const std::vector<vec3> &mesh2Points,
                         float &averangeEuclidean,
                         int samples) {
    std::vector<std::pair<vec3, vec3>> paired;
    paired.reserve(mesh1Points.size());

    // Sub sample if necessary
    if (samples > 0 && samples < mesh1Points.size()) {
        std::random_shuffle(mesh1Points.begin(), mesh1Points.end());
        mesh1Points = std::vector<vec3>(mesh1Points.begin(),
                                   mesh1Points.begin() + samples);
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
        paired.push_back(std::pair<vec3, vec3>(mesh1Points[i], mesh2Points[minInd]));
    }
    averangeEuclidean /= (float) mesh1Points.size();
    return paired;
};

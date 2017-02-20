//==============================================================================
// The class provides the means for utilizing ICP algorithm for two
// meshes/cloud points (for now).
//==============================================================================
// author: dybisz
//------------------------------------------------------------------------------
#ifndef ICP_ALGORIITHM_H
#define ICP_ALGORIITHM_H

#ifdef Success
#undef Success
#endif

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
#include <vector>
#include <utility>
#include <iostream>
#include <algorithm>
#include "logic/typedefs.h"
#include "rendering/gl_ply_model.h"
#include "entities/points_cloud.h"

struct ICPResults {
    ICPResults(const Eigen::Matrix3f &R,
               const Eigen::Vector3f &t,
               const Eigen::Vector3f &cM1,
               const Eigen::Vector3f &cM2,
               const float &avrgEucl) : m_R(R),
                                        m_t(t),
                                        m_centroidM1(cM1),
                                        m_centroidM2(cM2),
                                        avrgEucl(avrgEucl) {};
    Eigen::Matrix3f m_R;
    Eigen::Vector3f m_t;
    Eigen::Vector3f m_centroidM1;
    Eigen::Vector3f m_centroidM2;
    float avrgEucl;
};

class ICPAlgorithm {
public:
    /**
     * The procedure calculates ICP based on point-to-point scheme, for
     * provided points clouds.
     * NOTE: m2 is mapped on m1; PointClouds objects are assumed to be copies.
     *
     * @param m1 PointCloud object of mesh1
     * @param m2
     * @return
     */
    std::vector<ICPResults> pointToPointsICP(PointsCloud m1,
                                             PointsCloud m2,
                                             bool useNearestNeighbors = false,
                                             int subSamples = -1);

    /**
     * The procedure calculate ICP based on point-to-point scheme, for meshes
     * provided during the initialization.
     * NOTE: Mesh2 is mapped on Mesh1
     *
     * @return A sequence of intermediate results (rotations, translation) in
     *         form of a vector
     */
    std::vector<ICPResults> pointToPointsICP();

private:

    mat3 __calculateMatrixA(const std::vector<std::pair<vec3, vec3>> &pairs);

    /**
     * Using provided centroids: <c_p, c_q>, the procedure converts <p_i,
     * q_i> pairs into <p_i - c_p, q_i - c_q pairs.
     * pairs
     *
     * @param pairs     Vector of pairs of corresponding vertices.
     * @param centroids A pair of centroids, eahc entry for corresponding mesh.
     *
     * @return          Vector of pairs updated according to aforementioned
     *                  scheme.
     */
    std::vector<std::pair<vec3, vec3>> __getTyldaPairs(
            const std::vector<std::pair<vec3, vec3>> &pairs,
            const std::pair<vec3, vec3> &centroids);

    /**
     * Calculate two centroids for two set of points. A centroid is simply an
     * average of position.
     *
     * @param pointsP Vector of points corresponding to the first mesh.
     * @param pointsQ Vector of points corresponding to the second mesh.
     *
     * @return        A pair of centroids, each corresponding to appropriate
     *                mesh.
     */
    std::pair<vec3, vec3> __getCentroids(
            const std::vector<vec3> &pointsP,
            const std::vector<vec3> &pointsQ);

    /**
     * The procedure takes mesh1points and join each entry with closest one
     * (in euclidean sense) from mesh2Points. In addition the mean average of
     * all distance is calculated and stored in corresponding reference.
     *
     * @param mesh1points      A vector of points from mesh 1.
     * @param mesh2Points      A vector of poitns from mesh 2.
     * @param averageEuclidean A reference that will be updated to store mean
     *                         average of all paired distances.
     *
     * @return                 A vector of pairs
     */
    std::vector<std::pair<vec3, vec3>> __getPairs(std::vector<vec3> mesh1Points,
                                                  const std::vector<vec3> &mesh2Points,
                                                  float &averageEuclidean,
                                                  int subSamples = -1);
};


#endif

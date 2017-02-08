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

using namespace std;
using namespace Eigen;

struct ICPResults {
    ICPResults(const Matrix3f& R,
               const Vector3f& t,
               const Vector3f& cM1,
               const Vector3f& cM2) : m_R(R),
                                      m_t(t),
                                      m_centroidM1(cM1),
                                      m_centroidM2(cM2) {};
    Matrix3f m_R;
    Vector3f m_t;
    Vector3f m_centroidM1;
    Vector3f m_centroidM2;
};

class ICPAlgorithm {
public:
    ICPAlgorithm(const vector<Vector3f>& mesh1Points,
                 const vector<Vector3f>& mesh2Points);

    /**
     * The procedure calculate ICP based on point-to-point scheme, for meshes
     * provided during the initialization.
     * NOTE: Mesh2 is mapped on Mesh1
     *
     * @return A sequence of intermediate results (rotations, translation) in
     *         form of a vector
     */
    vector<ICPResults> pointToPointsICP();

    /**
     * The procedure calculate one step of ICP algorithm based on
     * point-to-point scheme, for meshes provided during the initialization.
     * NOTE: Mesh2 is mapped on Mesh1
     *
     * @return An intermediate result (rotations, translation).
     *
     */
    ICPResults pointToPointsICPStep();

private:
    vector<Vector3f> m_mesh1Points, m_mesh2Points;

    /**
     * Updates values of provided vertices. Please note that values will be
     * overriden.
     *
     * @param vertices Vector of vertices to update.
     * @param R        Rotation to be applied to vertices.
     * @param t        Translation to be applied to vertices.
     */
    vector<Vector3f>  updateVertices(vector<Vector3f>& vertices, const Matrix3f& R,
                        const Vector3f t);

    Matrix3f calculateMatrixA(const vector<pair<Vector3f, Vector3f>>& pairs);

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
    vector<pair<Vector3f, Vector3f>> getTyldaPairs(
            const vector<pair<Vector3f,Vector3f>> &pairs,
            const pair<Vector3f, Vector3f> &centroids);

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
    pair<Vector3f, Vector3f> getCentroids(const vector<Vector3f> & pointsP,
                                         const vector<Vector3f> & pointsQ);

    /**
     * Classic distance between two points/coordinates.
     *
     * @param v1 First coordinate.
     * @param v2 Second coordinate.
     *
     * @return   Distance in Euclidean metric.
     */
    float euclideanDistance(const Vector3f& v1, const Vector3f& v2);

    /**
     * The procedure takes mesh1points and join each entry with closest one
     * (in euclidean sense) from mesh2Points.
     *
     * @param mesh1points A vector of points from mesh 1.
     * @param mesh2Points A vector of poitns from mesh 2.
     *
     * @return            A vector of pairs
     */
    vector<pair<Vector3f, Vector3f>> getPairs(
            const vector<Vector3f> &mesh1Points,
            const vector<Vector3f> &mesh2Points);

};


#endif

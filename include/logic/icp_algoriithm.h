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

    // NOTE: Q -> P   <=>  M2 -> M1
    vector<ICPResults> pointToPointsICP();

private:
    vector<Vector3f> m_mesh1Points, m_mesh2Points;

    void updateVertices(vector<Vector3f>& vertices, const Matrix3f& R,
                        const Vector3f t);

    Matrix3f calculateMatrixA(const vector<pair<Vector3f, Vector3f>>& pairs);

    /**
     *
     * @param pairs
     * @param centroids
     * @return
     */
    vector<pair<Vector3f, Vector3f>> getTyldaPairs(
            const vector<pair<Vector3f,Vector3f>> &pairs,
            const pair<Vector3f, Vector3f> &centroids);

    /**
     *
     * @param points
     * @return
     */
    pair<Vector3f, Vector3f> getCentroids(const vector<Vector3f> & pointsP,
                                         const vector<Vector3f> & pointsQ);

    /**
     *
     * @param pairs
     * @return
     */
    pair<Vector3f, Vector3f> getCentroids(
            const vector<pair<Vector3f, Vector3f>>& pairs);

    /**
     * Classic distance between points.
     *
     * @param v1
     * @param v2
     * @return
     */
    float euclideanDistance(const Vector3f& v1, const Vector3f& v2);

    /**
     *
     * @param mesh1points
     * @param mesh2Points
     * @return
     */
    vector<pair<Vector3f, Vector3f>> getPairs(const vector<Vector3f>& mesh1points,
                                              const vector<Vector3f> & mesh2Points);

};


#endif

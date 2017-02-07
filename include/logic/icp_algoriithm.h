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
    Matrix3f R;
    Vector3f t;
    Vector3f centroidM1;
    Vector3f centroidM2;
};

class ICPAlgorithm {
public:
    ICPAlgorithm(const vector<Vector3f>& mesh1Points,
                 const vector<Vector3f>& mesh2Points);

    vector<ICPResults> pointToPointsICP();

private:
    vector<Vector3f> m_mesh1Points, m_mesh2Points;

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
    pair<Vector3f, Vector3f> getCentroid(const vector<Vector3f> & pointsP,
                                         const vector<Vector3f> & pointsQ);

    /**
     * Classic distance between points.
     *
     * @param v1
     * @param v2
     * @return
     */
    float euclideanDistance(const Vector3f& v1, const Vector3f& v2);

    /**
     * Matches points of M1 to closest points of M2 and return the pairs.
     *
     * @return
     */
    vector<pair<Vector3f, Vector3f>> getPairs();

};


#endif

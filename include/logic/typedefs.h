//==============================================================================
// TODO
//==============================================================================
// author: dybisz
//------------------------------------------------------------------------------

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#ifdef Success
#undef Success
#endif

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
#include <iostream>

namespace Application {
    typedef Eigen::Vector3f vec3;
    typedef Eigen::Vector4f vec4;
    typedef Eigen::Matrix3f mat3;
    typedef Eigen::Matrix4f mat4;
    typedef std::string t_string;
}

#endif

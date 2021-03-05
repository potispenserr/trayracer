// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#pragma once
#include "vec3.h"

//------------------------------------------------------------------------------
/**
*/
class Ray
{
public:
    Ray(vec3 startpoint, vec3 dir) :
        b(startpoint),
        m(dir)
    {

    }

    ~Ray()
    {

    }

    vec3 PointAt(float t)
    {
        return {b + m * t};
    }

    // beginning of ray
    vec3 b;
    // magnitude and direction of ray
    vec3 m;
};

// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#pragma once
#include "object.h"
#include <stdlib.h>
#include <time.h>
#include "mat4.h"
#include "pbr.h"
#include "random.h"
#include "ray.h"
#include "material.h"

// returns a random point on the surface of a unit sphere
inline vec3 random_point_on_unit_sphere()
{
    float x = RandomFloatNTP();
    float y = RandomFloatNTP();
    float z = RandomFloatNTP();
    vec3 v( x, y, z );
    return normalize(v);
}

// a spherical object
class Sphere : public Object
{
public:
    float radius;
    vec3 center;
    Material material;

    Sphere(float radius, vec3 center, Material& material) : 
        radius(radius),
        center(center),
        material(material)
    {

    }

    ~Sphere() override
    {
    
    }

    Color GetColor()
    {
        return material.color;
    }

    bool Intersect(HitResult& hit, Ray ray, float maxDist) override
    {
        vec3 oc = ray.b - this->center;
        vec3 dir = ray.m;
        float b = dot(oc, dir);
    
        // early out if sphere is "behind" ray
        if (b > 0)
            return false;

        float a = dot(dir, dir);
        float c = dot(oc, oc) - this->radius * this->radius;
        float discriminant = b * b - a * c;

        if (discriminant > 0)
        {
            constexpr float minDist = 0.001f;
            float div = 1.0f / a;
            float sqrtDisc = sqrt(discriminant);
            float temp = (-b - sqrtDisc) * div;
            float temp2 = (-b + sqrtDisc) * div;

            if (temp < maxDist && temp > minDist)
            {
                vec3 p = ray.PointAt(temp);
                hit.p = p;
                hit.normal = (p - this->center) * (1.0f / this->radius);
                hit.t = temp;
                return true;
            }
            if (temp2 < maxDist && temp2 > minDist)
            {
                vec3 p = ray.PointAt(temp2);
                hit.p = p;
                hit.normal = (p - this->center) * (1.0f / this->radius);
                hit.t = temp2;
                return true;
            }
        }

        return false;
    }

    Ray ScatterRay(Ray ray, vec3 point, vec3 normal) override
    {
        return BSDF(material, ray, point, normal);
    }

};

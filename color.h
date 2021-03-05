// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#pragma once

struct Color
{
    float r = 0;
    float g = 0;
    float b = 0;

    void operator+=(Color const& rhs)
    {
        this->r += rhs.r;
        this->g += rhs.g;
        this->b += rhs.b;
    }

    Color operator+(Color const& rhs)
    {
        return {this->r + rhs.r,
                this->g + rhs.g,
                this->b + rhs.b};
    }

    Color operator*(Color const& rhs)
    {
        return {this->r * rhs.r,
                this->g * rhs.g,
                this->b * rhs.b};
    }
};

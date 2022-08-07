#ifndef MYTYPES__H_
#define MYTYPES__H_
#include <cstdint>
#include <vector>
typedef float float32_t;
typedef double float64_t;
typedef struct
{
    float64_t x;
    float64_t y;
} float64Point2D_t;
typedef struct
{
    uint32_t x;
    uint32_t y;
} IntPoint2D_t;

#endif
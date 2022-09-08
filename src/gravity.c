#include <stdint.h>
typedef float  f32;
typedef double f64;

// Our G is G * 10**9 * 2
#define SCALED_G_CONST (0.006674*2)

typedef struct {
    f64 pos_x;
    f64 pos_y;
    f64 vel_x;
    f64 vel_y;
    uint8_t radius;
    f64 mass;
} Body;

f64 forceBetween(f64 distance, f64 mass_1, f64 mass_2) {
    return SCALED_G_CONST * mass_1 * mass_2 / (distance * distance);
}

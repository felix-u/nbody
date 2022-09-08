#include <stdint.h>
typedef float  f32;
typedef double f64;

// Our G is G * 10**8 * 2
#define SCALED_G_CONST (0.006674*2)

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} RGB;

typedef struct {
    f64 pos_x;
    f64 pos_y;
    f64 vel_x;
    f64 vel_y;
    uint8_t radius;
    f64 mass;
    RGB clr;
} Body;

f64 forceBetween(f64 distance, f64 mass_1, f64 mass_2) {
    return SCALED_G_CONST * mass_1 * mass_2 / (distance * distance);
}

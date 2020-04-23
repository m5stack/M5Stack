#include <Arduino.h>

float get_point(float *p, uint8_t rows, uint8_t cols, int8_t x, int8_t y);
void set_point(float *p, uint8_t rows, uint8_t cols, int8_t x, int8_t y, float f);
void get_adjacents_1d(float *src, float *dest, uint8_t rows, uint8_t cols, int8_t x, int8_t y);
void get_adjacents_2d(float *src, float *dest, uint8_t rows, uint8_t cols, int8_t x, int8_t y);
float cubicInterpolate(float p[], float x);
float bicubicInterpolate(float p[], float x, float y);
void interpolate_image(float *src, uint8_t src_rows, uint8_t src_cols, 
                       float *dest, uint8_t dest_rows, uint8_t dest_cols);


float get_point(float *p, uint8_t rows, uint8_t cols, int8_t x, int8_t y) {
  if (x < 0)        x = 0;
  if (y < 0)        y = 0;
  if (x >= cols)    x = cols - 1;
  if (y >= rows)    y = rows - 1;
  return p[y * cols + x];
}

void set_point(float *p, uint8_t rows, uint8_t cols, int8_t x, int8_t y, float f) {
  if ((x < 0) || (x >= cols)) return;
  if ((y < 0) || (y >= rows)) return;
  p[y * cols + x] = f;
}

// src is a grid src_rows * src_cols
// dest is a pre-allocated grid, dest_rows*dest_cols
void interpolate_image(float *src, uint8_t src_rows, uint8_t src_cols, 
                       float *dest, uint8_t dest_rows, uint8_t dest_cols) {
  float mu_x = (src_cols - 1.0) / (dest_cols - 1.0);
  float mu_y = (src_rows - 1.0) / (dest_rows - 1.0);

  float adj_2d[16]; // matrix for storing adjacents
 
  
  for (uint8_t y_idx=0; y_idx < dest_rows; y_idx++) {
    for (uint8_t x_idx=0; x_idx < dest_cols; x_idx++) {
       float x = x_idx * mu_x;
       float y = y_idx * mu_y;
       //Serial.print("("); Serial.print(y_idx); Serial.print(", "); Serial.print(x_idx); Serial.print(") = ");
       //Serial.print("("); Serial.print(y); Serial.print(", "); Serial.print(x); Serial.print(") = ");
       get_adjacents_2d(src, adj_2d, src_rows, src_cols, x, y);
       /*
       Serial.print("[");
       for (uint8_t i=0; i<16; i++) {
         Serial.print(adj_2d[i]); Serial.print(", ");
       }
       Serial.println("]");
       */
       float frac_x = x - (int)x; // we only need the ~delta~ between the points
       float frac_y = y - (int)y; // we only need the ~delta~ between the points
       float out = bicubicInterpolate(adj_2d, frac_x, frac_y);
       //Serial.print("\tInterp: "); Serial.println(out);
       set_point(dest, dest_rows, dest_cols, x_idx, y_idx, out);
    }
  }
}

// p is a list of 4 points, 2 to the left, 2 to the right
float cubicInterpolate(float p[], float x) {
    float r = p[1] + (0.5 * x * (p[2] - p[0] + x*(2.0*p[0] - 5.0*p[1] + 4.0*p[2] - p[3] + x*(3.0*(p[1] - p[2]) + p[3] - p[0]))));
  /*
    Serial.print("interpolating: ["); 
    Serial.print(p[0],2); Serial.print(", ");
    Serial.print(p[1],2); Serial.print(", ");
    Serial.print(p[2],2); Serial.print(", ");
    Serial.print(p[3],2); Serial.print("] w/"); Serial.print(x); Serial.print(" = ");
    Serial.println(r);
  */
    return r;
}

// p is a 16-point 4x4 array of the 2 rows & columns left/right/above/below
float bicubicInterpolate(float p[], float x, float y) {
    float arr[4] = {0,0,0,0};
    arr[0] = cubicInterpolate(p+0, x);
    arr[1] = cubicInterpolate(p+4, x);
    arr[2] = cubicInterpolate(p+8, x);
    arr[3] = cubicInterpolate(p+12, x);
    return cubicInterpolate(arr, y);
}

// src is rows*cols and dest is a 4-point array passed in already allocated!
void get_adjacents_1d(float *src, float *dest, uint8_t rows, uint8_t cols, int8_t x, int8_t y) {
    //Serial.print("("); Serial.print(x); Serial.print(", "); Serial.print(y); Serial.println(")");
    // pick two items to the left
    dest[0] = get_point(src, rows, cols, x-1, y);
    dest[1] = get_point(src, rows, cols, x, y);
    // pick two items to the right
    dest[2] = get_point(src, rows, cols, x+1, y);
    dest[3] = get_point(src, rows, cols, x+2, y);
}


// src is rows*cols and dest is a 16-point array passed in already allocated!
void get_adjacents_2d(float *src, float *dest, uint8_t rows, uint8_t cols, int8_t x, int8_t y) {
    //Serial.print("("); Serial.print(x); Serial.print(", "); Serial.print(y); Serial.println(")");
    float arr[4];
    for (int8_t delta_y = -1; delta_y < 3; delta_y++) { // -1, 0, 1, 2
        float *row = dest + 4 * (delta_y+1); // index into each chunk of 4
        for (int8_t delta_x = -1; delta_x < 3; delta_x++) { // -1, 0, 1, 2
            row[delta_x+1] = get_point(src, rows, cols, x+delta_x, y+delta_y);
        }
    }
}

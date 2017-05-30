/*
#   CMath
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include "cmath.h"
#include <tgmath.h>

/*
* 2d vectors
*
* Use the `vec2_new()` function to create vectors. All other vector functions start
* with the `v2_` prefix.
*/
vec2 vec2_new(float x, float y) {
  return (vec2){x,y};
}
vec2 v2_add(vec2 a, vec2 b)          {
  return (vec2){ a.x + b.x, a.y + b.y };
}
vec2 v2_adds(vec2 a, float s)           {
  return (vec2){ a.x + s,   a.y + s};
}
vec2 v2_sub(vec2 a, vec2 b)          {
  return (vec2){ a.x - b.x, a.y - b.y};
}
vec2 v2_subs(vec2 a, float s)           {
  return (vec2){ a.x - s,   a.y - s   };
}
vec2 v2_mul(vec2 a, vec2 b)          {
  return (vec2){ a.x * b.x, a.y * b.y };
}
vec2 v2_muls(vec2 a, float s)           {
  return (vec2){ a.x * s,   a.y * s  };
}
vec2 v2_div(vec2 a, vec2 b)          {
  return (vec2){ a.x / b.x, a.y / b.y };
}
vec2 v2_divs(vec2 a, float s)           {
  return (vec2){ a.x / s,   a.y / s   };
}

float v2_length(vec2 v)                    {
  return sqrtf(v.x*v.x + v.y*v.y);
}

vec2 v2_norm   (vec2 v) {
  float len = v2_length(v);
  if (len > 0)
    return (vec2){ v.x / len, v.y / len };
  else
    return (vec2){ 0, 0};
}

float v2_dot(vec2 a, vec2 b)          {
  return a.x*b.x + a.y*b.y;
}


/*
* 3d vectors
*
* Use the `vec3_new()` function to create vectors. All other vector functions start
* with the `v3_` prefix.
*/
vec3 vec3_new(float x, float y, float z) {
  return (vec3){x,y,z};
}
vec3 vec3_newv(vec3 a) {
  vec3 b;
  a.x = b.x;
  a.y = b.y;
  a.z = b.z;
  return b;
}
vec3 v3_add(vec3 a, vec3 b)          {
  return (vec3){ a.x + b.x, a.y + b.y, a.z + b.z };
}
vec3 v3_adds(vec3 a, float s)           {
  return (vec3){ a.x + s,   a.y + s,   a.z + s   };
}
vec3 v3_sub(vec3 a, vec3 b)          {
  return (vec3){ a.x - b.x, a.y - b.y, a.z - b.z };
}
vec3 v3_subs(vec3 a, float s)           {
  return (vec3){ a.x - s,   a.y - s,   a.z - s   };
}
vec3 v3_mul(vec3 a, vec3 b)          {
  return (vec3){ a.x * b.x, a.y * b.y, a.z * b.z };
}
vec3 v3_muls(vec3 a, float s)           {
  return (vec3){ a.x * s,   a.y * s,   a.z * s   };
}
vec3 v3_div(vec3 a, vec3 b)          {
  return (vec3){ a.x / b.x, a.y / b.y, a.z / b.z };
}
vec3 v3_divs(vec3 a, float s)           {
  return (vec3){ a.x / s,   a.y / s,   a.z / s   };
}
float v3_length(vec3 v)                    {
  return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
}

/*
 * The dot product in simple terms is : A float that represents how much the two vectors
 * are moving together based on angle.
 * If A * B = 0 => A,B are perpendicular ' _|_ ' (90 degree)
 * If A * B = 1 => A,B are colliniar ' = '
 *
 * Note:
 * If the dot product is less than 90 degrees => dot product will be positive
 * If the dot product is greater than 90 degrees => dot product will be negative
 *
 * Another way of representing the dot product is:
 * "That is to say, the dot product of two vectors will be equal to the cosine of
 * the angle between the vectors, times the lengths of each of the vectors."
 * A · B = |A| * |B| * cos(Θ)
 *
 * Source:
 * http://www.mvps.org/DirectX/articles/math/dot/index.htm
 */
float v3_dot(vec3 a, vec3 b)          {
  return a.x*b.x + a.y*b.y + a.z*b.z;
}

/*
 * Returns a third vector which is perpendicular on A, B
 *
 * Another way of representing the cross product is:
 * |A| * |B| * sin(Θ)
 */
vec3 v3_cross(vec3 a, vec3 b) {
  return (vec3){
      a.y * b.z - a.z * b.y,
          a.z * b.x - a.x * b.z,
          a.x * b.y - a.y * b.x
    };
}

vec3 v3_norm(vec3 v) {
  float len = v3_length(v);
  if (len > 0)
    return (vec3){ v.x / len, v.y / len, v.z / len };
  else
    return (vec3){ 0, 0, 0};
}

vec3 v3_proj(vec3 v, vec3 onto) {
  return v3_muls(onto, v3_dot(v, onto) / v3_dot(onto, onto));
}


float v3_angle_between(vec3 a, vec3 b) {
  return acosf( v3_dot(a, b) / (v3_length(a) * v3_length(b)) );
}

// -- [[ END OF VEC3 ]] -- \\

mat4x4 m4x4_new(float m00, float m01, float m02, float m03,
                float m10, float m11, float m12, float m13,
                float m20, float m21, float m22, float m23,
                float m30, float m31, float m32, float m33)
{
  return (mat4x4){
      m00, m01, m02, m03,
          m10, m11, m12, m13,
          m20, m21, m22, m23,
          m30, m31, m32, m33,
    };
}

void m4x4_set   (mat4x4* out,
                 float m00, float m01, float m02, float m03,
                 float m10, float m11, float m12, float m13,
                 float m20, float m21, float m22, float m23,
                 float m30, float m31, float m32, float m33) {
  out->m[0][0] = m00;
  out->m[0][1] = m01;
  out->m[0][2] = m02;
  out->m[0][3] = m03;
  out->m[1][0] = m10;
  out->m[1][1] = m11;
  out->m[1][2] = m12;
  out->m[1][3] = m13;
  out->m[2][0] = m20;
  out->m[2][1] = m21;
  out->m[2][2] = m22;
  out->m[2][3] = m23;
  out->m[3][0] = m30;
  out->m[3][1] = m31;
  out->m[3][2] = m32;
  out->m[3][3] = m33;
}

/*
 * Creates an orthographic projection matrix. It maps the right handed cube
 * defined by left, right, bottom, top, back and front onto the screen and
 * z-buffer. You can think of it as a cube you move through world or camera
 * space and everything inside is visible.
 *
 * The arguments are ordered in a way that for each axis you specify the minimum
 * followed by the maximum. Thats why it's bottom to top and back to front.
 *
 * Implementation details:
 *
 * To be more exact the right-handed cube is mapped into normalized device
 * coordinates, a left-handed cube where (-1 -1) is the lower left corner,
 * (1, 1) the upper right corner and a z-value of -1 is the nearest point and
 * 1 the furthest point. OpenGL takes it from there and puts it on the screen
 * and into the z-buffer.
 *
 * Sources:
 * https://msdn.microsoft.com/en-us/library/windows/desktop/dd373965(v=vs.85).aspx
 * https://unspecified.wordpress.com/2012/06/21/calculating-the-gluperspective-matrix-and-other-opengl-matrix-maths/
 */
void m4x4_newOrtho(mat4x4* projectionMatrix, float left, float right, float bottom, float top, float zNear, float zFar) {
  float x_orth = 2.0f / (right - left);
  float y_orth = 2.0f / (top - bottom);
  float z_orth = -2.0f / (zNear - zFar);

  float tx = -(right + left) / (right - left);
  float ty = -(top + bottom) / (top - bottom);
  float tz = (zNear + zFar) / (zNear - zFar);

  projectionMatrix->m[0][0] = x_orth;
  projectionMatrix->m[0][1] = 0.0f;
  projectionMatrix->m[0][2] = 0.0f;
  projectionMatrix->m[0][3] = 0.0f;

  projectionMatrix->m[1][0] = 0.0f;
  projectionMatrix->m[1][1] = y_orth;
  projectionMatrix->m[1][2] = 0.0f;
  projectionMatrix->m[1][3] = 0.0f;

  projectionMatrix->m[2][0] = 0.0f;
  projectionMatrix->m[2][1] = 0.0f;
  projectionMatrix->m[2][2] = z_orth;
  projectionMatrix->m[2][3] = 0.0f;

  projectionMatrix->m[3][0] = tx;
  projectionMatrix->m[3][1] = ty;
  projectionMatrix->m[3][2] = tz;
  projectionMatrix->m[3][3] = 1.0f;
}
/*
 * Creates a perspective projection matrix for a camera.
 *
 * The camera is at the origin and looks in the direction of the negative Z axis.
 * `near_view_distance` and `far_view_distance` have to be positive and > 0.
 * They are distances from the camera eye, not values on an axis.
 *
 * `near_view_distance` can be small but not 0. 0 breaks the projection and
 * everything ends up at the max value (far end) of the z-buffer. Making the
 * z-buffer useless.
 *
 * The matrix is the same as `gluPerspective()` builds. The view distance is
 * mapped to the z-buffer with a reciprocal function (1/x). Therefore the z-buffer
 * resolution for near objects is very good while resolution for far objects is
 * limited.
 *
 * Sources:
 * https://unspecified.wordpress.com/2012/06/21/calculating-the-gluperspective-matrix-and-other-opengl-matrix-maths/
 */
void m4x4_newPerspective(mat4x4* projectionMatrix, float fov, float ratio, float zNear, float zFar) {
  float ymax, xmax;
  ymax = zNear * tanf((float)(fov * M_PI / 360.0f));
  xmax = ymax * ratio;

  float left = -xmax;
  float right = xmax;
  float bottom = -ymax;
  float top = ymax;

  float temp, temp2, temp3, temp4;
  temp = 2.0f * zNear;
  temp2 = right - left;
  temp3 = top - bottom;
  temp4 = zFar - zNear;

  projectionMatrix->m[0][0] = temp / temp2;
  projectionMatrix->m[0][1] = 0.0f;
  projectionMatrix->m[0][2] = 0.0f;
  projectionMatrix->m[0][3] = 0.0f;

  projectionMatrix->m[1][0] = 0.0f;
  projectionMatrix->m[1][1] = temp / temp3;
  projectionMatrix->m[1][2] = 0.0f;
  projectionMatrix->m[1][3] = 0.0f;

  projectionMatrix->m[2][0] = (right + left) / temp2;
  projectionMatrix->m[2][1] = (top + bottom) / temp3;
  projectionMatrix->m[2][2] = (-zFar - zNear) / temp4;
  projectionMatrix->m[2][3] = -1.0f;

  projectionMatrix->m[3][0] = 0.0f;
  projectionMatrix->m[3][1] = 0.0f;
  projectionMatrix->m[3][2] = (-temp * zFar) / temp4;
  projectionMatrix->m[3][3] = 1.0f;
}
/*
 * Builds a transformation matrix for a camera that looks from `from` towards
 * `to`. `up` defines the direction that's upwards for the camera. All three
 * vectors are given in world space and `up` doesn't need to be normalized.
 *
 * Implementation details:
 *
 * x, y and z are the right-handed base vectors of the cameras subspace.
 * x has to be normalized because the cross product only produces a normalized
 *   output vector if both input vectors are orthogonal to each other. And up
 *   probably isn't orthogonal to z.
 *
 * These vectors are then used to build a 3x3 rotation matrix. This matrix
 * rotates a vector by the same amount the camera is rotated. But instead we
 * need to rotate all incoming vertices backwards by that amount. That's what a
 * camera matrix is for: To move the world so that the camera is in the origin.
 * So we take the inverse of that rotation matrix and in case of an rotation
 * matrix this is just the transposed matrix. That's why the 3x3 part of the
 * matrix are the x, y and z vectors but written horizontally instead of
 * vertically.
 *
 * The translation is derived by creating a translation matrix to move the world
 * into the origin (thats translate by minus `from`). The complete lookat matrix
 * is then this translation followed by the rotation. Written as matrix
 * multiplication:
 *
 *   lookat = rotation * translation
 *
 * Since we're right-handed this equals to first doing the translation and after
 * that doing the rotation. During that multiplication the rotation 3x3 part
 * doesn't change but the translation vector is multiplied with each rotation
 * axis. The dot product is just a more compact way to write the actual
 * multiplications.
 */
void m4x4_newLookAt(mat4x4* in, vec3 pos, vec3 target, vec3 up) {
  vec3 z_a = v3_norm(v3_sub(target, pos));
  vec3 x_a = v3_norm(v3_cross(up,    z_a));
  vec3 y_a = v3_cross(z_a, x_a);

  return m4x4_set(in,
                  x_a.x,           y_a.x,          z_a.x,          0,
                  x_a.y,           y_a.y,           z_a.y,        0,
                  x_a.z,           y_a.z,           z_a.z,        0,
                  -v3_dot(x_a, pos),  -v3_dot(y_a, pos),  -v3_dot(z_a, pos),  1);
}


void m4x4_newRotationScale(mat4x4 *out, float a, vec3 axis, float s_x, float s_y, float s_z) {
  float a_x = axis.x, a_y = axis.y, a_z = axis.z;
  float c = cos(a), s = sin(a);

  m4x4_set( out,
            s_x * (c + a_x*a_x*(1-c)),       a_x*a_y*(1-c) - a_z*s,               0,                             0,
            a_y*a_x*(1-c) + a_z*s,           s_y * (c + a_y*a_y*(1-c)),     a_x*a_z*(1-c) + a_y*s,               0,
            a_z*a_x*(1-c) - a_y*s,           a_z*a_y*(1-c)+a_x*s,           a_y*a_y*(1-c) - a_x*s,               0,
            0,                                      0,                      s_z * (c + a_z*a_z*(1-c)),           1
            );

}

/*
 * Creates a matrix to rotate around an axis by a given angle. The axis doesn't
 * need to be normalized.
 *
 * Sources:
 * https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
 */
void m4x4_newRotation(mat4x4 *out, float a, vec3 axis) {
  float x = axis.x, y = axis.y, z = axis.z;
  float c = cos(a), s = sin(a);

  out->m[0][0] = c + x*x*(1-c);
  out->m[0][1] = y*x*(1-c) + z*s;
  out->m[0][2] = z*x*(1-c) - y*s;
  out->m[0][3] = 0.0f;
  out->m[1][0] = x*y*(1-c) - z*s;
  out->m[1][1] = c + y*y*(1-c);
  out->m[1][2] = z*y*(1-c) + x*s;
  out->m[1][3] = 0.0f;
  out->m[2][0] = 0.0f;
  out->m[2][1] = x*z*(1-c) + y*s;
  out->m[2][2] = y*z*(1-c) - x*s;
  out->m[2][3] = c + z*z*(1-c);
  out->m[3][0] = 0.0f;
  out->m[3][1] = 0.0f;
  out->m[3][2] = 0.0f;
  out->m[3][3] = 1.0f;
}

void m4x4_newIdentity(mat4x4 *out) {
  m4x4_set(out,
           1,  0,  0,  0,
           0,  1,  0,  0,
           0,  0,  1,  0,
           0,  0,  0,  1
           );
}

void m4x4_newScaling(mat4x4 *out, float x, float y, float z) {
  m4x4_set(out,
           x,  0,  0,  0,
           0,  y,  0,  0,
           0,  0,  z,  0,
           0,  0,  0,  1
           );
}

void m4x4_newTranslation(mat4x4 *out, float x, float y, float z) {
  m4x4_set(out,
           1,  0,  0,  x,
           0,  1,  0,  y,
           0,  0,  1,  z,
           0,  0,  0,  1
           );
}

void m4x4_newRotationX(mat4x4 *out, float a) {
  float c = cos(a);
  float s = sin(a);

  m4x4_set(out,
           1,  0,  0,  0,
           0,  c, -s,  0,
           0,  s,  c,  0,
           0,  0,  0,  1
           );
}

void m4x4_newRotationY(mat4x4 *out, float a) {
  float c = cos(a);
  float s = sin(a);

  m4x4_set(out,
           c,  0,  s,  0,
           0,  1,  0,  0,
           -s,  0,  c,  0,
           0,  0,  0,  1
           );
}

void m4x4_newRotationZ(mat4x4 *out, float a) {
  float c = cos(a);
  float s = sin(a);

  m4x4_set(out,
           c, -s,  0,  0,
           s,  c,  0,  0,
           0,  0,  1,  0,
           0,  0,  0,  1
           );
}

void m4x4_mulM4x4(mat4x4 *out, mat4x4 const* a, mat4x4 const* b) {
  for(int i = 0; i < 4; i++) {
      for(int j = 0; j < 4; j++) {
          float sum = 0;
          for(int k = 0; k < 4; k++) {
              sum += a->m[k][j] * b->m[i][k];
            }
          out->m[i][j] = sum;
        }
    }
}

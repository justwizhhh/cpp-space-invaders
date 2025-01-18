#ifndef VECTOR2_H
#define VECTOR2_H


struct Vector2
{
  // construction
  Vector2(float x_ = 0.0, float y_ = 0.0);
  Vector2(const Vector2& rhs);

  // operations
  void set(float new_x, float new_y);
  Vector2 operator*(float scalar);
  Vector2& operator=(const Vector2& rhs);
  void normalise();

  // data
  float x = 0;
  float y = 0;
};

#endif // VECTOR2_H
#ifndef TYPES_H
#define TYPES_H

struct button
{
    bool state;
    int last_down;
    int last_up;
};
struct MouseButton
{
    bool state;
    int last_down;
    int last_up;
    int x;
    int y;
};
struct type_Mouse
{
    MouseButton left;
    MouseButton right;
    MouseButton middle;
    bool WheelUp;
    bool WheelDown;
    int x;
    int y;
};

struct Vector_i{ int x, y, z; };
struct Vector_f{ float x, y, z; };

struct pixel{ unsigned char r, g, b; };

struct Color_c{ unsigned char r, g, b, a; };
struct Color_f{ float r, g, b, a; };

struct Rect_i{ int x, y, w, h; };
struct Rect_f{ float x, y, w, h; };

struct Circle_i{ int x, y, r, d; };
struct Circle_f{ float x, y, r, d; };

struct Angle_i{ int p, y, r; };
struct Angle_f{ float p, y, r; };

struct Frame
{
    Rect_f  image;
    Rect_f  uv;
};
struct Animframe
{
    int frame;
    Rect_f pos;
    float scale;
    float rotation;
    Rect_f uv;
};

#endif

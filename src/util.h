#include <vector>

struct vec3{
    float x,y,z;
};

struct triangle{
    vec3 points[3];
    float light;

    triangle(vec3 ps[], float l){
        points[0] = ps[0];
        points[1] = ps[1];
        points[2] = ps[2];

        light = l;
    }
};

struct line{
    float x1, x2, y1, y2;

    line(float a, float b, float c, float d){
        x1 = a;
        y1 = b;
        x2 = c;
        y2 = d;
    }
};

struct face {
    int a, b, c;
};

struct mesh
{
	std::vector<vec3> vertecis;
    
    std::vector<face> faces;
};

struct mat4x4
{
	float m[4][4] = { 0 };
};

void MultiplyMatrixVector(vec3 &i, vec3 &o, mat4x4 &m)
{
    o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
    o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
    o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
    float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

    if (w != 0.0f)
    {
        o.x /= w; o.y /= w; o.z /= w;
    }
}
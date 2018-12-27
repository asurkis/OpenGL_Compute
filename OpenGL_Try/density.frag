#version 430

#define IMAGE_SIZE 400
#define PARTICLE_COUNT (1 << 15)
#define DIVISOR 1.0f

out vec3 color;

uniform vec2 windowSize;

layout(std430, binding = 0) buffer Pos {
    vec2 m[];
} pos;

layout (std430, binding = 2) buffer Cnt {
    uint m[IMAGE_SIZE][IMAGE_SIZE];
} cnt;

void main()
{
    vec2 pos = IMAGE_SIZE * ((gl_FragCoord.xy - 0.5f * windowSize) / min(windowSize.x, windowSize.y) + vec2(0.5));

    uvec2 p1 = uvec2(pos);
    uvec2 p2 = uvec2(ceil(pos.x), ceil(pos.y));

    vec2 k = pos - p1;
    uint c11 = cnt.m[p1.x][p1.y];
    uint c12 = cnt.m[p1.x][p2.y];
    uint c21 = cnt.m[p2.x][p1.y];
    uint c22 = cnt.m[p2.x][p2.y];

    float d = (1 - k.y) * ((1 - k.x) * c11 + k.x * c21)
    		+ k.y * ((1 - k.x) * c12 + k.x * c22);
    color = vec3(0, d / DIVISOR, d / DIVISOR);

    // vec2 p = 2 * (gl_FragCoord.xy - 0.5f * windowSize) / max(windowSize.x, windowSize.y);
    //
    // float sum = 0;
    // uint i;
    // for (i = 0; i < PARTICLE_COUNT; i++)
    // {
    //     vec2 diff = p - pos.m[i];
    //     float len = max(0.01f, length(diff));
    //     // float len2 = diff.x * diff.x + diff.y * diff.y;
    //     if (len < 0.1f)
    //         sum += 1e-4f / len;
    //     // sum += 0.001f;
    // }
    //
    // color = vec3(0, 1, 1) * sum; // * (1e+3f / sum);
}

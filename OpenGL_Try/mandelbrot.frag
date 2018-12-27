#version 430 core

out vec3 color;

uniform vec2 windowSize;

void main()
{
    vec2 c = 2 * gl_FragCoord.xy / windowSize - vec2(1);
    c *= 2;

    vec2 z = vec2(0.0, 0.0);
    float i;
    for (i = 0.0; i < 1.0; i += 0.005) {
        z = vec2(
            z.x * z.x - z.y * z.y + c.x,
            z.y * z.x + z.x * z.y + c.y
        );

        if (length(z) > 4.0) {
            break;
        }
    }

    color = vec3(i);
}
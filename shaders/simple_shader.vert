#version 450

vec2 positions[6] = vec2[] (
    vec2(-.2, .3),
    vec2(0, -.3),
    vec2(.2,.3),

    vec2(.2, -.3),
    vec2(.4, -.6),
    vec2(.6,-.3)
);

void main() {
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
}
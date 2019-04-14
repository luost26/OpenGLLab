#version 410 core

layout (triangles) in;
layout (line_strip, max_vertices=6) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

void GenerateLine(int index) {
    gl_Position = gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = gl_in[index].gl_Position + vec4(normalize(gs_in[index].normal), 0.0)*0.05;
    EmitVertex();
    EndPrimitive();
    
}

void main() {
    GenerateLine(0);
    GenerateLine(1);
    GenerateLine(2);
}

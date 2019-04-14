#version 410 core

layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in VS_OUT {
    vec3 color;
} gs_in[];

out vec3 fColor;

void main() {
    fColor = gs_in[0].color;
    
    gl_Position = gl_in[0].gl_Position + vec4(-0.1, -0.1, 0.0, 0.0);
    gl_PointSize = 10.0;
    EmitVertex();
    
    gl_Position = gl_in[0].gl_Position + vec4(0.1, -0.1, 0.0, 0.0);
    gl_PointSize = 10.0;
    EmitVertex();
    
    gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.1, 0.0, 0.0);
    gl_PointSize = 10.0;
    EmitVertex();
    
    gl_Position = gl_in[0].gl_Position + vec4(0.1, 0.1, 0.0, 0.0);
    gl_PointSize = 10.0;
    EmitVertex();
    
    fColor = vec3(1.0,1.0,1.0);
    
    gl_Position = gl_in[0].gl_Position + vec4(0, 0.15, 0.0, 0.0);
    gl_PointSize = 10.0;
    EmitVertex();
    
    EndPrimitive();
}

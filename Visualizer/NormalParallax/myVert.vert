#version 330 core

// VBO-ból érkező változók
in vec3 vs_in_pos;
in vec3 vs_in_norm;
in vec2 vs_in_tex;
in vec3 vs_in_tan;
in vec3 vs_in_bitan;

// A pipeline-ban tovább adandó értékek
out vec3 vs_out_pos;
out vec3 vs_out_norm;
out vec2 vs_out_tex;
out vec3 vs_out_tan;
out vec3 vs_out_bitan;

// Shader külső paraméterei
uniform mat4 MVP;
uniform mat4 world;
uniform mat4 worldIT;

void main()
{
    gl_Position = MVP * vec4(vs_in_pos, 1);
    
    vs_out_pos = (world * vec4(vs_in_pos, 1)).xyz;
    vs_out_norm = (world * vec4(vs_in_norm, 0)).xyz;
    vs_out_tex = vs_in_tex;
    vs_out_tan = (world * vec4(vs_in_tan, 0)).xyz;
    vs_out_bitan = (world * vec4(vs_in_bitan, 0)).xyz;
}
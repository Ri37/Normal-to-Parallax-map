#version 330 core

// VBO-ból érkező változók
in vec3 vs_in_pos;
in vec3 vs_in_norm;
in vec2 vs_in_tex;
in vec3 vs_in_tan;
in vec3 vs_in_bitan;

// A pipeline-ban tovább adandó értékek
out VS_OUT {
    vec3 fragPos;
    vec2 texCoords;
    vec3 tanLightPos;
    vec3 tanViewPos;
    vec3 tanFragPos;
} vs_out;

// Shader külső paraméterei
uniform mat4 MVP;
uniform mat4 world;
uniform mat4 worldIT;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    gl_Position = MVP * vec4(vs_in_pos, 1);

    vs_out.fragPos = vec3(world * vec4(vs_in_pos, 1.0));
    vs_out.texCoords = vs_in_tex;

    vec3 T   = normalize(mat3(world) * vs_in_tan);
    vec3 B   = normalize(mat3(world) * vs_in_bitan);
    vec3 N   = normalize(mat3(world) * vs_in_norm);
    mat3 TBN = transpose(mat3(T, B, N));

    vs_out.tanLightPos = TBN * lightPos;
    vs_out.tanViewPos  = TBN * viewPos;
    vs_out.tanFragPos  = TBN * vs_out.fragPos;
}

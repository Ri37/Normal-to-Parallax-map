#version 330 core

// Pipeline-ból bejövő per-fragment attribútumok
in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;
in vec3 vs_out_tan;
in vec3 vs_out_bitan;

// Kimenő érték - a fragment színe
out vec4 fs_out_col;

// Textúra mintavételező objektumok
uniform sampler2D texImage;           // Alaptérkép
uniform sampler2D normalTexImage;     // Normál térkép
uniform sampler2D heightTexImage;     // Magasság térkép

// Fényforrás tulajdonságok
uniform vec4 light_pos;
uniform vec3 La = vec3(0.1);
uniform vec3 Ld = vec3(0.8);

// Anyag tulajdonságok
uniform vec3 Ka = vec3(1.0);
uniform vec3 Kd = vec3(1.0);

// Intenzitás Bump map-hez
uniform float intensity;

vec3 heightToNormal(vec2 texCoords) {
    // Texel offset beállítása
    const ivec2 texelOffsetX = ivec2(1, 0); // egy texel offset vízszintes mintavételért
    const ivec2 texelOffsetY = ivec2(0, 1); // egy texel offset függőleges mintavételért

    // Szomszédos magasságok
    float heightRight  = textureOffset(heightTexImage, texCoords, texelOffsetX).r;
    float heightLeft   = textureOffset(heightTexImage, texCoords, -texelOffsetX).r;
    float heightUp     = textureOffset(heightTexImage, texCoords, texelOffsetY).r;
    float heightDown   = textureOffset(heightTexImage, texCoords, -texelOffsetY).r;

    // Értékek invertálása
    heightRight  = abs(heightRight - 1);
    heightLeft   = abs(heightLeft  - 1);
    heightUp     = abs(heightUp    - 1);
    heightDown   = abs(heightDown  - 1);

    // Gradiens számítása
    float dX = heightRight - heightLeft;
    float dY = heightUp - heightDown;

    // Z komponens beállítása intenzitásért
    vec3 normal = normalize(vec3(dX, dY, intensity)); // imGUI

    return normal;
}

void main()
{
    vec3 T = normalize(vs_out_tan);
    vec3 N = normalize(vs_out_norm);
    vec3 B = normalize(vs_out_bitan);
    mat3 TBN = mat3(T, B, N);

    vec3 normal;
    
    #if defined(USE_NORMAL_MAP)
        normal = heightToNormal(vs_out_tex); // Normal számítása környező magasságokból
    #elif defined(USE_BUMP_MAP)
        normal = texture(normalTexImage, vs_out_tex).xyz;
        normal = 2.0 * normal - 1.0;         // skálázás [0, 1] -> [-1, 1]
    #endif

    normal = normalize(TBN * normal);

    vec3 Ambient = La * Ka;

    vec3 lightDir = light_pos.xyz - vs_out_pos;
    float distance = length(lightDir);
    vec3 ToLight = normalize(lightDir);

    float DiffuseFactor = max(dot(ToLight, normal), 0.1);
    vec3 Diffuse = 10 * DiffuseFactor * Ld * Kd / (distance * distance); // feny gyengülése távolság függvényébe
    
    fs_out_col = vec4(Ambient + Diffuse, 1) * texture(texImage, vs_out_tex);
}
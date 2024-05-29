#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 fragPos;
    vec2 texCoords;
    vec3 tanLightPos;
    vec3 tanViewPos;
    vec3 tanFragPos;
} fs_in;

uniform sampler2D texImage;
uniform sampler2D heightTexImage;

uniform float heightScale = 0.05;

// Fény tulajdonsagai
uniform vec3 La = vec3(0.1);
uniform vec3 Ld = vec3(0.8);

// Anyag tulajdonságok
uniform vec3 Ka = vec3(1.0);
uniform vec3 Kd = vec3(1.0);

vec3 heightToNormal(vec2 texCoords) {
    // Skála
    float scale = 10;

    // Texel offset beállítása
    const ivec2 texelOffsetX = ivec2(1, 0); // Egy texel offset vízszintes mintavételért
    const ivec2 texelOffsetY = ivec2(0, 1); // Egy texel offset függőleges mintavételért

    // Szomszédos magasságok
    float heightRight  = textureOffset(heightTexImage, texCoords, texelOffsetX).r;
    float heightLeft   = textureOffset(heightTexImage, texCoords, -texelOffsetX).r;
    float heightUp     = textureOffset(heightTexImage, texCoords, texelOffsetY).r;
    float heightDown   = textureOffset(heightTexImage, texCoords, -texelOffsetY).r;

    // Gradiens számítása
    float dx = heightRight - heightLeft;
    float dy = heightUp - heightDown;

    vec3 dX = vec3(1, 0, dx * scale);
    vec3 dY = vec3(0, 1, dy * scale);

    // Z komponens beállítása intenzitásért
    vec3 normal = normalize(cross(dX,dY));

    return normal;
}

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
    // mélység rétegek száma
    const float minLayers = 8;
    const float maxLayers = 64;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  
    // réteg méret számítása
    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;
    // mennyivel kell elmozdítani a réteget a kezdeti ponttól
    vec2 P = viewDir.xy / viewDir.z * heightScale; 
    vec2 deltaTexCoords = P / numLayers;
  
    // kezdeti értékek
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = abs(texture(heightTexImage, currentTexCoords).r - 1);
      
    while(currentLayerDepth < currentDepthMapValue)
    {
        currentTexCoords -= deltaTexCoords; // elmozdítás
        currentDepthMapValue = abs(texture(heightTexImage, currentTexCoords).r - 1);  
        currentLayerDepth += layerDepth;  
    }
    
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // lineáris interpoláció
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = abs(texture(heightTexImage, prevTexCoords).r - 1) - currentLayerDepth + layerDepth;
 
    // textúra koordináták interpolálása
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}

void main()
{
    vec3 viewDir = normalize(fs_in.tanViewPos - fs_in.tanFragPos);
    vec2 texCoords = fs_in.texCoords;
    
    texCoords = ParallaxMapping(fs_in.texCoords,  viewDir);       
    if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
        discard;

    vec3 normal = heightToNormal(texCoords);

    vec3 Ambient = La * Ka;

    vec3 lightDir  = fs_in.tanLightPos - fs_in.tanFragPos;
    float distance = length(lightDir);
    vec3 ToLight = normalize(lightDir);

    float DiffuseFactor = max(dot(ToLight, normal), 0.1);
    vec3 Diffuse = 10 * DiffuseFactor * Ld * Kd / (distance * distance); // feny gyengülése távolság függvényébe

    FragColor = vec4(Ambient + Diffuse, 1) * texture(texImage, texCoords);
}
#pragma once

#include<string_view>

static constexpr std::string_view grid_vertex_shader_code = 
"#version 410 core\n"
"\n"
"layout (location = 0) in vec3 aPos;\n"
"\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"}\n";

static constexpr std::string_view grid_fragment_shader_code =
"#version 410 core\n"
"\n"
"out vec4 FragColor;\n"
"\n"
"void main()\n"
"{\n"
"    FragColor = vec4(0.5, 0.5, 0.5, 1.0);\n"
"}\n";

static constexpr std::string_view light_source_vertex_shader_code =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"} \n";

static constexpr std::string_view light_source_fragment_shader_code =
"#version 330 core\n"
"out vec4 FragColor;\n"
"\n"
"uniform vec3 lightColor;\n"
"\n"
"void main()\n"
"{\n"
"    FragColor = vec4(lightColor, 1.0); \n"
"}\n";

static constexpr std::string_view vertex_shader_code = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aNormal;\n"
"layout (location = 2) in vec2 aTexCoords;\n"
"\n"
"out vec3 Normal;\n"
"out vec3 FragPos;\n"
"out vec2 TexCoords;\n"
"\n"
"uniform mat3 normal_model;\n"
"\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"\n"
"uniform float uv_rotation;\n"
"float uniformAngle = 0.5f;\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"    Normal = normal_model * aNormal; \n"
"    FragPos = vec3(model * vec4(aPos, 1.0));\n"
"    TexCoords = aTexCoords;\n"
"} \n";

static constexpr std::string_view fragment_shader_code = 
"#version 330 core\n"
"out vec4 FragColor;\n"

"in vec3 FragPos;\n"
"in vec3 Normal;\n"
"in vec2 TexCoords;\n"

"uniform float time;\n"
"uniform vec3 viewPos;\n"

"struct DirLight {\n"
"    vec3 direction;\n"
"\n"  
"    vec3 ambient;\n"
"    vec3 diffuse;\n"
"    vec3 specular;\n"
"};\n"  
"#define MAX_DIR_LIGHTS 16\n"
"uniform DirLight dirLights[MAX_DIR_LIGHTS];\n"
"uniform int direct_lights;\n"
"\n"
"struct PointLight {\n"
"    vec3 position;\n"
"\n"
"    float constant;\n"
"    float linear;\n"
"    float quadratic;\n"  
"\n"
"    vec3 ambient;\n"
"    vec3 diffuse;\n"
"    vec3 specular;\n"
"};\n"
"#define MAX_POINT_LIGHTS 16\n"
"uniform PointLight pointLights[MAX_POINT_LIGHTS];\n"
"uniform int point_lights;\n"
"\n"
"struct SpotLight {\n"
"    vec3 position;\n"
"    vec3 direction;\n"
"    float cutoff;\n"
"    float outer_cutoff;\n"
"\n"
"    float constant;\n"
"    float linear;\n"
"    float quadratic;\n"  
"\n"
"    vec3 ambient;\n"
"    vec3 diffuse;\n"
"    vec3 specular;\n"
"};\n"
"#define MAX_SPOT_LIGHTS 16\n"
"uniform SpotLight spotLights[MAX_SPOT_LIGHTS];\n"
"uniform int spot_lights;\n"
"\n"
"struct Texture {\n"
"    sampler2D id;\n"
"    int type;\n"
"    float offsetX;\n"
"    float offsetY;\n"
"    float scaleX;\n"
"    float scaleY;\n"
"    float rotation;\n"
"};\n"
"#define MAX_TEXTURES 16\n"
"uniform Texture textures[MAX_TEXTURES];\n"
"uniform int num_textures;\n"
"\n"
"struct Material {\n"
"    float ambient;\n"
"    float diffuse;\n"
"    float specular;\n"
"\n"
"    vec3 color;\n"
"\n"
"    float shininess;\n"
"};\n"
"uniform Material material;\n"
"\n"
"uniform vec3 highlight_color;\n"
"uniform float highlight_value;\n"
"\n"
"vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);\n"
"\n"
"vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);\n"
"\n"
"vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);\n"
"\n"
"vec2 CalcTexCoords(Texture t, vec2 inTexCoords);\n"
"\n"
"void main()\n"
"{\n"
"    // properties\n"
"    vec3 norm = normalize(Normal);\n"
"    vec3 viewDir = normalize(viewPos - FragPos);\n"
"    vec3 result = vec3(0.0);\n"
"\n"
"   // phase 1: Directional lighting\n"
"    for(int i = 0; i < direct_lights; i++)\n"
"        result += CalcDirLight(dirLights[i], norm, viewDir);\n"
"    // phase 2: Point lights\n"
"    for(int i = 0; i < point_lights; i++)\n"
"       result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);\n"
"    // phase 3: Spot light\n"
"    for(int i = 0; i < spot_lights; i++)\n"
"        result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);\n"
"\n"
"    //Emission Light\n"
"    //*\n"
"    for(int i=0; i<num_textures; i++){\n"
"        vec2 tex_coords = CalcTexCoords(textures[i], TexCoords);\n"
"        if(textures[i].type == 2){ //Emission\n"
"            vec4 emission_texture = texture(textures[i].id, tex_coords);\n"
"            if(emission_texture.a > 0.1){\n"
"                result = vec3(emission_texture);\n"
"            }\n"
"        }\n"
"    }\n"
"    //*/\n"
"\n"
"    FragColor = vec4(mix(result, highlight_color, highlight_value), 1.0);\n"
"}\n"
"\n"
"vec2 CalcTexCoords(Texture t, vec2 inTexCoords){\n"
"    vec2 tex_coords = inTexCoords;\n"
"    tex_coords += vec2(-0.5, -0.5); //move to center\n"
"    tex_coords /= vec2(t.scaleX, t.scaleY); //scale\n"
"    mat2 rot_mat = mat2(cos(t.rotation),\n"
"                        sin(t.rotation),\n"
"                       -sin(t.rotation),\n"
"                        cos(t.rotation));\n"
"    tex_coords *= rot_mat; //rotate\n"
"    tex_coords += vec2(0.5, 0.5); //move back\n"
"    tex_coords += vec2(-t.offsetX, t.offsetY) * rot_mat; //move\n"
"    return tex_coords;\n"
"}\n"
"\n"
"vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)\n"
"{\n"
"    vec3 lightDir = normalize(-light.direction);\n"
"\n"
"    // diffuse shading\n"
"    float diff = max(dot(normal, lightDir), 0.0);\n"
"    // specular shading\n"
"    vec3 reflectDir = reflect(-lightDir, normal);\n"
"    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"
"    // combine results\n"
"    vec3 ambient  = light.ambient * material.color;\n"
"    vec3 diffuse  = light.diffuse * diff * material.color;\n"
"    vec3 specular = light.specular * spec * material.specular;\n"
"\n"
"    for(int i=0; i<num_textures; i++){\n"
"        vec2 tex_coords = CalcTexCoords(textures[i], TexCoords);\n"
"        if(textures[i].type == 0){ //Diffuse\n"
"            vec4 diffuse_texture = texture(textures[i].id, tex_coords);\n"
"            if(diffuse_texture.a > 0.1){\n"
"                ambient = light.ambient * vec3(diffuse_texture);\n"
"                diffuse = light.diffuse * diff * vec3(diffuse_texture);\n"
"                specular = light.specular * spec * material.specular;\n"
"            }\n"
"        }\n"
"        if(textures[i].type == 1){ //Specular\n"
"            vec4 specular_texture = texture(textures[i].id, tex_coords);\n"
"            if(specular_texture.a > 0.1){\n"
"                specular = light.specular * spec * vec3(specular_texture);\n"
"            }\n"
"        }\n"
"    }\n"
"\n"
"    ambient *= material.ambient;\n"
"    diffuse *= material.diffuse;\n"
"\n"
"    return (ambient + diffuse + specular);\n"
"}\n"
"\n"
"//*\n"
"vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)\n"
"{\n"
"    vec3 lightDir = normalize(light.position - fragPos);\n"
"    // diffuse shading\n"
"    float diff = max(dot(normal, lightDir), 0.0);\n"
"    // specular shading\n"
"    vec3 reflectDir = reflect(-lightDir, normal);\n"
"    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"
"    // attenuation\n"
"    float distance    = length(light.position - fragPos);\n"
"    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));\n"
"    // combine results\n"
"    vec3 ambient  = light.ambient  * material.color;\n"
"    vec3 diffuse  = light.diffuse  * diff * material.color;\n"
"    vec3 specular = light.specular * spec * material.specular;\n"
"\n"
"    for(int i=0; i<num_textures; i++){\n"
"        vec2 tex_coords = CalcTexCoords(textures[i], TexCoords);\n"
"        if(textures[i].type == 0){ //Diffuse\n"
"            vec4 diffuse_texture = texture(textures[i].id, tex_coords);\n"
"            if(diffuse_texture.a > 0.1){\n"
"                ambient = light.ambient * vec3(diffuse_texture);\n"
"                diffuse = light.diffuse * diff * vec3(diffuse_texture);\n"
"                specular = light.specular * spec * material.specular;\n"
"            }\n"
"        }\n"
"        if(textures[i].type == 1){ //Specular\n"
"            vec4 specular_texture = texture(textures[i].id, tex_coords);\n"
"            if(specular_texture.a > 0.1){\n"
"                specular = light.specular * spec * vec3(specular_texture);\n"
"            }\n"
"        }\n"
"    }\n"
"\n"
"    ambient *= material.ambient;\n"
"    diffuse *= material.diffuse;\n"
"\n"
"    ambient  *= attenuation;\n"
"    diffuse  *= attenuation;\n"
"    specular *= attenuation;\n"
"\n"
"    return (ambient + diffuse + specular);\n"
"}\n"
"//*/\n"
"\n"
"//*\n"
"vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)\n"
"{\n"
"    vec3 lightDir = normalize(light.position - fragPos);\n"
"    // diffuse shading\n"
"    float diff = max(dot(normal, lightDir), 0.0);\n"
"    // specular shading\n"
"    vec3 reflectDir = reflect(-lightDir, normal);\n"
"    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"
"    // attenuation\n"
"    float distance    = length(light.position - fragPos);\n"
"    float attenuation = 1.0 / (light.constant + light.linear * distance + \n"
"  			     light.quadratic * (distance * distance));\n"
"    // combine results\n"
"    vec3 ambient  = light.ambient  * material.color;\n"
"    vec3 diffuse  = light.diffuse  * diff * material.color;\n"
"    vec3 specular = light.specular * spec * material.specular;\n"
"\n"
"    for(int i=0; i<num_textures; i++){\n"
"        vec2 tex_coords = CalcTexCoords(textures[i], TexCoords);\n"
"        if(textures[i].type == 0){ //Diffuse\n"
"            vec4 diffuse_texture = texture(textures[i].id, tex_coords);\n"
"            if(diffuse_texture.a > 0.1){\n"
"                ambient = light.ambient * vec3(diffuse_texture);\n"
"                diffuse = light.diffuse * diff * vec3(diffuse_texture);\n"
"                specular = light.specular * spec * material.specular;\n"
"            }\n"
"        }\n"
"        if(textures[i].type == 1){ //Specular\n"
"            vec4 specular_texture = texture(textures[i].id, tex_coords);\n"
"            if(specular_texture.a > 0.1){\n"
"                specular = light.specular * spec * vec3(specular_texture);\n"
"            }\n"
"        }\n"
"    }\n"
"    ambient  *= attenuation;\n"
"    diffuse  *= attenuation;\n"
"    specular *= attenuation;\n"
"\n"
"    float theta = dot(lightDir, normalize(-light.direction));\n"
"    float epsilon = light.cutoff - light.outer_cutoff;\n"
"    float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0, 1.0);\n"
"    //float intensity = 1.0;\n"
"    diffuse *= intensity;\n"
"    specular *= intensity;\n"
"\n"
"    vec3 result = ambient;\n"
"    if(theta > light.outer_cutoff){\n"
"        result += diffuse + specular;\n"
"    }\n"
"    return result;\n"
"}\n"
"//*/\n";
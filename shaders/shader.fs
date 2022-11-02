#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse; // define el color de la superficie bajo iluminación difusa. El color difuso se establece (al igual que la iluminación ambiental) en el color de la superficie deseada.
    sampler2D specular; //establece el color del resaltado especular en la superficie (o posiblemente incluso refleja un color específico de la superficie).
    float shininess; //afecta la dispersión/radio del brillo especular.
}; 
struct LightDirection {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 2
struct LightPoint {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;    
in vec3 Normal;  
in vec2 TexCoords;

uniform vec3 viewPos;  
uniform LightDirection lightDirection;  
uniform LightPoint lightPoints[NR_POINT_LIGHTS];  
uniform Material material;

vec3 calLightDirection(LightDirection lightDirection);
vec3 calLightPoint(LightPoint light);

void main()
{
    vec3 result = calLightDirection(lightDirection);
    for(int i = 0; i < NR_POINT_LIGHTS; i ++){
        result = result + calLightPoint(lightPoints[i]);
    }
    FragColor = vec4(result, 1.0);
}

vec3 calLightDirection(LightDirection lightDirection)
{
    vec3 ambient =  lightDirection.ambient * vec3(texture(material.diffuse, TexCoords));

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-lightDirection.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightDirection.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    vec3 viewDirection = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec  = pow(max(dot(reflectDir, viewDirection), 0.0), material.shininess);
    vec3 specular = lightDirection.specular * spec * vec3(texture(material.specular, TexCoords));

    return ( ambient + diffuse + specular);
}
vec3 calLightPoint(LightPoint light)
{
    vec3 ambient =  light.ambient * vec3(texture(material.diffuse, TexCoords));

     vec3 norm = normalize(Normal);
     vec3 lightDir = normalize(light.position - FragPos);
     float diff = max(dot(norm, lightDir), 0.0);
     vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

     vec3 viewDirection = normalize(viewPos - FragPos);
     vec3 reflectDir = reflect(-lightDir, norm);
     float spec  = pow(max(dot(reflectDir, viewDirection), 0.0), material.shininess);
     vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation;   
    return ( ambient + diffuse + specular);
}
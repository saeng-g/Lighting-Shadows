#version 330 core

uniform vec3 camera_pos;
uniform vec3 light_pos;

in vec3 pos;
in vec3 norm;
in vec2 uvC;

out vec4 color;

void main()
{
    vec3 objectColor = vec3(0.0f, 194.0f/255.0f, 1.0f); //obj color
    vec3 L = vec3(1.0f); //light color

    vec3 l = normalize(light_pos - pos);
    vec3 n = normalize(norm);
    vec3 v = normalize(camera_pos - pos);
    vec3 r = reflect(-l, n);
    
    float amb = 0.2f;
    float diff = max(dot(n, l), 0.0f) * 0.5f;
    float spec = pow(max(dot(v, r), 0.0f), 16) * 0.2f;

    vec3 ambient = L * amb;
    vec3 diffuse = L * diff;
    vec3 specular = L * spec; 
    
    vec3 result = (ambient + diffuse + specular) * objectColor;
    color = vec4(result, 1.0f);
} 
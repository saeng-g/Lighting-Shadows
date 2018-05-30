#version 330 core
  
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec2 uvs;

uniform mat4 m_mat;
uniform mat4 v_mat;
uniform mat4 p_mat;

out vec3 pos;
out vec3 norm;
out vec2 uvC;

void main()
{
    norm = vec3(m_mat * vec4(normals, 1.0f));
    pos = vec3(m_mat * vec4(position, 1.0f));
    gl_Position = p_mat * v_mat * vec4(pos, 1.0f);
    uvC = uvs;
}
#version 130

in vec4 vPosition;
in vec4 vColor;

uniform float midX;
uniform float midY;
uniform float midZ;

uniform float angleX;
uniform float angleY;
uniform float angleZ;

out vec4 color;

void main()
{
  float PI = 3.14159265358979323846264;
  float rad_angle_x = angleX*PI/180.0;
  float rad_angle_y = angleY*PI/180.0;
  float rad_angle_z = angleZ*PI/180.0;

  vec4 a = gl_Vertex;

  vec4 b = a;
  b.y = (a.y - midY) * cos(rad_angle_x) - (a.z - midZ) * sin(rad_angle_x) + midY;
  b.z = (a.z - midZ) * cos(rad_angle_x) + (a.y - midY) * sin(rad_angle_x) + midZ;

  vec4 c = b;
  c.x = (b.x - midX) * cos(rad_angle_y) - (b.z - midZ) * sin(rad_angle_y) + midX;
  c.z = (b.z - midZ) * cos(rad_angle_y) + (b.x - midX) * sin(rad_angle_y) + midZ;

  vec4 d = c;
  d.x = (c.x - midX) * cos(rad_angle_z) - (c.y - midY) * sin(rad_angle_z) + midX;
  d.y = (c.y - midY) * cos(rad_angle_z) + (c.x - midX) * sin(rad_angle_z) + midY;

  gl_Position = gl_ModelViewProjectionMatrix*d;
  color = vColor;
}


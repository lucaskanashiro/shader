#version 130

in vec3 position;
in vec4 color;
in vec2 texCoord;

out vec2 texCoordV;

uniform float midX;
uniform float midY;
uniform float midZ;

uniform float angleX;
uniform float angleY;
uniform float angleZ;

float rad_angle_x;
float rad_angle_y;
float rad_angle_z;

vec4 rotate(vec4 a);

void main()
{
  float PI = 3.14159265358979323846264;
  rad_angle_x = angleX*PI/180.0;
  rad_angle_y = angleY*PI/180.0;
  rad_angle_z = angleZ*PI/180.0;

  vec4 a = gl_Vertex;
  vec4 d = rotate(a);

  gl_Position = gl_ModelViewProjectionMatrix*d;
  texCoordV = texCoord;
}

vec4 rotate(vec4 a)
{
  vec4 b = a;
  b.y = (a.y - midY) * cos(rad_angle_x) - (a.z - midZ) * sin(rad_angle_x) + midY;
  b.z = (a.z - midZ) * cos(rad_angle_x) + (a.y - midY) * sin(rad_angle_x) + midZ;

  vec4 c = b;
  c.x = (b.x - midX) * cos(rad_angle_y) - (b.z - midZ) * sin(rad_angle_y) + midX;
  c.z = (b.z - midZ) * cos(rad_angle_y) + (b.x - midX) * sin(rad_angle_y) + midZ;

  vec4 d = c;
  d.x = (c.x - midX) * cos(rad_angle_z) - (c.y - midY) * sin(rad_angle_z) + midX;
  d.y = (c.y - midY) * cos(rad_angle_z) + (c.x - midX) * sin(rad_angle_z) + midY;

  return d;
}

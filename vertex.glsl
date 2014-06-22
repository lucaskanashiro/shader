#version 130

in vec4 vPosition;
in vec4 vColor;

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
  b.y = a.y*cos(rad_angle_x) - a.z*sin(rad_angle_x);
  b.z = a.z*cos(rad_angle_x) + a.y*sin(rad_angle_x);

  vec4 c = b;
  c.x = b.x*cos(rad_angle_y) - b.z*sin(rad_angle_y);
  c.z = b.z*cos(rad_angle_y) + b.x*sin(rad_angle_y);

  vec4 d = c;
  d.x = c.x*cos(rad_angle_z) - c.y*sin(rad_angle_z);
  d.y = c.y*cos(rad_angle_z) + c.x*sin(rad_angle_z);

  gl_Position = gl_ModelViewProjectionMatrix*d;
  color = vColor;
}


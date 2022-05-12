/*
using namespace std;
using glm::mat3;
using glm::vec3;

// --------------------------------------------------------
// GLOBAL VARIABLES

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const float focalLength = SCREEN_HEIGHT;
SDL2Aux *sdlAux;
int t;
vec3 cameraPos(0, 0, -3);
vector<Triangle> triangles;
struct Intersection
{
  vec3 position;
  float distance;
  int triangleIndex;
};
glm::mat3 R(1.0f);
float yaw = 0.01;
vec3 lightPos(0, -0.5, -0.7);
vec3 lightColor = 14.f * vec3(1, 1, 1);
vec3 DirectLight(const Intersection &i);
vec3 indirectLight = 0.5f * vec3(1, 1, 1);

// --------------------------------------------------------
// FUNCTION DECLARATIONS

void Draw();
void Interpolate(vec3 a, vec3 b, vector<vec3> &result);
void Update();
bool ClosestIntersection(
    vec3 start,
    vec3 dir,
    const vector<Triangle> &triangles,
    Intersection &closestIntersection);

// --------------------------------------------------------
// FUNCTION DEFINITIONS

int main(int argc, char *argv[])
{
  t = SDL_GetTicks();
  sdlAux = new SDL2Aux(SCREEN_WIDTH, SCREEN_HEIGHT);
  LoadTestModel(triangles);
  while (!sdlAux->quitEvent())
  {
    Update();
    Draw();
  }

  sdlAux->saveBMP("screenshot.bmp");

  return 0;
}

void Draw()
{
  sdlAux->clearPixels();
  Intersection intersect_point;
  for (int y = 0; y < SCREEN_HEIGHT; ++y)
  {
    for (int x = 0; x < SCREEN_WIDTH; ++x)
    {
      vec3 direction(x - SCREEN_WIDTH / 2, y - SCREEN_HEIGHT / 2, focalLength);
      if (ClosestIntersection(cameraPos, direction, triangles, intersect_point))
      {
        vec3 direct_light = DirectLight(intersect_point);
        vec3 indirectLight = 0.5f * vec3(1, 1, 1);
        vec3 color = (direct_light + indirectLight) * triangles[intersect_point.triangleIndex].color;
        sdlAux->putPixel(x, y, color);
      }
      else
      {
        vec3 black(0, 0, 0);
        sdlAux->putPixel(x, y, black);
      }
    }
  }
  sdlAux->render();
}

void Update()
{
  // Compute frame time:
  int t2 = SDL_GetTicks();
  float dt = float(t2 - t);
  t = t2;
  // cout << "Render time: " << dt << " ms." << endl;
  const Uint8 *state = SDL_GetKeyboardState(NULL);
  if (state[SDL_SCANCODE_UP])
  {
    // Move camera forward
    cameraPos.z = cameraPos.z + 0.1;
  }
  if (state[SDL_SCANCODE_DOWN])
  {
    // Move camera backward
    cameraPos.z = cameraPos.z - 0.1;
  }
  if (state[SDL_SCANCODE_LEFT])
  {
    // Move camera to the left
    cameraPos.x = cameraPos.x - 0.1;
    R = mat3(cos(yaw), 0, sin(yaw), 0, 1, 0, -sin(yaw), 0, cos(yaw));
    cameraPos = R * cameraPos;
  }
  if (state[SDL_SCANCODE_RIGHT])
  {
    // Move camera to the right
    cameraPos.x = cameraPos.x + 0.1;
    R = mat3(cos(-yaw), 0, sin(-yaw), 0, 1, 0, -sin(-yaw), 0, cos(-yaw));
    cameraPos = R * cameraPos;
  }

  if (state[SDL_SCANCODE_W])
  {
    lightPos.z += 0.1;
  }
  if (state[SDL_SCANCODE_S])
  {
    lightPos.z -= 0.1;
  }
  if (state[SDL_SCANCODE_A])
  {
    lightPos.x -= 0.1;
  }
  if (state[SDL_SCANCODE_D])
  {
    lightPos.x += 0.1;
  }
  if (state[SDL_SCANCODE_Q])
  {
    lightPos.y += 0.1;
  }
  if (state[SDL_SCANCODE_E])
  {
    lightPos.y -= 0.1;
  }
};

bool ClosestIntersection(
    vec3 start,
    vec3 dir,
    const vector<Triangle> &triangles,
    Intersection &closestIntersection)
{
  float m = std::numeric_limits<float>::max();
  bool intersect = false;
  for (int i = 0; i < triangles.size(); ++i)
  {
    vec3 v0 = triangles[i].v0;
    vec3 v1 = triangles[i].v1;
    vec3 v2 = triangles[i].v2;
    vec3 e1 = v1 - v0;
    vec3 e2 = v2 - v0;
    vec3 b = start - v0;
    mat3 A(-dir, e1, e2);
    vec3 x = glm::inverse(A) * b;
    if (x.x >= 0 && x.x < m && x.y >= 0 && x.z >= 0 && (x.y + x.z) <= 1)
    {
      if (x.x < closestIntersection.distance || intersect == false)
      {
        intersect = true;
        closestIntersection.distance = x.x;
        closestIntersection.position = x;
        closestIntersection.triangleIndex = i;
      }
    }
  }
  return intersect;
}

vec3 DirectLight(const Intersection &i)
{
  /*
  float r = glm::distance(lightPos, i.position);
  float A = 4 * M_PI * r * r;
  vec3 B = lightColor / A;
  vec3 r_hat = glm::normalize(lightPos - i.position);
  vec3 n_hat = triangles[i.triangleIndex].normal;
  float rhatTimesnhat = glm::dot(r_hat, n_hat);
  float max = glm::max(rhatTimesnhat, 0.f);
  vec3 D = B * max;
  return D;*/
/*
vec3 norm, r, direct_light_color, multi;
float pi = 3.1415;
Intersection shadow;
bool shad = false;
Triangle tria = triangles[i.triangleIndex];
norm = tria.normal;
r = lightPos - i.position;
multi = norm * r;
float cos_theta = multi[0] + multi[1] + multi[2];
if (cos_theta > 0.0f)
{
  float length = sqrt(r[0] * r[0] + r[1] * r[1] + r[2] * r[2]);
  direct_light_color = lightColor * cos_theta / (4 * pi * length * length);
}
shad = ClosestIntersection(lightPos, -glm::normalize(r), triangles, shadow);
if ((shadow.triangleIndex != i.triangleIndex) && (shadow.distance < glm::length(r) - 0.0001f))
{
  direct_light_color = vec3(0.0f, 0.0f, 0.0f);
}

return direct_light_color;
}*/

/*
 * SDL2 skeleton for lab assignments 1–3 of the KTH course DH2323,
 * Computer Graphics and Interaction (and also SU DA3001, Datalogi II).
 *
 * See README.md for details.
 */

#include </opt/homebrew/Cellar/sdl2/2.0.20/include/SDL2/SDL.h>
#include <iostream>
#include </opt/homebrew/Cellar/glm/0.9.9.8/include/glm/glm.hpp>
#include <vector>
#include "SDL2Auxiliary.h"
#include "TestModel.h"



using namespace std;
using glm::mat3;
using glm::vec3;

struct Intersection
{
  vec3 position;
  float distance;
  int triangleIndex;
};

// ----------------------------------------------------------------------------
// GLOBAL VARIABLES

const int SCREEN_WIDTH = 100;
const int SCREEN_HEIGHT = 100;
SDL_Surface *screen;
int t;
vector<Triangle> triangles;
SDL2Aux *sdlAux;

// rotation variables
mat3 R = mat3(vec3(1, 0, 0), vec3(0, 1, 0), vec3(0, 0, 1));
float yaw = 0.2;

// camera variables
float focalLength = (SCREEN_HEIGHT / 2) / 0.71; // assuming 90 degree viewing angle
vec3 cameraPos(0, 0, -2.25);

// xxxtentacion variables
vec3 lightPos(0, -0.5, -0.7);
vec3 lightColor = 14.f * vec3(1, 1, 1);
vec3 indirectLight = 0.5f * vec3(1, 1, 1);

// ----------------------------------------------------------------------------
// FUNCTIONS

void Update();
void Draw();
bool ClosestIntersection(vec3 start, vec3 dir, const vector<Triangle> &triangles, Intersection &closestIntersection);
vec3 DirectLight(Intersection &i);

int main(int argc, char *argv[])
{
  t = SDL_GetTicks();
  sdlAux = new SDL2Aux(SCREEN_WIDTH, SCREEN_HEIGHT);
  LoadTestModel(triangles);
  while (!sdlAux->quitEvent())
  {
    Update();
    Draw();
  }

  sdlAux->saveBMP("screenshot.bmp");

  return 0;
}

void Update()
{
  // Compute frame time:
  int t2 = SDL_GetTicks();
  float dt = float(t2 - t);
  t = t2;
  cout << "Render time: " << dt << " ms." << endl;
  vec3 forward(R[2][0], R[2][1], R[2][2]);
  vec3 right(R[0][0], R[0][1], R[0][2]);

  const Uint8 *keystate = SDL_GetKeyboardState(NULL);
  if (keystate[SDL_SCANCODE_UP])
  {
    // Move camera forward
    cameraPos = cameraPos + 0.2f * forward;
  }
  if (keystate[SDL_SCANCODE_DOWN])
  {
    cameraPos = cameraPos - 0.2f * forward;
  }
  if (keystate[SDL_SCANCODE_LEFT])
  {
    // Move camera to the left
    yaw = yaw + 0.2;
    R = mat3(vec3(glm::cos(yaw), float(0), -glm::sin(yaw)), vec3(0, 1, 0), vec3(glm::sin(yaw), float(0), glm::cos(yaw)));
    cameraPos = cameraPos - 0.2f * right;
  }
  if (keystate[SDL_SCANCODE_RIGHT])
  {
    // Move camera to the right
    yaw = yaw - 0.2;
    R = mat3(vec3(glm::cos(yaw), float(0), -glm::sin(yaw)), vec3(0, 1, 0), vec3(glm::sin(yaw), float(0), glm::cos(yaw)));
    cameraPos = cameraPos + 0.2f * right;
  }
  if (keystate[SDL_SCANCODE_W])
  {
    lightPos.z += 0.1;
  }
  if (keystate[SDL_SCANCODE_S])
  {
    lightPos.z -= 0.1;
  }
  if (keystate[SDL_SCANCODE_D])
  {
    lightPos.x += 0.1;
  }
  if (keystate[SDL_SCANCODE_A])
  {
    lightPos.x -= 0.1;
  }
  if (keystate[SDL_SCANCODE_Q])
  {
    lightPos.y += 0.1;
  }
  if (keystate[SDL_SCANCODE_E])
  {
    lightPos.y -= 0.1;
  }
}

void Draw()
{
  for (int y = 0; y < SCREEN_HEIGHT; ++y)
  {
    for (int x = 0; x < SCREEN_WIDTH; ++x)
    {
      Intersection intersection;
      vec3 color;
      vec3 d(float(x) - (SCREEN_WIDTH / 2), float(y) - (SCREEN_HEIGHT / 2), focalLength);
      d = R * d;
      if (ClosestIntersection(cameraPos, d, triangles, intersection))
      {
        color = DirectLight(intersection);
        vec3 indirect = triangles[intersection.triangleIndex].color * indirectLight;
        color = color + indirect;
      }
      else
      {
        color = vec3(float(0), float(0), float(0));
      }

      sdlAux->putPixel(x, y, color);
    }
   
  }
   sdlAux->render();
}

bool ClosestIntersection(vec3 start, vec3 dir, const vector<Triangle> &triangles, Intersection &closestIntersection)
{
  float m = std::numeric_limits<float>::max();
  float t, u, v;
  vec3 v0, v1, v2, e1, e2, b, x;
  mat3 A(float(-1) * dir, e1, e2);
  for (int i = 0; i < triangles.size(); i++)
  {
    // loop through triangles and solve interesction system
    v0 = triangles[i].v0;
    v1 = triangles[i].v1;
    v2 = triangles[i].v2;
    e1 = v1 - v0;
    e2 = v2 - v0;
    b = start - v0;
    A = mat3(-dir, e1, e2);
    x = glm::inverse(A) * b;
    t = x[0];
    u = x[1];
    v = x[2];
    // check if ray intersects triangle
    if (u >= 0 && v >= 0 && u + v <= 1 && t >= 0)
    {

      vec3 intPoint = v0 + u * e1 + v * e2;
      float dist = glm::distance(intPoint, start);
      if (dist < m)
      {
        m = dist;
        closestIntersection.position =  intPoint;
        closestIntersection.distance = dist;
        closestIntersection.triangleIndex= i;
      }
    }
  }
  if (m < std::numeric_limits<float>::max())
  {
    return true;
  }
  return false;
}

vec3 DirectLight(Intersection &i)
{
  Intersection intersection;
  Triangle t = triangles[i.triangleIndex];
  vec3 n = t.normal;
  vec3 r = lightPos - i.position;
  vec3 rHat = glm::normalize(r);
  vec3 D;
  // move origin sligthtly along normal to avoid intersecting with the triangle itself
  vec3 shadowOrig = glm::length(rHat * n) < 0.f ? i.position - n * 0.001f : i.position + n * 0.001f;
  if (ClosestIntersection(shadowOrig, rHat, triangles, intersection))
  {
    if (intersection.distance < glm::length(r))
    {
      D = vec3(0, 0, 0);
      return D;
    }
  }

  D = vec3(lightColor * glm::max(glm::dot(rHat, n), 0.f)) / (4.f * glm::pow(glm::length(r), 2.f) * 3.14f); // glm does not seem to include pi for some reason
  D = t.color * D;

  return D;
}

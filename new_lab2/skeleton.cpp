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
        vec3 directColor = DirectLight(intersect_point);
        vec3 finalColor = triangles[intersect_point.triangleIndex].color * (directColor + indirectLight);
        sdlAux->putPixel(x, y, finalColor);
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
  cout << "Render time: " << dt << " ms." << endl;
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
      vec3 intPoint = v0 + x.y * e1 + x.z * e2;
      float distance = glm::distance(intPoint, start);
      if (distance < closestIntersection.distance || intersect == false)
      {
        intersect = true;
        closestIntersection.distance = distance;
        closestIntersection.position = intPoint;
        closestIntersection.triangleIndex = i;
      }
    }
  }
  return intersect;
}

vec3 DirectLight(const Intersection &i)
{
  float r = glm::distance(lightPos, i.position);
  float A = 4 * M_PI * r * r;
  vec3 B = lightColor / A;
  vec3 r_hat = glm::normalize(lightPos - i.position);
  vec3 n_hat = triangles[i.triangleIndex].normal;
  float rhatTimesnhat = glm::dot(r_hat, n_hat);
  float max = glm::max(rhatTimesnhat, 0.f);
  vec3 D = B * max;
  Intersection intersection;
  // Why need this line?
  vec3 shadowOrig = glm::length(r_hat * n_hat) < 0.f ? i.position - n_hat * 0.001f : i.position + n_hat * 0.001f;
  if (ClosestIntersection(shadowOrig, r_hat, triangles, intersection))
  {
    if (intersection.distance < r)
    {
      D = vec3(0, 0, 0);
      return D;
    }
  }
  return D;
}
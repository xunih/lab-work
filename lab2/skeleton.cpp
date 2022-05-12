/*
 * SDL2 skeleton for lab assignments 1–3 of the KTH course DH2323,
 * Computer Graphics and Interaction (and also SU DA3001, Datalogi II).
 *
 * See README.md for details.
 */
#define _USE_MATH_DEFINES

#include <cmath>
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

const int SCREEN_WIDTH = 50;
const int SCREEN_HEIGHT = 50;
float focalLength = SCREEN_HEIGHT;
SDL2Aux *sdlAux;
int t;
vector<Triangle> triangles;
struct Intersection
{
  vec3 position;
  float distance;
  int triangleIndex;
};
vec3 cameraPos(0, 0, -3);
mat3 R;
float yaw = 0.0;
vec3 indirectLight = 0.5f * vec3(1, 1, 1);
// vec3 lightPos(0, -0.5, -0.7);
// vec3 lightColor = 14.f * vec3(1, 1, 1);

// --------------------------------------------------------
// FUNCTION DECLARATIONS

void Draw();
void Update();
bool ClosestIntersection(
    vec3 start,
    vec3 dir,
    const vector<Triangle> &triangles,
    Intersection &closestIntersection);
vec3 DirectLight(Intersection &i);
vec3 lightPos(0, -0.5, -0.7);
vec3 lightColor = 14.f * vec3(1, 1, 1);

// --------------------------------------------------------
// FUNCTION DEFINITIONS

int main(int argc, char *argv[])
{
  sdlAux = new SDL2Aux(SCREEN_WIDTH, SCREEN_HEIGHT);
  t = SDL_GetTicks();
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
  Intersection closestIntersectionPoint;
  for (int y = 0; y < SCREEN_HEIGHT; ++y)
  {
    for (int x = 0; x < SCREEN_WIDTH; ++x)
    {
      vec3 direction(x - (SCREEN_WIDTH / 2), y - (SCREEN_HEIGHT / 2), focalLength);

      if (ClosestIntersection(cameraPos, direction, triangles, closestIntersectionPoint))
      {
        sdlAux->putPixel(x, y, triangles[closestIntersectionPoint.triangleIndex].color);
      }
      else
      {
        vec3 blackColor(0.0, 0.0, 0.0);
        sdlAux->putPixel(x, y, blackColor);
      }
    }
    sdlAux->render();
  }
}

void Update()
{
  vec3 right(R[0][0], R[0][1], R[0][2]);
  vec3 down(R[1][0], R[1][1], R[1][2]);
  vec3 forward(R[2][0], R[2][1], R[2][2]);

  int t2 = SDL_GetTicks();
  float dt = float(t2 - t);
  t = t2;
  cout << "Render time: " << dt << " ms." << endl;
  const Uint8 *keystate = SDL_GetKeyboardState(0);
  if (keystate[SDL_SCANCODE_UP])
  {
    cameraPos.z = cameraPos.z + 0.5;
  }
  if (keystate[SDL_SCANCODE_DOWN])
  {
    // Move camera backward
    cameraPos.z = cameraPos.z - 0.5;
  }
  if (keystate[SDL_SCANCODE_LEFT])
  {
    // Move camera to the left
    yaw = 5.0f * M_PI / 180.0f;
    R = mat3(cos(yaw), 0, sin(yaw),
             0, 1, 0,
             -sin(yaw), 0, cos(yaw));
    cameraPos = R * cameraPos;
    cout << "inside left" << endl;
  }
  if (keystate[SDL_SCANCODE_RIGHT])
  {
    // Move camera to the right
    yaw = 5.0f * M_PI / (-180.0f);
    R = mat3(cos(yaw), 0, sin(yaw),
             0, 1, 0,
             -sin(yaw), 0, cos(yaw));
    cameraPos = R * cameraPos;
    cout << "inside right" << endl;
  }
  if (keystate[SDL_SCANCODE_W])
    lightPos.z += 0.1;
  if (keystate[SDL_SCANCODE_S])
    lightPos.z -= 0.1;
  if (keystate[SDL_SCANCODE_D])
    lightPos.x += 0.1;
  if (keystate[SDL_SCANCODE_A])
    lightPos.x -= 0.1;
  if (keystate[SDL_SCANCODE_X])
    lightPos.y += 0.1;
  if (keystate[SDL_SCANCODE_Z])
    lightPos.y -= 0.1;
}

bool ClosestIntersection(vec3 start,
                         vec3 dir,
                         const vector<Triangle> &triangles,
                         Intersection &closestIntersection)
{
  float m = std::numeric_limits<float>::max();
  bool intersect = false;
  int index;
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
    float t = x.x;
    float u = x.y;
    float v = x.z;
    float uv = u + v;
    if (t >= 0 && t < m && u >= 0 && v >= 0 && uv <= 1)
    {
      index = i;
      m = t;
      intersect = true;
    }
  }
  if (intersect == true)
  {
    closestIntersection.triangleIndex = index;
    return true;
  }
  else
  {
    return false;
  }
}

vec3 DirectLight(Intersection &i)
{
  vec3 n_hat = triangles[i.triangleIndex].normal;
  float r = glm::distance(lightPos, i.position);
  vec3 r_hat = glm::normalize(lightPos - i.position);
  float cross = glm::dot(r_hat, n_hat);
  float divide = 4.0f * M_PI * (r * r);
  vec3 D = (lightColor * max(cross, 0.0f)) / divide;
  return D;
}
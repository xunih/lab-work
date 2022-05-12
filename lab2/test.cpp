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
float focalLength = SCREEN_HEIGHT / 2;
SDL2Aux *sdlAux;
int t;
vector<Triangle> triangles;
struct Intersection
{
  vec3 position;
  float distance;
  int triangleIndex;
};
vec3 cameraPos(0, 0, -(2 * focalLength / SCREEN_HEIGHT + 1));

// --------------------------------------------------------
// FUNCTION DECLARATIONS

void Draw();
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
  LoadTestModel(triangles);
  while (!sdlAux->quitEvent())
  {
    cout << "inside main" << endl;
    Update();
    Draw();
  }

  sdlAux->saveBMP("screenshot.bmp");

  return 0;
}

void Draw()
{
  cout << "inside draw" << endl;
  sdlAux->clearPixels();
  for (int y = 0; y < SCREEN_HEIGHT; ++y)
  {
    cout << "inside for loop one" << endl;
    for (int x = 0; x < SCREEN_WIDTH; ++x)
    {

      cout << "inside for loop two" << endl;
      Intersection closestIntersectionPoint;
      vec3 direction(x - (SCREEN_WIDTH / 2), y - (SCREEN_HEIGHT / 2), focalLength);
      if (ClosestIntersection(cameraPos, direction, triangles, closestIntersectionPoint) == true)
      {
        cout << "inside true" << endl;
        sdlAux->putPixel(x, y, triangles[closestIntersectionPoint.triangleIndex].color);
      }
      else
      {
        cout << "inside false" << endl;
        vec3 blackColor(0.0, 0.0, 0.0);
        sdlAux->putPixel(x, y, blackColor);
      }
    }
    sdlAux->render();
  }
}

void Update()
{
  cout << "inside update" << endl;
  int t2 = SDL_GetTicks();
  float dt = float(t2 - t);
  t = t2;
  cout << "Render time: " << dt << " ms." << endl;
};

bool ClosestIntersection(vec3 start,
                         vec3 dir,
                         const vector<Triangle> &triangles,
                         Intersection &closestIntersection)
{
  float m = std::numeric_limits<float>::max();
  bool intersect = false;
  for (size_t i = 0; i < triangles.size(); ++i)
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
    if (t >= 0 && u > 0 && v > 0 && uv < 1 && t < m)
    {
      intersect = true;
    }
    else
    {
      intersect = false;
    }
  }
  return intersect;
}

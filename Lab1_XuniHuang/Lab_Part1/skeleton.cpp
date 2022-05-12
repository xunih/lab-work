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

using namespace std;
using glm::vec3;

// --------------------------------------------------------
// GLOBAL VARIABLES

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
SDL2Aux *sdlAux;
int t;

// --------------------------------------------------------
// FUNCTION DECLARATIONS

void Draw();
void Interpolate(vec3 a, vec3 b, vector<vec3> &result);
void Update();

// --------------------------------------------------------
// FUNCTION DEFINITIONS

int main(int argc, char *argv[])
{
  t = SDL_GetTicks();
  sdlAux = new SDL2Aux(SCREEN_WIDTH, SCREEN_HEIGHT);
  /*
  vector<vec3> result(1);
  vec3 a(1, 4, 9.2);
  vec3 b(4, 1, 9.8);
  Interpolate(a, b, result);
  for (int i = 0; i < result.size(); ++i)
  {
    cout << "( "
         << result[i].x << ", "
         << result[i].y << ", "
         << result[i].z << " ) ";
  }*/

  while (!sdlAux->quitEvent())
  {
    Draw();
  }

  sdlAux->saveBMP("screenshot.bmp");

  return 0;
}

void Draw()
{
  sdlAux->clearPixels();
  vec3 topLeft(1, 0, 0);     // red
  vec3 topRight(0, 0, 1);    // blue
  vec3 bottomLeft(0, 1, 0);  // green
  vec3 bottomRight(1, 1, 0); // yellow

  // Interpolate the values of the very top, and down side of the screen
  vector<vec3> topSide(SCREEN_WIDTH);
  vector<vec3> downSide(SCREEN_WIDTH);
  Interpolate(topLeft, topRight, topSide);
  Interpolate(bottomRight, bottomLeft, downSide);

  for (int x = 0; x < SCREEN_WIDTH; ++x)
  {
    // For each column of the screen,
    // interpolate between the values of the top and down side of this column
    vector<vec3> colResults(SCREEN_HEIGHT);
    Interpolate(topSide[x], downSide[x], colResults);
    for (int y = 0; y < SCREEN_HEIGHT; ++y)
    {
      sdlAux->putPixel(x, y, colResults[y]);
    }
  }
  sdlAux->render();
}

void Interpolate(vec3 a, vec3 b, vector<vec3> &result)
{
  // When the size of the results array is not 0
  if (result.size() - 1 != 0)
  {
    result[0].x = a.x;
    result[0].y = a.y;
    result[0].z = a.z;
    for (float i = 1; i < result.size() - 1; ++i)
    {
      // colorAfterInterpolatoon = a(1 - t) + bt
      // t = i/(result.size()-1)
      result[i].x = a.x * (1 - (i / (result.size() - 1))) + b.x * (i / (result.size() - 1));
      result[i].y = a.y * (1 - (i / (result.size() - 1))) + b.y * (i / (result.size() - 1));
      result[i].z = a.z * (1 - (i / (result.size() - 1))) + b.z * (i / (result.size() - 1));
    }
    result[result.size() - 1].x = b.x;
    result[result.size() - 1].y = b.y;
    result[result.size() - 1].z = b.z;
  }
  else
  // When the size of the result array is 0, take the average of two colors
  {
    result[0].x = (a.x + b.x) / 2;
    result[0].y = (a.y + b.y) / 2;
    result[0].z = (a.z + b.z) / 2;
  }
}

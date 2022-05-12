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
const float focalLength = SCREEN_HEIGHT / 2;
SDL2Aux *sdlAux;
vector<vec3> stars(1000);
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
  float max = 1.0;
  float minForXandY = -1.0;
  for (int i = 0; i < stars.size() - 1; ++i)
  {
    // Generates a random number from -1 and 1
    float randNumX = minForXandY + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - minForXandY)));
    // cout << randNumX << ", ";
    // Generates a random number from -1 and 1
    float randNumY = minForXandY + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - minForXandY)));
    // cout << randNumY << ", ";
    // Generates a random number from 0 and 1
    float randNumZ = float(rand()) / float(RAND_MAX);
    // cout << randNumZ << ", ";
    stars[i].x = randNumX;
    // cout << stars[i].x << ", ";
    stars[i].y = randNumY;
    // cout << stars[i].y << ", ";
    stars[i].z = randNumZ;
    // cout << stars[i].z << ", ";
  }
  sdlAux = new SDL2Aux(SCREEN_WIDTH, SCREEN_HEIGHT);

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

  for (int y = 0; y < SCREEN_HEIGHT; ++y)
  {
    for (int x = 0; x < SCREEN_WIDTH; ++x)
    {
      // Firstly make it black
      vec3 color(0.0, 0.0, 0.0);
      sdlAux->putPixel(x, y, color);
    }
  }

  for (size_t s = 0; s < stars.size(); ++s)
  {
    // Star projection and drawing
    int u = focalLength * (stars[s].x / stars[s].z) + SCREEN_WIDTH / 2;
    int v = focalLength * (stars[s].y / stars[s].z) + SCREEN_HEIGHT / 2;
    vec3 colorTwo = 0.2f * vec3(1, 1, 1) / (stars[s].z * stars[s].z);
    sdlAux->putPixel(u, v, colorTwo);
  }
  sdlAux->render();
}

void Update()
{
  int t2 = SDL_GetTicks();
  float dt = float(t2 - t);
  t = t2;
  float v = 0.001;
  // Update the stars to enable motion
  for (int i = 0; i < stars.size(); ++i)
  {
    stars[i].z = stars[i].z - v * dt;
    if (stars[i].z <= 0)
      stars[i].z += 1;
    if (stars[i].z > 1)
      stars[i].z -= 1;
  }
};

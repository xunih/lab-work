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
using glm::ivec2;
using glm::mat3;
using glm::vec2;
using glm::vec3;

// --------------------------------------------------------
// GLOBAL VARIABLES

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
float focalLength = SCREEN_HEIGHT;
SDL2Aux *sdlAux;
int t;
vector<Triangle> triangles;
vec3 cameraPos(0, 0, -3.001);
glm::mat3 R(1.0f);
float yaw = 0.01;
vec3 currentColor;
float depthBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];
struct Pixel
{
  int x;
  int y;
  float zinv;
  vec3 pos3d;
};
struct Vertex
{
  vec3 position;
};

vec3 lightPos(0, -0.5, -0.7);
vec3 lightPower = 1.1f * vec3(1, 1, 1);
vec3 indirectLightPowerPerArea = 0.5f * vec3(1, 1, 1);
vec3 currentNormal;
float currentReflectance;
float storedZinv;

// --------------------------------------------------------
// FUNCTION DECLARATIONS

void Draw();
void Update();
void VertexShader(const vec3 &v, Pixel &p);
void Interpolate(Pixel a, Pixel b, vector<Pixel> &result);
void DrawLineSDL(Pixel a, Pixel b, vec3 color);
void DrawPolygonEdges(const vector<vec3> &vertices);
void ComputePolygonRows(
    const vector<Pixel> &vertexPixels, vector<Pixel> &leftPixels,
    vector<Pixel> &rightPixels);
void DrawPolygonRows(
    const vector<Pixel> &leftPixels,
    const vector<Pixel> &rightPixels);
void DrawPolygon(const vector<Vertex> &vertices);
void PixelShader(const Pixel &p);

// --------------------------------------------------------
// FUNCTION DEFINITIONS

int main(int argc, char *argv[])
{
  sdlAux = new SDL2Aux(SCREEN_WIDTH, SCREEN_HEIGHT);
  t = SDL_GetTicks();
  LoadTestModel(triangles);
  /*
  vector<ivec2> result(4);
  ivec2 a(4, 2);
  ivec2 b(1, 8);
  Interpolate(a, b, result);
  for (int i = 0; i < result.size(); ++i)
  {
    cout << "( "
         << result[i].x << ", "
         << result[i].y << " ) ";
  }*/
  /*
  vector<ivec2> vertexPixels(3);
  vertexPixels[0] = ivec2(10, 5);
  vertexPixels[1] = ivec2(5, 10);
  vertexPixels[2] = ivec2(15, 15);
  vector<ivec2> leftPixels;
  vector<ivec2> rightPixels;
  ComputePolygonRows(vertexPixels, leftPixels, rightPixels);
  for (int row = 0; row < leftPixels.size(); ++row)
  {
    cout << "Start: ("
         << leftPixels[row].x << ","
         << leftPixels[row].y << "). "
         << "End: ("
         << rightPixels[row].x << ","
         << rightPixels[row].y << "). " << endl;
  }*/
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
  // sdlAux->clearPixels();
  for (int y = 0; y < SCREEN_HEIGHT; ++y)
  {
    for (int x = 0; x < SCREEN_WIDTH; ++x)
      depthBuffer[y][x] = 0;
  }
  for (int i = 0; i < triangles.size(); ++i)
  {
    currentColor = triangles[i].color;
    vector<Vertex> vertices(3);
    vertices[0].position = triangles[i].v0;
    vertices[1].position = triangles[i].v1;
    vertices[2].position = triangles[i].v2;
    currentNormal = triangles[i].normal;
    currentReflectance = 15;
    DrawPolygon(vertices);
    /*
    for (int k = 0; k < vertices.size(); ++k)
    {
      ivec2 projPos;
      VertexShader(vertices[k], projPos);
      vec3 color(1, 1, 1);
      sdlAux->putPixel(projPos.x, projPos.y, color);
    }*/
    // DrawPolygonEdges(vertices);
  }
  sdlAux->render();
}

void Update()
{
  // Compute frame time : 
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
}

void VertexShader(const Vertex &v, Pixel &p)
{
  vec3 pos = v.position - cameraPos;
  p.zinv = 1 / pos.z;
  p.x = int(focalLength * pos.x * p.zinv) + SCREEN_WIDTH / 2;
  p.y = int(focalLength * pos.y * p.zinv) + SCREEN_HEIGHT / 2;
  p.pos3d = v.position;
}

void Interpolate(Pixel a, Pixel b, vector<Pixel> &result)
{
  int N = result.size();
  float stepX = (b.x - a.x) / float(max(N - 1, 1));
  float stepY = (b.y - a.y) / float(max(N - 1, 1));
  float stepZ = (b.zinv - a.zinv) / float(max(N - 1, 1));

  b.pos3d = b.pos3d * b.zinv;
  a.pos3d = a.pos3d * a.zinv;

  vec3 stepPos = (b.pos3d - a.pos3d) / float(max(N - 1, 1)); 
  Pixel current(a);
  for (int i = 0; i < N; ++i)
  {
    current.x = a.x + i * stepX;
    current.y = a.y + i * stepY;
    current.zinv = a.zinv + i * stepZ;
    current.pos3d = a.pos3d + (float)i * stepPos;
    current.pos3d = current.pos3d / current.zinv; 
    result[i] = current; 
  }
}

void DrawLineSDL(Pixel a, Pixel b, vec3 color)
{
  ivec2 delta;
  delta.x = glm::abs(a.x - b.x);
  delta.y = glm::abs(a.y - b.y);
  int pixels = glm::max(delta.x, delta.y) + 1;
  vector<Pixel> line(pixels);
  Interpolate(a, b, line);
  for (int i = 0; i < pixels; ++i)
  {
    PixelShader(line[i]);
  }
}

void ComputePolygonRows(
    const vector<Pixel> &vertexPixels, vector<Pixel> &leftPixels,
    vector<Pixel> &rightPixels)
{ // 1. Find max and min y-value of the polygon
  // and compute the number of rows it occupies.
  int maxYValue = vertexPixels[0].y;
  int minYValue = vertexPixels[0].y;
  for (int i = 0; i < vertexPixels.size(); ++i)
  {
    if (maxYValue < vertexPixels[i].y)
      maxYValue = vertexPixels[i].y;
    if (minYValue > vertexPixels[i].y)
      minYValue = vertexPixels[i].y;
  }
  int nrOfRows = maxYValue - minYValue + 1;

  // 2. Resize leftPixels and rightPixels
  // so that they have an element for each row.x
  leftPixels.resize(nrOfRows);
  rightPixels.resize(nrOfRows);

  // 3. Initialize the x-coordinates in leftPixels
  // to some really large value and the x-coordinates
  // in rightPixels to some really small value.
  for (int i = 0; i < nrOfRows; ++i)
  {
    leftPixels[i].x = +numeric_limits<int>::max();
    leftPixels[i].y = i + minYValue;
    rightPixels[i].x = -numeric_limits<int>::max();
    rightPixels[i].y = i + minYValue;
  }

  // 4. Loop through all edges of the polygon and use
  // linear interpolation to find the x-coordinate for
  // each row it occupies. Update the corresponding
  // values in rightPixels and leftPixels.
  for (int i = 0; i < vertexPixels.size(); ++i)
  {
    int j = (i + 1) % vertexPixels.size();
    ivec2 delta;
    delta.x = glm::abs(vertexPixels[i].x - vertexPixels[j].x);
    delta.y = glm::abs(vertexPixels[i].y - vertexPixels[j].y);
    int pixels = glm::max(delta.x, delta.y) + 1;
    vector<Pixel> resultedLine(pixels);
    Interpolate(vertexPixels[i], vertexPixels[j], resultedLine);
    for (int k = 0; k < resultedLine.size(); k++)
    {
      for (int s = 0; s < nrOfRows; s++)
      {
        if (leftPixels[s].y == resultedLine[k].y)
        {
          if (leftPixels[s].x > resultedLine[k].x)
          {
            leftPixels[s].x = resultedLine[k].x;
            leftPixels[s].zinv = resultedLine[k].zinv;
            leftPixels[s].pos3d = resultedLine[k].pos3d;
          }
          if (rightPixels[s].x < resultedLine[k].x)
          {
            rightPixels[s].x = resultedLine[k].x;
            rightPixels[s].zinv = resultedLine[k].zinv;
            rightPixels[s].pos3d = resultedLine[k].pos3d;
          }
        }
      }
    }
  }
}

void DrawPolygonRows(
    const vector<Pixel> &leftPixels,
    const vector<Pixel> &rightPixels)
{
  for (int i = 0; i < leftPixels.size(); i++)
  {
    DrawLineSDL(leftPixels[i], rightPixels[i], currentColor);
  }
}

void DrawPolygon(const vector<Vertex> &vertices)
{
  int V = vertices.size();
  vector<Pixel> vertexPixels(V);
  for (int i = 0; i < V; ++i)
    VertexShader(vertices[i], vertexPixels[i]);
  vector<Pixel> leftPixels;
  vector<Pixel> rightPixels;
  ComputePolygonRows(vertexPixels, leftPixels, rightPixels);
  DrawPolygonRows(leftPixels, rightPixels);
}

void PixelShader(const Pixel &p)
{
  int x = p.x;
  int y = p.y;
  float r = glm::distance(lightPos, p.pos3d);
  vec3 r_dir = glm::normalize(lightPos - p.pos3d);
  float cross = glm::dot(r_dir, currentNormal);
  vec3 D = lightPower * max(cross, 0.0f) / float(4.0f * M_PI * (r * r));
  vec3 illumination = D * currentReflectance + indirectLightPowerPerArea;
  if (p.zinv > depthBuffer[y][x])
  {
    depthBuffer[y][x] = p.zinv;
    sdlAux->putPixel(x, y, currentColor * illumination);
  }
}

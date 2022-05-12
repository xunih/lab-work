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

const int SCREEN_WIDTH = 150;
const int SCREEN_HEIGHT = 150;
float focalLength = SCREEN_WIDTH;
SDL2Aux *sdlAux;
int t;
vector<Triangle> triangles;
vec3 cameraPos(0, 0, -3.001);
glm::mat3 R(1.0f);
float yaw = 0;
vec3 currentColor;

// --------------------------------------------------------
// FUNCTION DECLARATIONS

void Draw();
void Update();
void VertexShader(const vec3 &v, ivec2 &p);
void Interpolate(ivec2 a, ivec2 b, vector<ivec2> &result);
void DrawLineSDL(ivec2 a, ivec2 b, vec3 color);
void DrawPolygonEdges(const vector<vec3> &vertices);
void ComputePolygonRows(const vector<ivec2> &vertexPixels, vector<ivec2> &leftPixels, vector<ivec2> &rightPixels);
void DrawPolygonRows(const vector<ivec2> &leftPixels,
                     const vector<ivec2> &rightPixels);
void DrawPolygon(const vector<vec3> &vertices);

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
  }
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
  yaw = 0;
  glm::mat3 R(1.0f);
  for (int i = 0; i < triangles.size(); ++i)
  {
    currentColor = triangles[i].color;
    vector<vec3> vertices(3);
    vertices[0] = triangles[i].v0;
    vertices[1] = triangles[i].v1;
    vertices[2] = triangles[i].v2;
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
  int t2 = SDL_GetTicks();
  // float dt = float(t2 - t);
  t = t2;
  // cout << "Render time: " << dt << " ms." << endl;
  const Uint8 *keystate = SDL_GetKeyboardState(0);
  if (keystate[SDL_SCANCODE_UP])
  {
    cameraPos.z = cameraPos.z + 0.1;
  }
  if (keystate[SDL_SCANCODE_DOWN])
  {
    cameraPos.z = cameraPos.z - 0.1;
  }
  if (keystate[SDL_SCANCODE_LEFT])
  {
    yaw += 0.1;
  }
  if (keystate[SDL_SCANCODE_RIGHT])
  {
    yaw -= 0.1;
  }
  if (keystate[SDL_SCANCODE_W])
    ;
  if (keystate[SDL_SCANCODE_S])
    ;
  if (keystate[SDL_SCANCODE_D])
    ;
  if (keystate[SDL_SCANCODE_A])
    ;
  if (keystate[SDL_SCANCODE_X])
    ;
  if (keystate[SDL_SCANCODE_Z])
    ;

  vec3 one(cos(yaw), 0, sin(yaw));
  vec3 two(0, 1, 0);
  vec3 three(-sin(yaw), 0, cos(yaw));
  R = mat3(one, two, three);
  cameraPos = R * cameraPos;
}

void VertexShader(const vec3 &v, ivec2 &p)
{
  p.x = focalLength * (cameraPos - v).x / (cameraPos - v).z + SCREEN_WIDTH / 2;
  p.y = focalLength * (cameraPos - v).y / (cameraPos - v).z + SCREEN_HEIGHT / 2;
}

void Interpolate(ivec2 a, ivec2 b, vector<ivec2> &result)
{
  int N = result.size();
  vec2 step = vec2(b - a) / float(max(N - 1, 1));
  vec2 current(a);
  for (int i = 0; i < N; ++i)
  {
    result[i] = current;
    current += step;
  }
}

void DrawLineSDL(ivec2 a, ivec2 b, vec3 color)
{
  ivec2 delta = glm::abs(a - b);
  int pixels = glm::max(delta.x, delta.y) + 1;
  vector<ivec2> line(pixels);
  Interpolate(a, b, line);
  for (int i = 0; i < pixels; ++i)
  {
    sdlAux->putPixel(line[i].x, line[i].y, color);
  }
}

void DrawPolygonEdges(const vector<vec3> &vertices)
{
  int V = vertices.size();
  vector<ivec2> projectedVertices(V);
  // Transform each vertex from 3D world position to 2D image position: vector<ivec2> projectedVertices( V );
  for (int i = 0; i < V; ++i)
  {
    VertexShader(vertices[i], projectedVertices[i]);
  }

  // Loop over all vertices and draw the edge from it to the next vertex:
  for (int i = 0; i < V; ++i)
  {
    int j = (i + 1) % V; // The next vertex
    vec3 color(1, 1, 1);
    DrawLineSDL(projectedVertices[i], projectedVertices[j], color);
  }
}

void ComputePolygonRows(const vector<ivec2> &vertexPixels, vector<ivec2> &leftPixels, vector<ivec2> &rightPixels)
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
    ivec2 delta = glm::abs(vertexPixels[i] - vertexPixels[j]);
    int pixels = glm::max(delta.x, delta.y) + 1;
    vector<ivec2> resultedLine(pixels);
    Interpolate(vertexPixels[i], vertexPixels[j], resultedLine);
    for (int k = 0; k < resultedLine.size(); k++)
    {
      for (int s = 0; s < nrOfRows; s++)
      {
        if (leftPixels[s].y == resultedLine[k].y)
        {
          if (leftPixels[s].x > resultedLine[k].x)
            leftPixels[s].x = resultedLine[k].x;
          if (rightPixels[s].x < resultedLine[k].x)
            rightPixels[s].x = resultedLine[k].x;
        }
      }
    }
  }
}

void DrawPolygonRows(const vector<ivec2> &leftPixels,
                     const vector<ivec2> &rightPixels)
{
  for (int i = 0; i < leftPixels.size(); i++)
  {
    DrawLineSDL(leftPixels[i], rightPixels[i], currentColor);
  }
}

void DrawPolygon(const vector<vec3> &vertices)
{
  int V = vertices.size();
  vector<ivec2> vertexPixels(V);
  for (int i = 0; i < V; ++i)
    VertexShader(vertices[i], vertexPixels[i]);
  vector<ivec2> leftPixels;
  vector<ivec2> rightPixels;
  ComputePolygonRows(vertexPixels, leftPixels, rightPixels);
  DrawPolygonRows(leftPixels, rightPixels);
}

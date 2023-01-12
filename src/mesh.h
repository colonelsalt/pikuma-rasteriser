#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"

#define NUM_MESH_VERTICES 8

extern vec3_t g_MeshVertices[NUM_MESH_VERTICES];

#define NUM_MESH_FACES (6 * 2) // 6 cube faces, 2 triangles per face
extern face_t g_MeshFaces[NUM_MESH_FACES];

#endif
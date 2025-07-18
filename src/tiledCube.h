#ifndef TILED_CUBE_H
#define TILED_CUBE_H

#include "raylib.h"
#include <vector>  // Fix: use angle brackets, not quotes
#include "collision.h"

struct MeshWithMaterial {
    Mesh mesh;
    Material material;
    Vector3 position;
};

// Generates a cube mesh with texture coords tiled 'tileRepeat' times per face.
// 'size' is half edge length (cube goes from -size to +size).
Mesh GenTiledCubeMesh(float size, float tileRepeat);

// Creates mesh + material with texture, adds collider at 'position'
MeshWithMaterial CreateTiledCubeWithColliderAndTexture(std::vector<AABB>& colliders, Vector3 position, Vector3  size, int tileRepeat, Texture2D texture);

// Draws the mesh+material at given position (translation only)
void DrawMeshAtPosition(const MeshWithMaterial& mesh, Vector3 position);

#endif // TILED_CUBE_H

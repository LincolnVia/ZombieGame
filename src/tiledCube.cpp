#include "tiledCube.h"   // or whatever your header is called
#include <vector>
#include "raylib.h"
#include "raymath.h"

#define CUBE_VERTICES 24
#define CUBE_INDICES 36



// Rotate point around Y axis helper
Vector3 RotatePointY(Vector3 point, float angle) {
    float cosA = cosf(angle);
    float sinA = sinf(angle);
    return {
        point.x * cosA - point.z * sinA,
        point.y,
        point.x * sinA + point.z * cosA
    };
}

// Add oriented cube collider to colliders vector
void AddCubeCollider(std::vector<AABB>& colliders, Vector3 position, float size, float rotationY = 0.0f) {
    float half = size * 0.5f;
    Vector3 corners[8] = {
        {-half, -half, -half}, {half, -half, -half}, {half, -half, half}, {-half, -half, half},
        {-half, half, -half},  {half, half, -half},  {half, half, half},  {-half, half, half}
    };
    for (int i = 0; i < 8; i++) { 
        corners[i] = RotatePointY(corners[i], rotationY);
        corners[i] = Vector3Add(corners[i], position);
    }
    Vector3 min = corners[0];
    Vector3 max = corners[0];
    for (int i = 1; i < 8; i++) {
        if (corners[i].x < min.x) min.x = corners[i].x;
        if (corners[i].y < min.y) min.y = corners[i].y;
        if (corners[i].z < min.z) min.z = corners[i].z;
        if (corners[i].x > max.x) max.x = corners[i].x;
        if (corners[i].y > max.y) max.y = corners[i].y;
        if (corners[i].z > max.z) max.z = corners[i].z;
    }
    colliders.push_back({min, max});
}

// Generate tiled cube mesh implementation (as you wrote it, just fix your `#define`s if needed)
Mesh GenTiledCubeMesh(Vector3 halfSize, float tileRepeat)
{
    Mesh mesh = {0};
    mesh.vertexCount = CUBE_VERTICES;
    mesh.triangleCount = CUBE_INDICES / 3;

    mesh.vertices = (float*)RL_MALLOC(mesh.vertexCount * 3 * sizeof(float));
    mesh.texcoords = (float*)RL_MALLOC(mesh.vertexCount * 2 * sizeof(float));
    mesh.normals = (float*)RL_MALLOC(mesh.vertexCount * 3 * sizeof(float));
    mesh.indices = (unsigned short*)RL_MALLOC(CUBE_INDICES * sizeof(unsigned short));

    float hx = halfSize.x;
    float hy = halfSize.y;
    float hz = halfSize.z;

    float vertices[CUBE_VERTICES * 3] = {
        // Front face (+Z)
        -hx, -hy,  hz,
         hx, -hy,  hz,
         hx,  hy,  hz,
        -hx,  hy,  hz,
        // Back face (-Z)
         hx, -hy, -hz,
        -hx, -hy, -hz,
        -hx,  hy, -hz,
         hx,  hy, -hz,
        // Top face (+Y)
        -hx,  hy,  hz,
         hx,  hy,  hz,
         hx,  hy, -hz,
        -hx,  hy, -hz,
        // Bottom face (-Y)
        -hx, -hy, -hz,
         hx, -hy, -hz,
         hx, -hy,  hz,
        -hx, -hy,  hz,
        // Right face (+X)
         hx, -hy,  hz,
         hx, -hy, -hz,
         hx,  hy, -hz,
         hx,  hy,  hz,
        // Left face (-X)
        -hx, -hy, -hz,
        -hx, -hy,  hz,
        -hx,  hy,  hz,
        -hx,  hy, -hz,
    };
    for (int i = 0; i < mesh.vertexCount * 3; i++)
        mesh.vertices[i] = vertices[i];

    // UV coordinates, tiled by tileRepeat (same for all faces)
    float texcoords[CUBE_VERTICES * 2];
    for (int face = 0; face < 6; face++) {
        int offset = face * 4;
        texcoords[(offset + 0) * 2 + 0] = 0.0f;        texcoords[(offset + 0) * 2 + 1] = 0.0f;
        texcoords[(offset + 1) * 2 + 0] = tileRepeat;  texcoords[(offset + 1) * 2 + 1] = 0.0f;
        texcoords[(offset + 2) * 2 + 0] = tileRepeat;  texcoords[(offset + 2) * 2 + 1] = tileRepeat;
        texcoords[(offset + 3) * 2 + 0] = 0.0f;        texcoords[(offset + 3) * 2 + 1] = tileRepeat;
    }
    for (int i = 0; i < mesh.vertexCount * 2; i++)
        mesh.texcoords[i] = texcoords[i];

    // Normals per face (same as before)
    float normals[CUBE_VERTICES * 3] = {
        0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1,
        0, 0,-1, 0, 0,-1, 0, 0,-1, 0, 0,-1,
        0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0,
        0,-1, 0, 0,-1, 0, 0,-1, 0, 0,-1, 0,
        1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,
       -1, 0, 0,-1, 0, 0,-1, 0, 0,-1, 0, 0,
    };
    for (int i = 0; i < mesh.vertexCount * 3; i++)
        mesh.normals[i] = normals[i];

    unsigned short indices[CUBE_INDICES] = {
         0,  1,  2,  0,  2,  3,
         4,  5,  6,  4,  6,  7,
         8,  9, 10,  8, 10, 11,
        12, 13, 14, 12, 14, 15,
        16, 17, 18, 16, 18, 19,
        20, 21, 22, 20, 22, 23
    };
    for (int i = 0; i < CUBE_INDICES; i++)
        mesh.indices[i] = indices[i];

    UploadMesh(&mesh, false);
    return mesh;
    
}// Create mesh+material, add collider
MeshWithMaterial CreateTiledCubeWithColliderAndTexture(std::vector<AABB>& colliders, Vector3 position, Vector3 halfSize, int tileRepeat, Texture2D texture)
{
    MeshWithMaterial result;
    result.mesh = GenTiledCubeMesh(halfSize, tileRepeat);
    result.position = position;
    result.material = LoadMaterialDefault();
    result.material.maps[MATERIAL_MAP_ALBEDO].texture = texture;
    result.material.maps[MATERIAL_MAP_ALBEDO].color = WHITE;

    // Add AABB collider
    AABB collider = {
        Vector3Subtract(position, halfSize),
        Vector3Add(position, halfSize)
    };
    colliders.push_back(collider);

    return result;
}

// Draw mesh + material at position (translation only)
void DrawMeshAtPosition(const MeshWithMaterial& meshMat, Vector3 position)
{
    Matrix transform = MatrixTranslate(position.x, position.y, position.z);
    DrawMesh(meshMat.mesh, meshMat.material, transform);
}// Draw mesh+material at position (translation only)

#pragma once
#include "raylib.h"
#include <vector>
#include "collision.h"

class Player {
public:
    Player(Vector3 startPos);

    void Update(float deltaTime, const std::vector<AABB>& colliders);
    void Draw() const;
    Camera3D GetCamera() const { return camera; }

private:
    Vector3 position;
    Vector3 velocity = {0,0,0};
    float yaw = 0;
    float pitch = 0;

    float moveSpeed = 5.0f;
    float gravity = 9.8f;
    float jumpForce = 5.0f;
    float playerHeight = 1.8f;
    float playerRadius = 0.3f;
    bool grounded = false;
    float mouseSensitivity = 0.003f;

    Camera3D camera;

    bool CheckBoxCollision(const BoundingBox& a, const BoundingBox& b);
    void TryMove(Vector3 delta, const std::vector<AABB>& colliders);
    bool CheckGround(const std::vector<AABB>& colliders);
};

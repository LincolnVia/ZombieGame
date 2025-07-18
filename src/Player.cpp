// Player.cpp
#include "Player.h"
#include "raymath.h"

Player::Player(Vector3 startPos) {
    position = startPos;
    camera.position = position;
    camera.target = Vector3Add(position, (Vector3){0,0,1});
    camera.up = {0,1,0};
    camera.fovy = 70;
    camera.projection = CAMERA_PERSPECTIVE;
}

void Player::Update(float deltaTime, const std::vector<AABB>& colliders) {
    // Mouse look
    Vector2 mouseDelta = GetMouseDelta();
    yaw -= mouseDelta.x * mouseSensitivity;
    pitch -= mouseDelta.y * mouseSensitivity;
    if (pitch > PI/2 - 0.01f) pitch = PI/2 - 0.01f;
    if (pitch < -PI/2 + 0.01f) pitch = -PI/2 + 0.01f;

    Vector3 forward = {
        cosf(pitch) * sinf(yaw),
        sinf(pitch),
        cosf(pitch) * cosf(yaw)
    };
    Vector3 right = Vector3Normalize(Vector3CrossProduct((Vector3){0,1,0}, forward));
    forward = Vector3Normalize(forward);

    // Input
    Vector3 input = {0,0,0};
    if (IsKeyDown(KEY_W)) input = Vector3Add(input, forward);
    if (IsKeyDown(KEY_S)) input = Vector3Subtract(input, forward);
    if (IsKeyDown(KEY_D)) input = Vector3Subtract(input, right);
    if (IsKeyDown(KEY_A)) input = Vector3Add(input, right);

    if (Vector3Length(input) > 0) input = Vector3Normalize(input);

    Vector3 move = Vector3Scale(input, moveSpeed * deltaTime);

    // Horizontal movement with collision (X and Z)
    TryMove({move.x, 0, 0}, colliders);
    TryMove({0, 0, move.z}, colliders);

    // Gravity
    if (!grounded) {
        velocity.y -= gravity * deltaTime;
    }

    // Jump
    if (grounded && IsKeyPressed(KEY_SPACE)) {
        velocity.y = jumpForce;
        grounded = false;
    }

    // Vertical movement with collision (Y)
    TryMove({0, velocity.y * deltaTime, 0}, colliders);

    // Update grounded status and reset vertical velocity if grounded
    grounded = CheckGround(colliders);
    if (grounded) velocity.y = 0;

    // Update camera
    camera.position = Vector3Add(position, (Vector3){0, playerHeight * 0.9f, 0});
    camera.target = Vector3Add(camera.position, forward);
}

void Player::Draw() const {
    Vector3 cubePos = Vector3Add(position, (Vector3){0, playerHeight/2, 0});
    DrawCube(cubePos, playerRadius*2, playerHeight, playerRadius*2, GREEN);
}

bool Player::CheckBoxCollision(const BoundingBox& a, const BoundingBox& b) {
    return CheckCollisionBoxes(a, b);
}

void Player::TryMove(Vector3 delta, const std::vector<AABB>& colliders) {
    Vector3 newPos = Vector3Add(position, delta);

    BoundingBox playerBox = {
        {newPos.x - playerRadius, newPos.y, newPos.z - playerRadius},
        {newPos.x + playerRadius, newPos.y + playerHeight, newPos.z + playerRadius}
    };

    for (const auto& box : colliders) {
        BoundingBox boxBB = { box.min, box.max };
        if (CheckBoxCollision(playerBox, boxBB)) {
            // Collision detected - cancel movement on that axis
            if (delta.x != 0) {
                newPos.x = position.x; // revert X
            }
            if (delta.y != 0) {
                newPos.y = position.y; // revert Y
                velocity.y = 0;       // reset vertical velocity on collision
            }
            if (delta.z != 0) {
                newPos.z = position.z; // revert Z
            }
            position = newPos;
            return;
        }
    }
    position = newPos;
}

bool Player::CheckGround(const std::vector<AABB>& colliders) {
    Vector3 testPos = Vector3Add(position, (Vector3){0, -0.1f, 0});
    BoundingBox playerBox = {
        {testPos.x - playerRadius, testPos.y, testPos.z - playerRadius},
        {testPos.x + playerRadius, testPos.y + 0.1f, testPos.z + playerRadius}
    };
    for (const auto& box : colliders) {
        BoundingBox boxBB = { box.min, box.max };
        if (CheckBoxCollision(playerBox, boxBB)) {
            return true;
        }
    }
    return false;
}

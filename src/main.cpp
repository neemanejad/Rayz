// TestRaylib.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <random>
#include <time.h>

typedef struct RectangleEntity {
    Vector2 location;
    Vector2 size;
    Color color;
} RectangleEntity;

typedef struct Entity {
    void* entity;
    void (*updateEntity)(void* entity, float deltaTime);
    void (*drawEntity)(void* entity, float deltaTime);
} Entity;

Vector2 GetOrigin(RectangleEntity *entity);
void HandleKeyboardInput(float deltaTime, RectangleEntity *entity);
void UpdateRectangleEntity(void* entity, float deltaTime);
void DrawRectangleEntity(void* entity, float deltaTime);

float movementSpeed = 300;
Camera2D camera = { 0 };


int main()
{
    InitWindow(1200, 800, "raylib [core] example - basic window");

    RectangleEntity* rectangleEntity = (RectangleEntity*)malloc(sizeof(RectangleEntity));
    if (rectangleEntity) {
        rectangleEntity->location = Vector2{ 20, 20 };
        rectangleEntity->size = Vector2{ 40, 40 };
        rectangleEntity->color = BLACK;
    }
    else {
        exit(1);
    }

    const int backgroundRectangleSize = 50;
    srand(time(NULL));
    RectangleEntity backgroundRectangles[backgroundRectangleSize];
    for (int i = 0; i < backgroundRectangleSize; i++) {
        Vector2 location = Vector2{ (float)(rand() % GetScreenWidth()), (float)(rand() % GetScreenHeight()) };
        Vector2 size = Vector2{ (float)(rand() % 100) + 10, (float)(rand() % 100) + 10 };
        backgroundRectangles[i] = RectangleEntity{ location, size, Color{ (unsigned char)(rand() % 256), (unsigned char)(rand() % 256), (unsigned char)(rand() % 256), 255} };
    }

    camera.target = GetOrigin(rectangleEntity);
    camera.offset = Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    camera.zoom = 1.0f;
    camera.rotation = 0.0f;

    Entity entities[1];
    entities[0] = Entity{ rectangleEntity, UpdateRectangleEntity, DrawRectangleEntity };
    
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        int entitiesLength = sizeof(entities) / sizeof(entities[0]);

        // Update entities
        for (int i = 0; i < entitiesLength; i++) {
            entities[i].updateEntity(entities[i].entity, deltaTime);
        }

        Vector2 origin = GetOrigin(rectangleEntity);
        camera.target = rectangleEntity->location;

        Vector2 mousePosition = GetScreenToWorld2D(GetMousePosition(), camera);

        BeginDrawing();
            ClearBackground(RAYWHITE);
            
            BeginMode2D(camera);
           
                // Draw entities
                for (int i = 0; i < entitiesLength; i++) {
                    entities[i].drawEntity(entities[i].entity, deltaTime);
                }

                for (int i = 0; i < backgroundRectangleSize; i++) {
                    DrawRectangle(
                        backgroundRectangles[i].location.x,
                        backgroundRectangles[i].location.y, 
                        backgroundRectangles[i].size.x, 
                        backgroundRectangles[i].size.y, 
                        backgroundRectangles[i].color
                    );
                }

            
                DrawLineEx(rectangleEntity->location, mousePosition, 2, RED);

            EndMode2D();

            DrawFPS(10.0f, 10.0f);
            DrawLine(GetScreenWidth() / 2.0f, 0, GetScreenWidth() / 2.0f, GetScreenHeight(), GREEN);
            DrawLine(0, GetScreenHeight() / 2.0f, GetScreenWidth(), GetScreenHeight() / 2.0f, GREEN);
        EndDrawing();

    }

    CloseWindow();

    free(rectangleEntity);

    return 0;
}

Vector2 GetOrigin(RectangleEntity *entity) {
    return Vector2
    {
        entity->location.x + (entity->size.x / 2.0f),
        entity->location.y + (entity->size.y / 2.0f)
    };
}

void HandleKeyboardInput(float deltaTime, RectangleEntity* entity) {
    Vector2 direction = { 0 };
    if (IsKeyDown(KEY_A)) {
        direction.x -= 1;
    }
    if (IsKeyDown(KEY_S)) {
        direction.y += 1;
    }
    if (IsKeyDown(KEY_D)) {
        direction.x += 1;
    }
    if (IsKeyDown(KEY_W)) {
        direction.y -= 1;
    }

    if (direction.x == 0 && direction.y == 0) {
        return;
    }

    Vector2 normalizedDirection = Vector2Normalize(direction);
    entity->location = Vector2Add(Vector2{ normalizedDirection.x * movementSpeed * deltaTime, normalizedDirection.y * movementSpeed * deltaTime }, entity->location);
}

void UpdateRectangleEntity(void* entity, float deltaTime)
{
    HandleKeyboardInput(deltaTime, (RectangleEntity*)entity);
}

void DrawRectangleEntity(void* entity, float deltaTime)
{
    RectangleEntity* rectangleEntity = (RectangleEntity*)entity;
    Rectangle rect = { rectangleEntity->location.x, rectangleEntity->location.y, rectangleEntity->size.x, rectangleEntity->size.y };
    DrawRectanglePro(rect, Vector2{ rectangleEntity->size.x / 2.0f, rectangleEntity->size.y / 2.0f }, 0.0f, BLACK);
}

#include <raylib.h>
#include <raymath.h>

#include "entity.h"
#include "logging.h"
#include "memory/dict.h"
#include "memory/pool.h"
#include "win32.h"

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Spritesheet {
    Texture2D texture;
    int sprite_width;
    int sprite_height;
} Spritesheet;

#define Dict(Type, Capacity)      \
    struct {                      \
        size_t indices[Capacity]; \
        int keys[Capacity];       \
        Type values[Capacity];    \
        size_t size;              \
    }

#define MAX_POSITION_COMPONENTS (int)128
typedef struct ComponentManager {
    Dict(Vector2, MAX_POSITION_COMPONENTS) positions;
} ComponentManager;

#define MAX_TEXTURES (int)128
typedef struct TexturePool {
    int keys[MAX_TEXTURES];
    Texture2D values[MAX_TEXTURES];
    int size;
} TexturePool;

#define MAX_SPRITESHEETS (int)128
typedef struct SpritesheetPool {
    int keys[MAX_SPRITESHEETS];
    Spritesheet values[MAX_SPRITESHEETS];
    int size;
} SpritesheetPool;

// TODO:
// Store `TextureID -> Texture2D`
// Get rid of Spritesheet somehow?
//
// What we want: to render the player and coffee and donut
// The coffee and donut are spinning
// The spinning animation is done by changing what part of the spritesheet texture we render
// Can't we just store a texture and a clip_rect as a component? "Texture2DComponent"?
//
// Goals:
// - Don't have two cases, texture and spritesheet, for rendering. Just one case: texture
// - Don't copy texture, 2 coffees should use the same 1 coffee texture

// UFO 50 is 16:9 at 384x216 resolution
#define RESOLUTION_WIDTH (int)768
#define RESOLUTION_HEIGHT (int)432

void ComponentManager_add_position(ComponentManager* components, EntityID id, Vector2 position) {
    Dict_insert(&components->positions, id.value, position);
}

void ComponentManager_get_position(ComponentManager* components, EntityID id, Vector2* position) {
    Dict_get(&components->positions, id.value, position);
}

void ComponentManager_set_position(ComponentManager* components, EntityID id, Vector2 position) {
    Dict_set(&components->positions, id.value, position);
}

//

void TexturePool_add_texture(TexturePool* pool, EntityID id, Texture2D texture) {
    Pool_add_item(pool, id.value, texture);
}

void TexturePool_get_texture(TexturePool* pool, EntityID id, Texture2D* texture) {
    Pool_get_item(pool, id.value, texture);
}

void TexturePool_set_texture(TexturePool* pool, EntityID id, Texture2D texture) {
    Pool_set_item(pool, id.value, texture);
}

void SpritesheetPool_add_spritesheet(SpritesheetPool* pool, EntityID id, Spritesheet spritesheet) {
    Pool_add_item(pool, id.value, spritesheet);
}

void SpritesheetPool_get_spritesheet(SpritesheetPool* pool, EntityID id, Spritesheet* spritesheet) {
    Pool_get_item(pool, id.value, spritesheet);
}

void SpritesheetPool_set_spritesheet(SpritesheetPool* pool, EntityID id, Spritesheet spritesheet) {
    Pool_set_item(pool, id.value, spritesheet);
}

Texture2D load_texture_from_file(const char* filename) {
    Image image = LoadImage(filename);
    if (image.data == NULL) {
        return (Texture2D) { 0 };
    }
    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);
    return texture;
}

void draw_sprite_centered(Spritesheet spritesheet, int sprite_index, Vector2 position, Color tint) {
    Rectangle rect = {
        .x = spritesheet.sprite_width * sprite_index,
        .y = spritesheet.sprite_height * sprite_index,
        .width = spritesheet.sprite_width,
        .height = spritesheet.sprite_height,
    };
    Vector2 centered_position = {
        .x = position.x - rect.width / 2,
        .y = position.y - rect.height / 2,
    };
    DrawTextureRec(spritesheet.texture, rect, centered_position, tint);
}

void draw_texture_centered(Texture2D texture, Vector2 position, Color tint) {
    Vector2 centered_position = {
        .x = position.x - texture.width / 2,
        .y = position.y - texture.height / 2,
    };
    DrawTexture(texture, centered_position.x, centered_position.y, tint);
}

int compare_position_ids_by_y_coordinate(void* ctx, const void* lhs, const void* rhs) {
    ComponentManager* components = (ComponentManager*)ctx;
    EntityID lhs_id = *(const EntityID*)lhs;
    EntityID rhs_id = *(const EntityID*)rhs;
    Vector2 lhs_pos = { 0 };
    Vector2 rhs_pos = { 0 };
    ComponentManager_get_position(components, lhs_id, &lhs_pos);
    ComponentManager_get_position(components, rhs_id, &rhs_pos);
    if (lhs_pos.y < rhs_pos.y) {
        return -1;
    } else if (lhs_pos.y == rhs_pos.y) {
        return 0;
    } else {
        return 1;
    }
}

int main(void) {
    /* Init */
    initialize_logging();
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(RESOLUTION_WIDTH, RESOLUTION_HEIGHT, "Program");
    LOG_INFO("Created window");

    // Render texture
    RenderTexture2D screen_texture = LoadRenderTexture(RESOLUTION_WIDTH, RESOLUTION_HEIGHT);

    /* Load resources */
    Spritesheet donut_spritesheet = {
        .texture = load_texture_from_file("resource/image/spinning_donut.png"),
        .sprite_width = 64,
        .sprite_height = 64,
    };
    Spritesheet coffee_spritesheet = {
        .texture = load_texture_from_file("resource/image/spinning_coffee.png"),
        .sprite_width = 64,
        .sprite_height = 64,
    };

    /* State */
    bool show_debug_overlay = false;
    ComponentManager components = { 0 };
    TexturePool textures = { 0 };
    SpritesheetPool spritesheets = { 0 };
    EntityID player_id = EntityID_new();
    EntityID donut_id = EntityID_new();
    EntityID coffee_id = EntityID_new();

    // add player
    ComponentManager_add_position(&components, player_id, Vector2Zero());
    TexturePool_add_texture(&textures, player_id, load_texture_from_file("resource/image/pill.png"));

    // add donut
    ComponentManager_add_position(&components, donut_id, (Vector2) { -48, 0 });
    SpritesheetPool_add_spritesheet(&spritesheets, donut_id, donut_spritesheet);

    // add coffee
    ComponentManager_add_position(&components, coffee_id, (Vector2) { 48, 0 });
    SpritesheetPool_add_spritesheet(&spritesheets, coffee_id, coffee_spritesheet);

    /* Run program */
    while (!WindowShouldClose()) {
        // retreive player position
        Vector2 player_pos = Vector2Zero();
        ComponentManager_get_position(&components, player_id, &player_pos);

        /* Update */
        {
            if (IsKeyPressed(KEY_F11)) {
                toggle_fullscreen();
            }

            if (IsKeyPressed(KEY_F3)) {
                show_debug_overlay = !show_debug_overlay;
            }

            Vector2 input_vec = Vector2Zero();
            if (IsKeyDown('A')) {
                input_vec = Vector2Add(input_vec, (Vector2) { -1, 0 });
            }
            if (IsKeyDown('D')) {
                input_vec = Vector2Add(input_vec, (Vector2) { 1, 0 });
            }
            if (IsKeyDown('W')) {
                input_vec = Vector2Add(input_vec, (Vector2) { 0, -1 });
            }
            if (IsKeyDown('S')) {
                input_vec = Vector2Add(input_vec, (Vector2) { 0, 1 });
            }
            input_vec = Vector2Normalize(input_vec);

            const float delta_time = GetFrameTime();
            const float player_speed = 200; // px / second
            const Vector2 moved_player_pos = Vector2Add(player_pos, Vector2Scale(input_vec, delta_time * player_speed));
            ComponentManager_set_position(&components, player_id, moved_player_pos);
        }

        /* Render scene */
        BeginTextureMode(screen_texture);
        {
            // Draw background
            ClearBackground(LIME);

            const int time_now = GetTime() * 1000; // ms
            const int frame_time = 70; // ms
            const int num_frames = 12;
            const int sprite_index = (time_now % (num_frames * frame_time)) / frame_time;
            const Vector2 screen_middle = {
                .x = RESOLUTION_WIDTH / 2,
                .y = RESOLUTION_HEIGHT / 2,
            };

            // sort entities based on position
            EntityID y_sorted_entities[MAX_POSITION_COMPONENTS] = { 0 };
            memcpy(y_sorted_entities, components.positions.keys, MAX_POSITION_COMPONENTS * sizeof(EntityID));
            qsort_s(y_sorted_entities, components.positions.size, sizeof(EntityID), compare_position_ids_by_y_coordinate, &components);

            // render entities
            for (int i = 0; i < components.positions.size; i++) {
                EntityID id = y_sorted_entities[i];
                Vector2 position = { 0 };
                ComponentManager_get_position(&components, id, &position);
                Vector2 centered_pos = Vector2Add(position, screen_middle);

                Texture2D texture = { 0 };
                TexturePool_get_texture(&textures, id, &texture);
                if (texture.id != 0) {
                    draw_texture_centered(texture, centered_pos, WHITE);
                    continue;
                }

                Spritesheet spritesheet = { 0 };
                SpritesheetPool_get_spritesheet(&spritesheets, id, &spritesheet);
                if (spritesheet.texture.id != 0) {
                    draw_sprite_centered(spritesheet, sprite_index, centered_pos, WHITE);
                }
            }

            // Draw FPS
            if (show_debug_overlay) {
                char text[128] = { 0 };
                snprintf(text, 128, "FPS: %d", GetFPS());
                DrawText(text, 0, 0, 16, WHITE);
            }
        }
        EndTextureMode();

        /* Render window */
        BeginDrawing();
        {
            // Draw background
            ClearBackground(BLACK);

            // Draw stretched screen texture
            int screen_width = GetScreenWidth();
            int screen_height = GetScreenHeight();
            int scale = min(screen_width / RESOLUTION_WIDTH, screen_height / RESOLUTION_HEIGHT);
            int scaled_width = scale * screen_texture.texture.width;
            int scaled_height = scale * screen_texture.texture.height;
            Rectangle scaled_screen_rect = {
                .x = (screen_width - scaled_width) / 2,
                .y = (screen_height - scaled_height) / 2,
                .width = scaled_width,
                .height = scaled_height,
            };
            DrawTexturePro(
                screen_texture.texture, (Rectangle) { 0, 0, screen_texture.texture.width, -screen_texture.texture.height }, scaled_screen_rect, Vector2Zero(), 0, WHITE
            );
        }
        EndDrawing();
    }

    /* Shutdown */
    LOG_INFO("Shutdown");
    CloseWindow();
    return 0;
}

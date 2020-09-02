#pragma once

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

constexpr int FPS = 60;
constexpr int MILLISECONDS_PER_FRAME = 1000 / FPS;

constexpr int MAP_TEXTURE_SIZE_X = 224;
constexpr int MAP_TEXTURE_SIZE_Y = 176;

constexpr int TILE_SIZE = 16;
constexpr int TILE_SCALE = 2;

constexpr int MAP_SIZE_X = 100;
constexpr int MAP_SIZE_Y = 30;

constexpr unsigned int WORLD_MAP_X = MAP_SIZE_X * TILE_SIZE * TILE_SCALE;
constexpr unsigned int WORLD_MAP_Y = MAP_SIZE_Y * TILE_SIZE * TILE_SCALE;

constexpr unsigned int second_to_millisecond = 1000;

constexpr float PI = 3.14159f;
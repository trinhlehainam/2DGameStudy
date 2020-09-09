#pragma once

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

constexpr unsigned int FPS = 60;

constexpr int MAP_TEXTURE_SIZE_X = 224;
constexpr int MAP_TEXTURE_SIZE_Y = 176;

constexpr int TILE_SIZE = 16;
constexpr int TILE_SCALE = 2;

constexpr int NUM_TILE_X = 250;
constexpr int NUM_TILE_Y = 150;

constexpr unsigned int WORLD_MAP_X = NUM_TILE_X * TILE_SIZE * TILE_SCALE;
constexpr unsigned int WORLD_MAP_Y = NUM_TILE_Y * TILE_SIZE * TILE_SCALE;

constexpr unsigned int second_to_millisecond = 1000;

constexpr float PI = 3.1415926535897932384626433832795f;
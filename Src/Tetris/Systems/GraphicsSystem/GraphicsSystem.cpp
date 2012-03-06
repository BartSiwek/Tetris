/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#include "Tetris/Systems/GraphicsSystem/GraphicsSystem.h"

#include <SDL/SDL.h>

#include <algorithm>
#include <stdexcept>
#include <cmath>

#include "Elg/Allocators/GlobalAllocator.h"
#include "Elg/EntityManager/EntityManager.h"
#include "Elg/AssetManager/AssetManager.h"

#include "Tetris/Helpers/Constants.h"
#include "Tetris/Systems/GraphicsSystem/GraphicsSystemProfiling.h"

GraphicsSystem::GraphicsSystem(TetrisEntityManager *entity_manager, TetrisAssetManager *asset_manager)
    : m_entity_manager_(entity_manager), m_asset_manager_(asset_manager), m_screen_(NULL) {
}

GraphicsSystem::~GraphicsSystem() {
}

void GraphicsSystem::Initialize() {
  // Attempt to create a 800x600 window with 32bit pixels.
  m_screen_ = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE);

  // If we fail, throw an exception
  if (m_screen_ == NULL) {
    throw std::runtime_error(SDL_GetError());
  }

  // Get the asset
  uint32 background_hash = ELG_STRING_HASH("BACKGROUND");
  m_background_asset_ = m_asset_manager_->Get<Elg::AssetManager::Assets::Image>(background_hash);
}

void GraphicsSystem::Render(float32 /* interpolationFactor */) {
  TETRIS_PROFILE_GRAPHICSSYSTEM_RENDER;

  // State state = currentState*interpolationFactor + previousState * (1.0 - interpolationFactor) -> Render with state
  if (SDL_MUSTLOCK(m_screen_)) {
    if (SDL_LockSurface(m_screen_) < 0) {
      return;
    }
  }

  // Render background
  RenderBackground();

  // Render entities
  RenderEntities();

  if (SDL_MUSTLOCK(m_screen_)) {
    SDL_UnlockSurface(m_screen_);
  }

  SDL_UpdateRect(m_screen_, 0, 0, m_screen_->w, m_screen_->h);

  SDL_Flip(m_screen_);
}

void GraphicsSystem::RenderBackground() {
  Elg::AssetManager::Assets::Image* background = m_background_asset_.Get();
  RenderImage(0, 0, background);
}

void GraphicsSystem::RenderEntities() {
  uint32 component_count = m_entity_manager_->GetDataCount<Graphics>();
  Graphics** components = ElgNew Graphics*[component_count];
  Graphics** components_end = components + component_count;

  ASSERT(components != NULL, "Memory allocation for temporary Graphics components array failed");

  for (uint32 component_index = 0;component_index < component_count;++component_index) {
    components[component_index] = m_entity_manager_->GetData<Graphics>() + component_index;
  }

  std::sort(components, components_end, GraphicsComponentComparator());

  uint32 start = 0;
  uint32 end = 0;
  while (start < component_count) {
    TetrominoRotationType current_representation = components[start]->Representation;
    while (end < component_count && components[end]->Representation == current_representation) {
      ++end;
    }
    RenderRepresentation(components, current_representation, start, end);
    start = end;
  }

  ElgDelete[] components;
}

void GraphicsSystem::RenderImage(int32 screen_position_x, int32 screen_position_y,
                                 Elg::AssetManager::Assets::Image* image) {
  // Fetch some basic parameters
  int32 image_width = image->width();
  int32 image_height = image->height();
  int32 screen_width = m_screen_->w;
  int32 screen_height = m_screen_->h;

  // Image top-left corner and bottom-right corner
  int32 image_top_left_x = screen_position_x;
  int32 image_top_left_y = screen_position_y;
  int32 image_bottom_right_x = screen_position_x + image_width;
  int32 image_bottom_right_y = screen_position_y + image_height;

  // Fetch channel information
  int32 image_color_channel_count = image->channel_count();
  int32 screen_color_channel_count = m_screen_->format->BytesPerPixel;

  ASSERT(image_color_channel_count == screen_color_channel_count,
         "Screen color channel count differs from image color channel count");
  if (image_color_channel_count == screen_color_channel_count &&
      image_bottom_right_x > 0 && screen_width > image_top_left_x &&
      image_bottom_right_y > 0 && screen_height > image_top_left_y) {
    // Figure out the intersection of two rectangles - screen and positioned image
    int32 area_top_left_x = max(image_top_left_x, 0);
    int32 area_top_left_y = max(image_top_left_y, 0);
    int32 area_bottom_right_x = min(image_bottom_right_x, screen_width);
    int32 area_bottom_right_y = min(image_bottom_right_y, screen_height);

    // Translate above computation to pair of screen coordinates & pair of image coordinates & width & height
    int32 area_width = area_bottom_right_x - area_top_left_x;
    int32 area_height = area_bottom_right_y - area_top_left_y;

    // Get the in image positions
    int32 image_position_x = area_top_left_x - screen_position_x;
    int32 image_position_y = area_top_left_y - screen_position_y;

    // Do the drawing
    ApplySubimageToArea(image, image_position_x, image_position_y, area_top_left_x, area_top_left_y,
                        area_width, area_height);
  }
}

void GraphicsSystem::ApplySubimageToArea(Elg::AssetManager::Assets::Image* image, int32 image_clip_x,
                                         int32 image_clip_y, int32 screen_area_x, int32 screen_area_y, int32 width,
                                         int32 height) {
  // Get basic parameters
  int32 channel_count = image->channel_count();
  int32 screen_row_size = m_screen_->pitch;
  int32 image_row_size = image->pitch();

  // Get data
  unsigned char* screen_pixels = static_cast<unsigned char*>(m_screen_->pixels);
  unsigned char* image_pixels = image->data();

  // Get initial aligns and such
  int32 screen_byte_offset = screen_area_y * screen_row_size +
                             screen_area_x * channel_count;
  int32 image_byte_offset = image_clip_y * image_row_size +
                            image_clip_x * channel_count;
  int32 ammount_to_copy = width * channel_count;

  // Do the copy
  int32 pixel_row = 0;
  while (pixel_row < height) {
    std::memcpy(screen_pixels + screen_byte_offset, image_pixels + image_byte_offset, ammount_to_copy);

    screen_byte_offset += screen_row_size;
    image_byte_offset += image_row_size;
    ++pixel_row;
  }
}

bool GraphicsSystem::GraphicsComponentComparator::operator()(const Graphics* lhs, const Graphics* rhs) {
  return (lhs->Representation > rhs->Representation);
}

void GraphicsSystem::RenderRepresentation(Graphics** components, TetrominoRotationType r,
                                          uint32 start, uint32 end) {
  switch (r) {
    case L_ROT0:
    case L_ROT1:
    case L_ROT2:
    case L_ROT3:
    case INV_L_ROT0:
    case INV_L_ROT1:
    case INV_L_ROT2:
    case INV_L_ROT3:
    case I_ROT0:
    case I_ROT1:
    case S_ROT0:
    case S_ROT1:
    case INV_S_ROT0:
    case INV_S_ROT1:
    case T_ROT0:
    case T_ROT1:
    case T_ROT2:
    case T_ROT3:
    case O_ROT0:
      RenderTetromino(components, start, end, Constants::TETROMINO_ROTATION_TO_OFFSETS[r]);
      break;
    case BOARD:
      RenderSimple(components, start, end);
      break;
    case PREVIEW:
      RenderSimple(components, start, end);
      break;
    case TILE:
      RenderSimple(components, start, end);
      break;
  }
}

void GraphicsSystem::RenderSimple(Graphics** components, int32 start, int32 end) {
  for (int32 component_index = start;component_index < end;++component_index) {
    Elg::AssetManager::Asset<TetrisAssetManager, Elg::AssetManager::Assets::Image> representationAsset =
      m_asset_manager_->Get<Elg::AssetManager::Assets::Image>(components[component_index]->RepresentationAssetIdHash);
    Elg::AssetManager::Assets::Image* image = representationAsset.Get();
    int32 x = components[component_index]->ScreenX;
    int32 y = components[component_index]->ScreenY;
    RenderImage(x, y, image);
  }
}

void GraphicsSystem::RenderTetromino(Graphics** components, uint32 start, uint32 end, const BoardOffset offsets[4]) {
  for (uint32 component_index = start;component_index < end;++component_index) {
    Elg::AssetManager::Asset<TetrisAssetManager, Elg::AssetManager::Assets::Image> representationAsset =
      m_asset_manager_->Get<Elg::AssetManager::Assets::Image>(components[component_index]->RepresentationAssetIdHash);
    Elg::AssetManager::Assets::Image* image = representationAsset.Get();
    for (uint32 offset_index = 0;offset_index < 4;++offset_index) {
      int32 offset_x = offsets[offset_index].column_offset - Constants::TETROMINO_RENDERING_OFFSET_COLUMN;
      int32 offset_y = offsets[offset_index].row_offset - Constants::TETROMINO_RENDERING_OFFSET_ROW;
      int32 x = components[component_index]->ScreenX + offset_x * Constants::TILE_WIDTH;
      int32 y = components[component_index]->ScreenY + offset_y * Constants::TILE_HEIGHT;
      RenderImage(x, y, image);
    }
  }
}

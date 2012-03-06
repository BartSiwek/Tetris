/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#ifndef TETRIS_SYSTEMS_GRAPHICSSYSTEM_GRAPHICSSYSTEM_H_
#define TETRIS_SYSTEMS_GRAPHICSSYSTEM_GRAPHICSSYSTEM_H_

#include <SDL/SDL.h>

#include "Tetris/Components/AllComponents.h"
#include "Tetris/Assets/AllAssets.h"

class GraphicsSystem {
 public:
  // Constructors
  GraphicsSystem(TetrisEntityManager *entity_manager, TetrisAssetManager *asset_manager);

  // Destructors
  ~GraphicsSystem();

  // Methods, including static methods
  void Initialize();
  void Render(float32 interpolationFactor);

 private:
  // Typedefs and enums
  class GraphicsComponentComparator {
   public:
    // Methods, including static methods
    bool operator()(const Graphics* lhs, const Graphics* rhs);
  };

  // Methods, including static methods
  void RenderBackground();
  void RenderEntities();
  void RenderImage(int32 screen_position_x, int32 screen_position_y, Elg::AssetManager::Assets::Image* image);
  void ApplySubimageToArea(Elg::AssetManager::Assets::Image* image, int32 image_clip_x, int32 image_clip_y,
                           int32 screen_area_x, int32 screen_area_y, int32 width, int32 height);
  void RenderRepresentation(Graphics** components, TetrominoRotationType r, uint32 start, uint32 end);
  void RenderSimple(Graphics** components, int32 start, int32 end);
  void RenderTetromino(Graphics** components, uint32 start, uint32 end, const BoardOffset offsets[4]);

  // Data Members
  SDL_Surface *m_screen_;

  Elg::AssetManager::Asset<TetrisAssetManager, Elg::AssetManager::Assets::Image> m_background_asset_;

  TetrisEntityManager *m_entity_manager_;
  TetrisAssetManager *m_asset_manager_;
};

#endif  // TETRIS_SYSTEMS_GRAPHICSSYSTEM_GRAPHICSSYSTEM_H_

/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#ifndef TETRIS_ASSETS_ALLASSETS_H_
#define TETRIS_ASSETS_ALLASSETS_H_

#include <SDL/SDL.h>

#include <vector>

#include "Elg/AssetManager/DescriptionMacros.h"
#include "Elg/AssetManager/AssetManagers/PoolManager.h"
#include "Elg/AssetManager/Loaders/ImageLoader.h"

namespace Elg { namespace AssetManager { class AssetDescriptor; } }

struct TetrisAssetManagerDescription {
  ELG_ASSET_MANAGER_DESCRIPTION_HEADER();

  typedef Elg::AssetManager::Loaders::ImageLoader<SDL_BYTEORDER == SDL_BIG_ENDIAN> PlatformImageLoader;
  typedef Elg::AssetManager::AssetManagers::PoolManager<PlatformImageLoader, 10> ImagePoolManager;
  ELG_ASSET_MANAGER_DESCRIPTION_LOADER(0, ImagePoolManager);

  ELG_ASSET_MANAGER_DESCRIPTION_FOOTER();

  TetrisAssetManagerDescription(const Elg::AssetManager::AssetDescriptor *asset_descriptors,
                                uint32 asset_descriptor_count)
    : ELG_ASSET_MANAGER_DESCRIPTION_INITIALIZERS(1) {
  }
};

// A asset mamanger type declarations
typedef Elg::AssetManager::AssetManager<TetrisAssetManagerDescription> TetrisAssetManager;

// A asset descriptor setup (this is static but usually would be loaded from some file)
Elg::AssetManager::AssetDescriptor* GetAssetDescriptors();
uint32 GetAssetDescriptorCount();

#endif  // TETRIS_ASSETS_ALLASSETS_H_

/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#ifndef ELG_ASSETMANAGER_LOADERS_IMAGELOADER_INL_H_
#define ELG_ASSETMANAGER_LOADERS_IMAGELOADER_INL_H_

#include "Elg/AssetManager/Loaders/ImageLoader.h"

// Include STBI library
#define STBI_HEADER_FILE_ONLY
#include <stb_image/stb_image.c>
#undef STBI_HEADER_FILE_ONLY

#include <functional>
#include <cstring>
#include <string>
#include <cstdio>

#include "Elg/Utilities/StringHash/Hash.h"
#include "Elg/Allocators/GlobalAllocator.h"
#include "Elg/AssetManager/Assets/Image.h"

namespace Elg {
namespace AssetManager {
namespace Loaders {

// Constants
template<bool IsBigEndian>
const uint32 ImageLoader<IsBigEndian>::ASSET_TYPE_ID_HASH = ELG_STRING_HASH("Image");

// Constructors
template<bool IsBigEndian>
ImageLoader<IsBigEndian>::ImageLoader() {
}

// Destructors
template<bool IsBigEndian>
ImageLoader<IsBigEndian>::~ImageLoader() {
}

template<bool IsBigEndian>
Elg::AssetManager::Assets::Image* ImageLoader<IsBigEndian>::Load(std::string filename) {
  // Open files
  FILE *file;
  fopen_s(&file, filename.c_str(), "rb");
  ASSERT(file != 0, "Unable to open asset file [%s]", filename.c_str());

  // Read the data (data is read in a RGBA order - i.e. big endian)
  int32 width, height, component_count;
  unsigned char *data = stbi_load_from_file(file, &width, &height, &component_count, 0);
  fclose(file);

  ASSERT(component_count == 4 || component_count == 3, "Unable to read image data from file [%s]", filename.c_str());

  // Store the data into Image
  Elg::AssetManager::Assets::Image* result = NULL;
  if (component_count == 4) {
    AdjustByteOrder(data, width * height, 4);
    result = ElgNew Elg::AssetManager::Assets::Image(width, height, Elg::AssetManager::Assets::Image::RGBA, data);
    ASSERT(result != NULL, "Memory allocation for new Image failed");
    stbi_image_free(data);
  } else if (component_count == 3) {
    AdjustByteOrder(data, width * height, 3);
    result = ElgNew Elg::AssetManager::Assets::Image(width, height, Elg::AssetManager::Assets::Image::RGB, data);
    ASSERT(result != NULL, "Memory allocation for new Image failed");
    stbi_image_free(data);
  }

  return result;
}

template<bool IsBigEndian>
void ImageLoader<IsBigEndian>::Unload(Elg::AssetManager::Assets::Image* image) {
  ElgDelete image;
}

}  // namespace Loaders
}  // namespace AssetManager
}  // namespace Elg

#endif  // ELG_ASSETMANAGER_LOADERS_IMAGELOADER_INL_H_

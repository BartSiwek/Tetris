/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#ifndef ELG_ASSETMANAGER_LOADERS_IMAGELOADER_H_
#define ELG_ASSETMANAGER_LOADERS_IMAGELOADER_H_

#include <string>

#include "Elg/Utilities/Types.h"

// Forward declarations
namespace Elg { namespace AssetManager { class AssetDescriptor; } }
namespace Elg { namespace AssetManager { namespace Assets { class Image; } } }

// Class declaration
namespace Elg {
namespace AssetManager {
namespace Loaders {

template<bool IsBigEndian>
class ImageLoader {
 public:
  // Constants and enums
  static const uint32 ASSET_TYPE_ID_HASH;

  // Typedefs
  typedef Elg::AssetManager::Assets::Image loaded_type;

  // Constructors
  ImageLoader();

  // Destructors
  virtual ~ImageLoader();

  // Methods, including static methods
  Elg::AssetManager::Assets::Image* Load(std::string filename);
  void Unload(Elg::AssetManager::Assets::Image* image);

 private:
  // Methods, including static methods
  void AdjustByteOrder(unsigned char *data, uint32 pixel_count, uint32 channel_count);

  // Disallow copy and assign
  ImageLoader(const ImageLoader& orig);
  void operator=(const ImageLoader&);
};

}  // namespace Loaders
}  // namespace AssetManager
}  // namespace Elg

#include "Elg/AssetManager/Loaders/ImageLoader-inl.h"

#endif  // ELG_ASSETMANAGER_LOADERS_IMAGELOADER_H_

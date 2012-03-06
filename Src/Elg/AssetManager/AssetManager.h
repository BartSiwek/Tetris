/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#ifndef ELG_ASSETMANAGER_ASSETMANAGER_H_
#define ELG_ASSETMANAGER_ASSETMANAGER_H_

#include <tuple>
#include <string>

#include "Elg/Utilities/Asserts.h"
#include "Elg/Utilities/Types.h"

namespace Elg {
namespace AssetManager {

template<typename AssetManagerType, typename AssetType>
class Asset;

class AssetDescriptor;

template<typename AssetManagerDescription>
class AssetManager {
 public:
  // Constructors
  AssetManager(const AssetDescriptor* asset_descriptors, uint32 asset_descriptor_count);

  // Destructor
  ~AssetManager();

  // Methods, including static methods
  template<typename AssetType> Asset<AssetManager<AssetManagerDescription>, AssetType> Get(uint32 id_hash);

 private:
  // Friendships
  template<typename AssetManagerType, typename AssetType> friend class Asset;

  // Methods, including static methods
  template<typename AssetType> AssetType* GetData(uint32 internal_id);

  // Data Members
  AssetManagerDescription m_description_;

  // Disallow copy and assign
  AssetManager(const AssetManager& orig);
  void operator=(const AssetManager&);
};

}  // namespace AssetManager
}  // namespace Elg

#include "Elg/AssetManager/AssetManager-inl.h"

#endif  // ELG_ASSETMANAGER_ASSETMANAGER_H_

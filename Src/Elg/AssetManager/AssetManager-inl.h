/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#ifndef ELG_ASSETMANAGER_ASSETMANAGER_INL_H_
#define ELG_ASSETMANAGER_ASSETMANAGER_INL_H_

#include "Elg/AssetManager/AssetManager.h"

#include "Elg/Utilities/Macros.h"
#include "Elg/Utilities/Asserts.h"
#include "Elg/AssetManager/Asset.h"
#include "Elg/AssetManager/AssetDescriptor.h"

namespace Elg {
namespace AssetManager {

template<typename AssetManagerDescription>
AssetManager<AssetManagerDescription>::AssetManager(const AssetDescriptor* asset_descriptors,
                                                    uint32 asset_descriptor_count)
    : m_description_(asset_descriptors, asset_descriptor_count) {
}

template<typename AssetManagerDescription>
AssetManager<AssetManagerDescription>::~AssetManager() {
}

template<typename AssetManagerDescription> template<typename AssetType>
Asset<AssetManager<AssetManagerDescription>, AssetType> AssetManager<AssetManagerDescription>::Get(uint32 id_hash) {
  // Static asserts
  enum { AssetTypeIndex = AssetManagerDescription::AssetLoaderIndex<AssetType>::value };
  STATIC_ASSERT(AssetTypeIndex != -1, "Unknown asset type");

  // Get the internal id from the manager
  uint32 asset_internal_id = m_description_.GetAssetLoader<AssetTypeIndex>().Get(id_hash);
  return Asset<AssetManager<AssetManagerDescription>, AssetType>(this, asset_internal_id);
}

template<typename AssetManagerDescription> template<typename AssetType>
AssetType* AssetManager<AssetManagerDescription>::GetData(uint32 internal_id) {
  // Static asserts
  enum { AssetTypeIndex = AssetManagerDescription::AssetLoaderIndex<AssetType>::value };
  STATIC_ASSERT(AssetTypeIndex != -1, "Unknown asset type");

  // Get the data from the manager
  return m_description_.GetAssetLoader<AssetTypeIndex>().GetData(internal_id);
}

}  // namespace AssetManager
}  // namespace Elg

#endif  // ELG_ASSETMANAGER_ASSETMANAGER_INL_H_

/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#ifndef ELG_ASSETMANAGER_ASSET_INL_H_
#define ELG_ASSETMANAGER_ASSET_INL_H_

#include "Elg/AssetManager/Asset.h"

#include "Elg/Utilities/Types.h"

namespace Elg {
namespace AssetManager {

// Constructors
template<typename AssetManagerType, typename AssetType>
Asset<AssetManagerType, AssetType>::Asset()
    : m_owner_(NULL), m_id_(0) {
}

template<typename AssetManagerType, typename AssetType>
Asset<AssetManagerType, AssetType>::Asset(AssetManagerType *owner, uint32 id)
    : m_owner_(owner), m_id_(id) {
}

template<typename AssetManagerType, typename AssetType>
Asset<AssetManagerType, AssetType>::Asset(const Asset<AssetManagerType, AssetType> &other)
    : m_owner_(other.m_owner_), m_id_(other.m_id_) {
}

// Destructor
template<typename AssetManagerType, typename AssetType>
Asset<AssetManagerType, AssetType>::~Asset() {
}

// Methods, including static methods
template<typename AssetManagerType, typename AssetType>
Asset<AssetManagerType, AssetType>& Asset<AssetManagerType, AssetType>::operator=(
    const Asset<AssetManagerType, AssetType> &rhs) {
  m_owner_ = rhs.m_owner_;
  m_id_ = rhs.m_id_;

  return *this;
}

template<typename AssetManagerType, typename AssetType>
AssetType* Asset<AssetManagerType, AssetType>::Get() {
  ASSERT(m_owner_ != NULL, "Attempt to get a uninitialized asset");
  return m_owner_->template GetData<AssetType>(m_id_);
}

template<typename AssetManagerType, typename AssetType>
AssetManagerType* Asset<AssetManagerType, AssetType>::owner() const {
  return m_owner_;
}

}  // namespace AssetManager
}  // namespace Elg

#endif  // ELG_ASSETMANAGER_ASSET_INL_H_

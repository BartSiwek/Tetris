/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#ifndef ELG_ASSETMANAGER_ASSET_H_
#define ELG_ASSETMANAGER_ASSET_H_

#include "Elg/Utilities/Types.h"

namespace Elg {
namespace AssetManager {

template<typename AssetManagerType, typename AssetType>
class Asset {
 public:
  // Typedefs
  typedef AssetManagerType asset_manager_type;
  typedef AssetType asset_type;

  // Constructors
  Asset();
  Asset(const Asset<AssetManagerType, AssetType> &other);

  // Destructor
  virtual ~Asset();

  // Methods, including static methods
  Asset<AssetManagerType, AssetType>& operator=(const Asset<AssetManagerType, AssetType> &rhs);
  AssetManagerType* owner() const;
  AssetType* Get();

 private:
  // Friendships
  template<typename AssetManagerDescripton>
  friend class AssetManager;

  // Constructors
  Asset(AssetManagerType *owner, uint32 id);

  // Data Members
  AssetManagerType *m_owner_;
  uint32 m_id_;
};

}  // namespace AssetManager
}  // namespace Elg

#include "Elg/AssetManager/Asset-inl.h"

#endif  // ELG_ASSETMANAGER_ASSET_H_

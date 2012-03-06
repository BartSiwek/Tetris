/*
 * Copyright (c) 2012 Bartlomiej Siwek All rights reserved.
 */

#ifndef ELG_ASSETMANAGER_DESCRIPTIONMACROS_H_
#define ELG_ASSETMANAGER_DESCRIPTIONMACROS_H_

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/enum.hpp>

#include "Elg/AssetManager/AssetDescriptor.h"

#define ELG_ASSET_MANAGER_DESCRIPTION_HEADER()                                                                        \
                                                                                                                      \
  template<typename AssetLoaderType> struct AssetLoaderIndex {                                                        \
    enum { value = -1 };                                                                                              \
  };                                                                                                                  \
                                                                                                                      \
  template<uint16 Index> struct AssetLoaderType;                                                                      \
                                                                                                                      \
  template<uint16 Index> typename AssetLoaderType<Index>::type& GetAssetLoader();                                     \
  template<uint16 Index> const typename AssetLoaderType<Index>::type& GetAssetLoader() const;


#define ELG_ASSET_MANAGER_DESCRIPTION_LOADER(LOADER_INDEX, ...)                                                       \
  typedef __VA_ARGS__ BOOST_PP_CAT(LoaderType, LOADER_INDEX);                                                         \
                                                                                                                      \
  BOOST_PP_CAT(LoaderType, LOADER_INDEX) BOOST_PP_CAT(Loader, LOADER_INDEX);                                          \
                                                                                                                      \
  template<>                                                                                                          \
  struct AssetLoaderIndex<__VA_ARGS__::asset_type> {                                                                  \
    enum { value = (LOADER_INDEX) };                                                                                  \
  };                                                                                                                  \
                                                                                                                      \
  template<>                                                                                                          \
  struct AssetLoaderType<(LOADER_INDEX)> {                                                                            \
    typedef BOOST_PP_CAT(LoaderType, LOADER_INDEX) type;                                                              \
  };                                                                                                                  \
                                                                                                                      \
  template<>                                                                                                          \
  AssetLoaderType<(LOADER_INDEX)>::type& GetAssetLoader<(LOADER_INDEX)>() {                                           \
    return BOOST_PP_CAT(Loader, LOADER_INDEX);                                                                        \
  }                                                                                                                   \
                                                                                                                      \
  template<>                                                                                                          \
  const AssetLoaderType<(LOADER_INDEX)>::type& GetAssetLoader<(LOADER_INDEX)>() const {                               \
    return BOOST_PP_CAT(Loader, LOADER_INDEX);                                                                        \
  }

#define ELG_ASSET_MANAGER_DESCRIPTION_FOOTER()

#define ELG_ASSET_MANAGER_DESCRIPTION_INITIALIZERS_CALLS(Z, N, DATA)                                                  \
  BOOST_PP_CAT(Loader, N)(asset_descriptors, asset_descriptor_count)

#define ELG_ASSET_MANAGER_DESCRIPTION_INITIALIZERS(LOADER_COUNT)                                                      \
  BOOST_PP_ENUM(LOADER_COUNT, ELG_ASSET_MANAGER_DESCRIPTION_INITIALIZERS_CALLS, DONT_CARE)

#endif  // ELG_ASSETMANAGER_DESCRIPTIONMACROS_H_

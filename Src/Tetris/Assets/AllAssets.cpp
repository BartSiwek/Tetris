/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#include "Tetris/Assets/AllAssets.h"

#include "Elg/Utilities/StringHash/Hash.h"

static const uint32 ASSET_DESCRIPTOR_COUNT = 10;

Elg::AssetManager::AssetDescriptor* GetAssetDescriptors() {
  static Elg::AssetManager::AssetDescriptor descriptors[ASSET_DESCRIPTOR_COUNT];
  static bool initialized = false;

  if (!initialized) {
    descriptors[0].set_id_hash(ELG_STRING_HASH("BACKGROUND"));
    descriptors[0].set_type_id_hash(ELG_STRING_HASH("Image"));
    descriptors[0].set_filename("Sprites\\Background.bmp");

    descriptors[1].set_id_hash(ELG_STRING_HASH("BOARD"));
    descriptors[1].set_type_id_hash(ELG_STRING_HASH("Image"));
    descriptors[1].set_filename("Sprites\\Board.bmp");

    descriptors[2].set_id_hash(ELG_STRING_HASH("PREVIEW"));
    descriptors[2].set_type_id_hash(ELG_STRING_HASH("Image"));
    descriptors[2].set_filename("Sprites\\NextPiece.bmp");

    descriptors[3].set_id_hash(ELG_STRING_HASH("BLUE_TILE"));
    descriptors[3].set_type_id_hash(ELG_STRING_HASH("Image"));
    descriptors[3].set_filename("Sprites\\Tiles\\Blue.bmp");

    descriptors[4].set_id_hash(ELG_STRING_HASH("CYAN_TILE"));
    descriptors[4].set_type_id_hash(ELG_STRING_HASH("Image"));
    descriptors[4].set_filename("Sprites\\Tiles\\Cyan.bmp");

    descriptors[5].set_id_hash(ELG_STRING_HASH("GREEN_TILE"));
    descriptors[5].set_type_id_hash(ELG_STRING_HASH("Image"));
    descriptors[5].set_filename("Sprites\\Tiles\\Green.bmp");

    descriptors[6].set_id_hash(ELG_STRING_HASH("ORANGE_TILE"));
    descriptors[6].set_type_id_hash(ELG_STRING_HASH("Image"));
    descriptors[6].set_filename("Sprites\\Tiles\\Orange.bmp");

    descriptors[7].set_id_hash(ELG_STRING_HASH("RED_TILE"));
    descriptors[7].set_type_id_hash(ELG_STRING_HASH("Image"));
    descriptors[7].set_filename("Sprites\\Tiles\\Red.bmp");

    descriptors[8].set_id_hash(ELG_STRING_HASH("VIOLET_TILE"));
    descriptors[8].set_type_id_hash(ELG_STRING_HASH("Image"));
    descriptors[8].set_filename("Sprites\\Tiles\\Violet.bmp");

    descriptors[9].set_id_hash(ELG_STRING_HASH("YELLOW_TILE"));
    descriptors[9].set_type_id_hash(ELG_STRING_HASH("Image"));
    descriptors[9].set_filename("Sprites\\Tiles\\Yellow.bmp");

    initialized = true;
  }
  return descriptors;
}

uint32 GetAssetDescriptorCount() {
  return ASSET_DESCRIPTOR_COUNT;
}

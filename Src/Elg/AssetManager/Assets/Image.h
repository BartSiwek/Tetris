/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#ifndef ELG_ASSETMANAGER_ASSETS_IMAGE_H_
#define ELG_ASSETMANAGER_ASSETS_IMAGE_H_

#include <cstring>

#include "Elg/Utilities/Types.h"
#include "Elg/Utilities/Asserts.h"
#include "Elg/Allocators/GlobalAllocator.h"

namespace Elg {
namespace AssetManager {
namespace Assets {

class Image {
 public:
  // Constants and enums
  enum ImageFormat {
    RGB,
    RGBA
  };

  // Constructors
  Image(uint32 width, uint32 height, ImageFormat format, unsigned char *data)
      : m_width_(width), m_height_(height) {
    if (format == RGB) {
      // Add extra channel
      uint32 pixel_count = width * height;
      m_data_ = ElgNew unsigned char[pixel_count * 4];
      ASSERT(m_data_ != NULL, "Memory allocation for new Image failed");

      uint32 pixel_index = 0;
      uint32 three_channel_offset = 0;
      uint32 four_channel_offset = 0;
      while (pixel_index < pixel_count) {
        std::memcpy(m_data_ + four_channel_offset, data + three_channel_offset, 3);
        m_data_[four_channel_offset + 3] = 255;

        ++pixel_index;
        three_channel_offset += 3;
        four_channel_offset += 4;
      }
    } else {
      // Just copy
      uint32 size = width * height * 4;
      m_data_ = ElgNew unsigned char[size];
      ASSERT(m_data_ != NULL, "Memory allocation for new Image failed");
      std::memcpy(m_data_, data, size);
    }
  }

  // Destructors
  ~Image() {
    ElgDelete[] m_data_;
  }

  // Methods, including static methods
  uint32 width() const {
    return m_width_;
  }

  uint32 height() const {
    return m_height_;
  }

  uint32 channel_count() const {
    return 4;
  }

  unsigned char* data() const {
    return m_data_;
  }

  uint32 pitch() {
    return width() * channel_count();
  }

 private:
  // Data members
  uint32 m_width_;
  uint32 m_height_;
  unsigned char *m_data_;

  // Disallow copy and assign
  Image(const Image& orig);
  void operator=(const Image&);
};

}  // namespace Assets
}  // namespace AssetManager
}  // namespace Elg

#endif  // ELG_ASSETMANAGER_ASSETS_IMAGE_H_

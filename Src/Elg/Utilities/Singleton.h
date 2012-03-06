/*
 * Copyright (c) 2011 Bartlomiej Siwek All rights reserved.
 */

#ifndef ELG_UTILITIES_SINGLETON_H_
#define ELG_UTILITIES_SINGLETON_H_

namespace Elg {
namespace Utilities {

template<typename T>
class Singleton {
 public:
  static T& instance() {
    static T singleton_instance;
    return singleton_instance;
  }

 private:
  Singleton();
  Singleton(const Singleton& /* other */);
  void operator=(const Singleton& /* other */);
  ~Singleton();
};

}  // namespace Utilities
}  // namespace Elg

#endif  // ELG_UTILITIES_SINGLETON_H_

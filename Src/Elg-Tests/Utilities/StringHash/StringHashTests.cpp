/*
 * Copyright (c) 2012 Bartlomiej Siwek All rights reserved.
 */

#include <UnitTest++.h>

#include <cstdlib>
#include <ctime>

#include "Elg/Utilities/StringHash/X65599Hash.h"
#include "Elg/Utilities/StringHash/X65599HashPreprocessor.h"
#include "Elg/Utilities/StringHash/Hash.h"

TEST(X65599_Runtime) {
  // Declare storage
  const uint32 WORD_LENGTH = 10;
  char test_input[WORD_LENGTH];
  char other_input[WORD_LENGTH];

  // Generate random words
  std::srand(static_cast<unsigned int>(std::time(NULL)));

  for (uint32 i = 0;i < WORD_LENGTH-1;++i) {
    // The rand is thread safe on VS according to MSDN
    test_input[i] = ('a' + (std::rand() % 26));  // NOLINT(runtime/threadsafe_fn)
  }
  test_input[WORD_LENGTH-1] = '\0';

  for (uint32 i = 0;i < WORD_LENGTH-1;++i) {
    // The rand is thread safe on VS according to MSDN
    other_input[i] = ('a' + (std::rand() % 26));  // NOLINT(runtime/threadsafe_fn)
  }
  other_input[WORD_LENGTH-1] = '\0';

  // Calculate the hashes (will not be folded cause they are  runtime generated)
  uint32 test_hash = Elg::Utilities::StringHash::X65599Hash(test_input);
  uint32 other_hash = Elg::Utilities::StringHash::X65599Hash(other_input);

  // Make sure that hashes are different
  CHECK(test_hash != other_hash);
}

TEST(X65599_Preprocessor) {
  // Calculate the hashes (should be converted to uint32's)
  uint32 test_hash = ELG_X65599("Prerocessor_Test", 17);
  uint32 other_hash = ELG_X65599("Other_Prerocessor_Test", 23);

  // Make sure that hashes are different
  CHECK(test_hash != other_hash);
}

TEST(X65599_PreprocessorAndCompileTimeEqual) {
  // Calculate the hashes (one should be converted to uint32)
  uint32 test_hash = ELG_X65599("Preprocessor_Runtime_Test", 26);
  uint32 other_hash = Elg::Utilities::StringHash::X65599Hash("Preprocessor_Runtime_Test");

  // Make sure that they are equal
  CHECK(test_hash == other_hash);
}

TEST(X65599_StringHash) {
  // Declare storage and data
  const uint32 WORD_LENGTH = 10;
  char other_input[WORD_LENGTH];

  // Generate other input
  std::srand(static_cast<unsigned int>(std::time(NULL)));
  for (uint32 i = 0;i < WORD_LENGTH-1;++i) {
    // The rand is thread safe on VS according to MSDN
    other_input[i] = ('a' + (std::rand() % 26));  // NOLINT(runtime/threadsafe_fn)
  }
  other_input[WORD_LENGTH-1] = '\0';

  // Hashes should be different
  CHECK(Elg::Utilities::StringHash::Hash("StringHash_Test") != Elg::Utilities::StringHash::Hash(other_input));
}

// Copyright (c) 2012, the Dart project authors.  Please see the AUTHORS file
// for details. All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.

#ifndef VM_BIT_SET_H_
#define VM_BIT_SET_H_

#include "vm/globals.h"

namespace dart {

// Just like its namesake in the STL, a BitSet object contains a fixed
// length sequence of bits.
template<intptr_t N>
class BitSet {
 public:
  BitSet() {
    Reset();
  }

  void Set(intptr_t i, bool value) {
    ASSERT(i >= 0);
    ASSERT(i < N);
    uword mask = (static_cast<uword>(1) << (i % kBitsPerWord));
    if (value) {
      data_[i / kBitsPerWord] |= mask;
    } else {
      data_[i / kBitsPerWord] &= ~mask;
    }
  }

  bool Test(intptr_t i) {
    ASSERT(i >= 0);
    ASSERT(i < N);
    uword mask = (static_cast<uword>(1) << (i % kBitsPerWord));
    return (data_[i / kBitsPerWord] & mask) != 0;
  }

  void Reset() {
    memset(data_, 0, sizeof(data_));
  }

  intptr_t Size() const {
    return N;
  }

 private:
  uword data_[1 + ((N - 1) / kBitsPerWord)];
};

}  // namespace dart

#endif  // VM_BIT_SET_H_

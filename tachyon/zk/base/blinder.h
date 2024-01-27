#ifndef TACHYON_ZK_BASE_BLINDER_H_
#define TACHYON_ZK_BASE_BLINDER_H_

#include <stddef.h>

#include <vector>

#include "tachyon/zk/base/random_field_generator_base.h"
#include "tachyon/zk/base/row_index.h"

namespace tachyon::zk {

template <typename PCS>
class Blinder {
 public:
  using F = typename PCS::Field;
  using Evals = typename PCS::Evals;

  Blinder(RandomFieldGeneratorBase<F>* random_field_generator,
          uint32_t blinding_factors)
      : random_field_generator_(random_field_generator),
        blinding_factors_(blinding_factors) {}

  const RandomFieldGeneratorBase<F>* random_field_generator() const {
    return random_field_generator_;
  }
  void set_blinding_factors(RowIndex blinding_factors) {
    blinding_factors_ = blinding_factors;
  }
  RowIndex blinding_factors() const { return blinding_factors_; }

  // The number of |blinding_rows| is determined to be either
  // |blinding_factors_| or |blinding_factors_| + 1, depending on the
  // |include_last_row| option.
  // Blinds |evals| at behind by |blinding_rows|.
  // Returns false if |evals.NumElements()| is less than |blinding_rows|.
  bool Blind(Evals& evals, bool include_last_row = false) {
    size_t size = evals.NumElements();
    RowIndex blinding_rows = blinding_factors_;
    if (include_last_row) ++blinding_rows;
    if (size < size_t{blinding_rows}) return false;
    size_t start = size - blinding_rows;
    for (size_t i = start; i < size; ++i) {
      *evals[i] = random_field_generator_->Generate();
    }
    return true;
  }

  F Generate() { return random_field_generator_->Generate(); }

 private:
  // not owned
  RandomFieldGeneratorBase<F>* random_field_generator_ = nullptr;
  RowIndex blinding_factors_ = 0;
};

}  // namespace tachyon::zk

#endif  // TACHYON_ZK_BASE_BLINDER_H_

//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// hyperloglog_presto.h
//
// Identification: src/include/primer/hyperloglog_presto.h
//
// Copyright (c) 2015-2025, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include <bitset>
#include <cmath>
#include <memory>
#include <mutex>  // NOLINT
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "common/util/hash_util.h"

/** @brief Dense bucket size. */
static constexpr int DENSE_BUCKET_SIZE = 4;
/** @brief Overflow bucket size. */
static constexpr int OVERFLOW_BUCKET_SIZE = 3;

/** @brief Total bucket size. */
static constexpr int TOTAL_BUCKET_SIZE = (DENSE_BUCKET_SIZE + OVERFLOW_BUCKET_SIZE);

namespace bustub {

template <typename KeyType>
class HyperLogLogPresto {
  /**
   * INSTRUCTIONS: Testing framework will use the GetDenseBucket and GetOverflow function,
   * hence SHOULD NOT be deleted. It's essential to use the dense_bucket_
   * data structure.
   */

  /** @brief Constant for HLL. */
  static constexpr double CONSTANT = 0.79402;

 public:
  /** @brief Disabling default constructor. */
  HyperLogLogPresto() = delete;

  explicit HyperLogLogPresto(int16_t n_leading_bits);

  /** @brief Returns the dense_bucket_ data structure. */
  auto GetDenseBucket() const -> std::vector<std::bitset<DENSE_BUCKET_SIZE>> { return dense_bucket_; }

  /** @brief Returns overflow bucket of a specific given index. */
  auto GetOverflowBucketofIndex(uint16_t idx) { return overflow_bucket_[idx]; }

  /** @brief Returns the cardinality of the set. */
  auto GetCardinality() const -> uint64_t { return cardinality_; }

  auto AddElem(KeyType val) -> void;

  auto ComputeCardinality() -> void;

 private:
  /** @brief Calculate Hash.
   *
   * @param[in] val
   *
   * @returns hash value
   */
  inline auto CalculateHash(KeyType val) -> hash_t {
    Value val_obj;
    if constexpr (std::is_same<KeyType, std::string>::value) {
      val_obj = Value(VARCHAR, val);
      return bustub::HashUtil::HashValue(&val_obj);
    }
    if constexpr (std::is_same<KeyType, int64_t>::value) {
      return static_cast<hash_t>(val);
    }
    return 0;
  }

  /** @brief Structure holding dense buckets (or also known as registers). */
  std::vector<std::bitset<DENSE_BUCKET_SIZE>> dense_bucket_;

  /** @brief Structure holding overflow buckets. */
  std::unordered_map<uint16_t, std::bitset<OVERFLOW_BUCKET_SIZE>> overflow_bucket_;

  /** @brief Storing cardinality value */
  uint64_t cardinality_;

  // TODO(student) - can add more data structures as required
  int16_t n_leading_bits_;

};
template <typename KeyType>
HyperLogLogPresto<KeyType>::HyperLogLogPresto(int16_t n_leading_bits) : cardinality_(0), n_leading_bits_(n_leading_bits) {
  if (n_leading_bits_ >= 0) {
    dense_bucket_.resize(1ULL << n_leading_bits_);
  }
}  
// Adds element to HyperLOGLOGPRESTO //
template <typename KeyType>
auto HyperLogLogPresto<KeyType>::AddElem(KeyType val) -> void {
  if (n_leading_bits_ < 0) {
    return;
  }
  auto hash = CalculateHash(val);
  std::bitset<64> binary(hash);
  uint16_t register_index = 0;
  if (n_leading_bits_ > 0) {
    for (int i = 63; i >= 64 - n_leading_bits_; i--) {
      register_index = (register_index << 1) | static_cast<uint16_t>(binary[i]);
    }
  }

  
  //count number of rightmost zeros in a row//
  uint64_t position = 0;
  int remaining_bits = 64 - n_leading_bits_;
  for (int i = 0; i < remaining_bits; i++) {
    if (binary[i]) {
      position = i + 1;
      break;
    }
  }
  // Get current register from dense bracket  //
  uint64_t current_register = dense_bucket_[register_index].to_ullong();
  auto overflow_it = overflow_bucket_.find(register_index);
  if (overflow_it != overflow_bucket_.end()) {
    current_register |= static_cast<uint64_t>(overflow_it->second.to_ullong()) << DENSE_BUCKET_SIZE;
  }
  if (position <= current_register) {
    return;
  }
  uint64_t dense_register = position & 0xF; //lower 4 bits//
  uint64_t overflow_register = position >> DENSE_BUCKET_SIZE; //upper 3 bits//
  dense_bucket_[register_index] = std::bitset<DENSE_BUCKET_SIZE>(dense_register);
  if (overflow_register != 0) {
    overflow_bucket_[register_index] = std::bitset<OVERFLOW_BUCKET_SIZE>(overflow_register);
  }  else {
    overflow_bucket_.erase(register_index);
  }
}
template <typename KeyType>
auto HyperLogLogPresto<KeyType>::ComputeCardinality() -> void {
  if (dense_bucket_.empty()) {
    cardinality_ = 0;
    return;
  }
  double sum = 0.0;
  const size_t m = dense_bucket_.size();
  for (size_t i = 0; i < m; i++) {
    uint64_t value = dense_bucket_[i].to_ullong(); //lower 4 bits//
    auto overflow_it = overflow_bucket_.find(static_cast<uint16_t>(i));
    if (overflow_it != overflow_bucket_.end()) {
      value |= static_cast<uint64_t>(overflow_it->second.to_ullong()) << DENSE_BUCKET_SIZE; //upper 3 bits//
    }
    sum += std::pow(2.0, -static_cast<double>(value));
  }
  const double estimate = CONSTANT * static_cast<double>(m) * static_cast<double>(m) / sum;
  cardinality_ = static_cast<uint64_t>(std::floor(estimate));
}

} // namespace bustub


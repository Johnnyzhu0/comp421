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

#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdint>
#include <memory>
#include <mutex>  // NOLINT
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "common/util/hash_util.h"
#include "type/type_id.h"
#include "type/value.h"

/** @brief Dense bucket size. */
static constexpr int DENSE_BUCKET_SIZE = 4;
/** @brief Overflow bucket size. */
static constexpr int OVERFLOW_BUCKET_SIZE = 3;
/** @brief Total bucket size. */
static constexpr int TOTAL_BUCKET_SIZE = (DENSE_BUCKET_SIZE + OVERFLOW_BUCKET_SIZE);

namespace bustub {

template <typename KeyType>
class HyperLogLogPresto {
  /** @brief Constant for HLL. */
  static constexpr double CONSTANT = 0.79402;

 public:
  /** @brief Disable default constructor. */
  HyperLogLogPresto() = delete;

  explicit HyperLogLogPresto(int16_t n_leading_bits);

  /** @brief Returns the dense_bucket_ data structure. */
  auto GetDenseBucket() const -> std::vector<std::bitset<DENSE_BUCKET_SIZE>> { return dense_bucket_; }

  /** @brief Returns overflow bucket of a specific given index. */
  auto GetOverflowBucketofIndex(uint16_t idx) const -> std::bitset<OVERFLOW_BUCKET_SIZE> {
    auto it = overflow_bucket_.find(idx);
    if (it != overflow_bucket_.end()) {
      return it->second;
    }
    return {0};
  }

  /** @brief Returns the cardinality of the set. */
  auto GetCardinality() const -> uint64_t { return cardinality_; }

  /** @brief Getter for dense bucket value. */
  auto GetDenseBucketValue(size_t index) const -> uint8_t;

  /** @brief Getter for overflow bucket value. */
  auto GetOverflowBucketValue(size_t index) const -> uint8_t;

  auto AddElem(KeyType val) -> void;

  auto ComputeCardinality() -> void;

 private:
  /** @brief Calculate Hash. */
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

  /** @brief Structure holding dense buckets (registers). */
  std::vector<std::bitset<DENSE_BUCKET_SIZE>> dense_bucket_;

  /** @brief Structure holding overflow buckets. */
  std::unordered_map<uint16_t, std::bitset<OVERFLOW_BUCKET_SIZE>> overflow_bucket_;

  /** @brief Storing cardinality value */
  uint64_t cardinality_;

  int16_t n_leading_bits_;
  std::mutex mutex_;
};

}  // namespace bustub
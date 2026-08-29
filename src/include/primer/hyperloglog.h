//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// hyperloglog.h
//
// Identification: src/include/primer/hyperloglog.h
//
// Copyright (c) 2015-2025, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include <bitset>
#include <memory>
#include <mutex>  // NOLINT
#include <string>
#include <utility>
#include <vector>
#include <cmath>
#include <algorithm>
#include "common/util/hash_util.h"

/** @brief Capacity of the bitset stream. */
static constexpr int BITSET_CAPACITY = 64;

namespace bustub {

template <typename KeyType>
class HyperLogLog {
  /** @brief Constant for HLL. */
  static constexpr double CONSTANT = 0.79402;

 public:
  /** @brief Disable default constructor. */
  HyperLogLog() = delete;

  explicit HyperLogLog(int16_t n_bits);


  /**
   * @brief Getter value for cardinality.
   *
   * @returns cardinality value
   */
  auto GetCardinality() { return cardinality_; }

  auto AddElem(KeyType val) -> void;

  auto ComputeCardinality() -> void;

 private:
  /**
   * @brief Calculates Hash of a given value.
   *
   * @param[in] val - value
   * @returns hash integer of given input value
   */
  inline auto CalculateHash(KeyType val) -> hash_t {
    Value val_obj;
    if constexpr (std::is_same<KeyType, std::string>::value) {
      val_obj = Value(VARCHAR, val);
    } else {
      val_obj = Value(BIGINT, val);
    }
    return bustub::HashUtil::HashValue(&val_obj);
  }

  auto ComputeBinary(const hash_t &hash) const -> std::bitset<BITSET_CAPACITY>;

  auto PositionOfLeftmostOne(const std::bitset<BITSET_CAPACITY> &bset) const -> uint64_t;

  /** @brief Cardinality value. */
  size_t cardinality_;

  /** @todo (student) can add their data structures that support HyperLogLog */
  int16_t n_bits_;
  std::vector<uint64_t> registers_;
};

template <typename KeyType>
HyperLogLog<KeyType>::HyperLogLog(int16_t n_bits) : cardinality_(0), n_bits_(n_bits){
  if (n_bits > 0) {
    registers_.resize(1ULL << n_bits, 0);
  } else if (n_bits == 0) {
    registers_.resize(1,0);
  }
}

template <typename KeyType>
auto HyperLogLog<KeyType>::ComputeBinary(const hash_t &hash) const -> std::bitset<BITSET_CAPACITY> {
  return {hash};
}
// Find position of leftmost 1 //
template <typename KeyType>
auto HyperLogLog<KeyType>::PositionOfLeftmostOne(const std::bitset<BITSET_CAPACITY> &bset) const -> uint64_t {
  for (int i = BITSET_CAPACITY - 1; i >= 0; i--) {
    if (bset[i]) {
      return BITSET_CAPACITY - i;
    }
  }
  return BITSET_CAPACITY;
}
// Add an element to the HyperLogLog//
template <typename KeyType>
auto HyperLogLog<KeyType>::AddElem(KeyType val) -> void {
  if (n_bits_ < 0) {
    return;
  }
  auto hash = CalculateHash(val);
  auto binary = ComputeBinary(hash);
  // Assigns Hash to a register//
  size_t register_index = 0;
  for (int i = BITSET_CAPACITY - 1;
   i >= BITSET_CAPACITY - n_bits_; i--) {
    register_index = (register_index << 1) | binary[i];
  }
  // Finds the positon of leftmost 1  //
  std::bitset<BITSET_CAPACITY> binary_copy;
  for (int i = BITSET_CAPACITY - n_bits_ - 1; i >= 0; i--) {
    binary_copy[i] = binary[i];
  }
  auto position = PositionOfLeftmostOne(binary_copy);
  // Updates the register with the maximum position of leftmost 1 //
  registers_[register_index] = std::max(registers_[register_index], position);
}
// Compute Cardinality of HyperLogLog //
template <typename KeyType>
auto HyperLogLog<KeyType>::ComputeCardinality() -> void {
  if (registers_.empty()) {
    cardinality_ = 0;
    return;
  }
  const size_t m = registers_.size();
  double sum = 0.0;
  for (auto value : registers_){
    sum += std::pow(2.0, -static_cast<double>(value));  
  }
  const double estimate = CONSTANT * static_cast<double>(m) * static_cast<double>(m) / sum;
  cardinality_ = static_cast<size_t>(std::floor(estimate));
}
}  // namespace bustub

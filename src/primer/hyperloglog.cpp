//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// hyperloglog.cpp
//
// Identification: src/primer/hyperloglog.cpp
//
// Copyright (c) 2015-2025, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "primer/hyperloglog.h"

namespace bustub {

template <typename KeyType>
HyperLogLog<KeyType>::HyperLogLog(int16_t n_bits) : cardinality_(0), n_bits_(n_bits) {
  if (n_bits_ >= 0) {
    registers_.resize(1ULL << n_bits_, 0);
  }
}

template <typename KeyType>
auto HyperLogLog<KeyType>::ComputeBinary(const hash_t &hash) const -> std::bitset<BITSET_CAPACITY> {
  return {hash};
}

template <typename KeyType>
auto HyperLogLog<KeyType>::PositionOfLeftmostOne(const std::bitset<BITSET_CAPACITY> &bset) const -> uint64_t {
  int remaining_bits = BITSET_CAPACITY - n_bits_;
  for (int i = remaining_bits - 1; i >= 0; i--) {
    if (bset[i]) {
      return static_cast<uint64_t>(remaining_bits - i);
    }
  }
  return static_cast<uint64_t>(remaining_bits) + 1;
}

template <typename KeyType>
auto HyperLogLog<KeyType>::AddElem(KeyType val) -> void {
  if (n_bits_ < 0) {
    return;
  }
  auto hash = CalculateHash(val);
  auto binary = ComputeBinary(hash);

  size_t register_index = 0;
  if (n_bits_ > 0) {
    for (int i = BITSET_CAPACITY - 1; i >= BITSET_CAPACITY - n_bits_; i--) {
      register_index = (register_index << 1) | static_cast<size_t>(binary[i]);
    }
  }

  auto position = PositionOfLeftmostOne(binary);
  {
    std::lock_guard<std::mutex> lock(mutex_);
    registers_[register_index] = std::max(registers_[register_index], position);
  }
}

template <typename KeyType>
auto HyperLogLog<KeyType>::ComputeCardinality() -> void {
  if (registers_.empty()) {
    cardinality_ = 0;
    return;
  }
  const size_t m = registers_.size();
  double sum = 0.0;
  for (auto value : registers_) {
    sum += std::pow(2.0, -static_cast<double>(value));
  }
  const double estimate = CONSTANT * static_cast<double>(m) * static_cast<double>(m) / sum;
  cardinality_ = static_cast<size_t>(std::floor(estimate));
}

template class HyperLogLog<int64_t>;
template class HyperLogLog<std::string>;

}  // namespace bustub
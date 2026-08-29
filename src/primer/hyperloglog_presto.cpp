//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// hyperloglog_presto.cpp
//
// Identification: src/primer/hyperloglog_presto.cpp
//
// Copyright (c) 2015-2025, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "primer/hyperloglog_presto.h"

namespace bustub {

template <typename KeyType>
HyperLogLogPresto<KeyType>::HyperLogLogPresto(int16_t n_leading_bits)
    : cardinality_(0), n_leading_bits_(n_leading_bits) {
  if (n_leading_bits_ >= 0) {
    dense_bucket_.resize(1ULL << n_leading_bits_);
  }
}

template <typename KeyType>
auto HyperLogLogPresto<KeyType>::GetDenseBucketValue(size_t index) const -> uint8_t {
  if (index >= dense_bucket_.size()) {
    return 0;
  }
  return static_cast<uint8_t>(dense_bucket_[index].to_ulong());
}

template <typename KeyType>
auto HyperLogLogPresto<KeyType>::GetOverflowBucketValue(size_t index) const -> uint8_t {
  auto it = overflow_bucket_.find(static_cast<uint16_t>(index));
  if (it != overflow_bucket_.end()) {
    return static_cast<uint8_t>(it->second.to_ulong());
  }
  return 0;
}

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

  uint64_t position = 0;
  int remaining_bits = 64 - n_leading_bits_;
  for (int i = 0; i < remaining_bits; i++) {
    if (binary[i]) {
      position = i + 1;
      break;
    }
  }

  std::lock_guard<std::mutex> lock(mutex_);
  uint64_t current_register = dense_bucket_[register_index].to_ullong();
  auto overflow_it = overflow_bucket_.find(register_index);
  if (overflow_it != overflow_bucket_.end()) {
    current_register |= static_cast<uint64_t>(overflow_it->second.to_ullong()) << DENSE_BUCKET_SIZE;
  }

  if (position <= current_register) {
    return;
  }

  uint64_t dense_register = position & 0xF;
  uint64_t overflow_register = position >> DENSE_BUCKET_SIZE;
  dense_bucket_[register_index] = std::bitset<DENSE_BUCKET_SIZE>(dense_register);
  if (overflow_register != 0) {
    overflow_bucket_[register_index] = std::bitset<OVERFLOW_BUCKET_SIZE>(overflow_register);
  } else {
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
    uint64_t value = dense_bucket_[i].to_ullong();
    auto overflow_it = overflow_bucket_.find(static_cast<uint16_t>(i));
    if (overflow_it != overflow_bucket_.end()) {
      value |= static_cast<uint64_t>(overflow_it->second.to_ullong()) << DENSE_BUCKET_SIZE;
    }
    sum += std::pow(2.0, -static_cast<double>(value));
  }
  const double estimate = CONSTANT * static_cast<double>(m) * static_cast<double>(m) / sum;
  cardinality_ = static_cast<uint64_t>(std::floor(estimate));
}

template class HyperLogLogPresto<int64_t>;
template class HyperLogLogPresto<std::string>;

}  // namespace bustub
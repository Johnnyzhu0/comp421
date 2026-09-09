//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// simple_replacer.h
//
// Identification: src/include/buffer/simple_replacer.h
//
// Copyright (c) 2015-2025, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include <optional>

#include "common/config.h"

namespace bustub {

enum class AccessType { Unknown = 0, Lookup, Scan, Index };

/**
 * @brief Abstract base class for the buffer pool's replacement policy.
 *
 * `BufferPoolManager` holds a `SimpleReplacer` so the eviction policy can be
 * swapped without touching the buffer pool. `LRUKReplacer` is the policy graded
 * in Project 1; a faster policy may be plugged in for the leaderboard via the
 * named `MakeReplacer` factory (see "buffer/replacer_factory.h").
 *
 * `RecordAccess` carries a `page_id` because some policies (e.g. ARC) key their
 * ghost history on page identity; policies that do not need it (e.g. LRU-K)
 * simply ignore the argument.
 */
class SimpleReplacer {
 public:
  SimpleReplacer() = default;
  virtual ~SimpleReplacer() = default;

  /** @brief Evict the frame chosen by the policy; std::nullopt if nothing is evictable. */
  virtual auto Evict() -> std::optional<frame_id_t> = 0;

  /**
   * @brief Record that `frame_id` (currently holding `page_id`) was accessed.
   * @param page_id and @param access_type may not be used by all replacers (such as LRU-k)
   * but this information is made available for optional replacer optimizations.
   * If these fields are not used, the overloads below choose sensible defaults.
   *
   */
  virtual void RecordAccess(frame_id_t frame_id, page_id_t page_id, AccessType access_type) = 0;

  /** @brief Convenience: record an access with the default (Unknown) access type. */
  void RecordAccess(frame_id_t frame_id, page_id_t page_id) { RecordAccess(frame_id, page_id, AccessType::Unknown); }

  /** @brief Convenience for callers that only have a frame id (page identity unused). */
  void RecordAccess(frame_id_t frame_id) { RecordAccess(frame_id, INVALID_PAGE_ID, AccessType::Unknown); }

  /** @brief Toggle whether `frame_id` may be selected for eviction. */
  virtual void SetEvictable(frame_id_t frame_id, bool set_evictable) = 0;

  /** @brief Stop tracking `frame_id` entirely, regardless of its access history. */
  virtual void Remove(frame_id_t frame_id) = 0;

  /** @return the number of frames that are currently evictable. */
  virtual auto Size() -> size_t = 0;
};

}  // namespace bustub

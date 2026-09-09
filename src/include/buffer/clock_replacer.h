//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// clock_replacer.h
//
// Identification: src/include/buffer/clock_replacer.h
//
// Copyright (c) 2015-2025, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include <list>
#include <mutex>  // NOLINT
#include <vector>

#include "buffer/replacer.h"
#include "common/config.h"

namespace bustub {

/**
 * ClockReplacer implements the clock replacement policy, which approximates the Least Recently Used policy.
 *
 * @deprecated Legacy replacer on the old `Replacer` interface (`Victim`/`Pin`/
 * `Unpin`), NOT compatible with `BufferPoolManager`, which now expects a
 * `SimpleReplacer` (`Evict`/`RecordAccess`/`SetEvictable`/`Remove`). Prefer
 * `LRUKReplacer` or `ArcReplacer`; port this onto `SimpleReplacer` before wiring
 * it into the buffer pool.
 */
class ClockReplacer : public Replacer {
 public:
  explicit ClockReplacer(size_t num_pages);

  ~ClockReplacer() override;

  auto Victim(frame_id_t *frame_id) -> bool override;

  void Pin(frame_id_t frame_id) override;

  void Unpin(frame_id_t frame_id) override;

  auto Size() -> size_t override;

 private:
  // TODO(student): implement me!
};

}  // namespace bustub

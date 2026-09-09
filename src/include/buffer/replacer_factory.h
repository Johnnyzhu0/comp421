//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// replacer_factory.h
//
// Identification: src/include/buffer/replacer_factory.h
//
// Copyright (c) 2015-2025, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>

#include "buffer/lru_k_replacer.h"
#include "buffer/simple_replacer.h"

namespace bustub {

/**
 * @brief Build the buffer pool's replacer for a named policy.
 *
 * The buffer pool never names a concrete replacer type; it only asks this factory
 * for a policy by name, so swapping the leaderboard's replacer is a one-line
 * change here that touches nothing else.
 *
 *   "LRU-K"       LRU-K, the replacer graded in Project 1. Used by every test and
 *                 by the buffer pool's default construction.
 *   "leaderboard" The policy the leaderboard benchmarks request. It uses LRU-K as
 *                 well, until you write a faster replacer: derive it from
 *                 SimpleReplacer, define it above (or in arc_replacer.*), and
 *                 return it in the "leaderboard" branch below, for example:
 *                     return std::make_shared<ArcReplacer>(num_frames);
 *
 * Any unrecognized policy throws std::runtime_error.
 */
inline auto MakeReplacer(std::string_view policy, size_t num_frames, size_t k) -> std::shared_ptr<SimpleReplacer> {
  if (policy == "LRU-K") {
    // The LRU-K replacer graded in Project 1.
    return std::make_shared<LRUKReplacer>(num_frames, k);
  }

  if (policy == "leaderboard") {
    // No faster replacer is wired up yet, so the leaderboard uses the same LRU-K
    // as everything else. To enter the leaderboard with your own policy, return
    // it here instead (see note above).
    return std::make_shared<LRUKReplacer>(num_frames, k);
  }

  throw std::runtime_error("MakeReplacer: unrecognized replacer policy '" + std::string(policy) + "'");
}

}  // namespace bustub

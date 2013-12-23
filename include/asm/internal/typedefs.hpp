#pragma once
#include <cstdint>
#include <utility>
#include <vector>
namespace ass { namespace internal {
typedef std::uint16_t addr_t;
typedef std::uint16_t pc_t;
typedef std::pair<std::unique_ptr<bfs::ifstream>, bfs::path> file_path_pair_t;
typedef std::vector<file_path_pair_t> ifvector_t;
}}
#ifndef ROB_ENTRY_HPP
#define ROB_ENTRY_HPP

struct ROB_entry {
     unsigned short id;
     std::uint16_t dest;
     std::uint16_t data;
     bool is_memory;
     bool is_ready;
     bool flush;
};

#endif // ROB_ENTRY_HPP

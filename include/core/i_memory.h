#include <vector>

#ifndef I_MEMORY_H
#define I_MEMORY_H

enum class HIT_POLICY {
    WRITE_THROUGH, WRITE_BACK
};

enum class MISS_POLICY {
    WRITE_ALLOCATE, WRITE_AROUND
};

class memory_block {
public:
    std::vector<unsigned short> data;
    unsigned short start_address;
};

class i_memory {
public:
    virtual unsigned short get_data( unsigned short address, int &delay ) = 0;
    virtual void write_data( unsigned short address,
                             unsigned short data, int &delay ) = 0;
    virtual memory_block fetch_block( unsigned short address, int size,
                                      int &delay ) = 0;
    virtual bool write_block( memory_block block, int &delay ) = 0;
    virtual unsigned int get_delay() = 0;
    virtual double get_hit_ratio() {
        return 0.0;
    }
    virtual double get_miss_ratio() {
        return 0.0;
    }
};

#endif // I_MEMORY_H

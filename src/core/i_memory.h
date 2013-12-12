#ifndef I_MEMORY_H
#define I_MEMORY_H

enum class HIT_POLICY {WRITE_THROUGH, WRITE_BACK};

enum class MISS_POLICY {WRITE_ALLOCATE, WRITE_AROUND};

class memory_block{
public:
    std::vector<short> data;
    int start_address;
};

class i_memory
{
public:
    virtual short get_data(int address) = 0;
    virtual void write_data(short data) = 0;
    virtual bool fill_cache(memory_block data) = 0;
    virtual ~i_memory();
};

#endif // I_MEMORY_H

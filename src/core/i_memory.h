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
    virtual unsigned short get_data(unsigned short address);
    virtual void write_data(unsigned short address, unsigned short data);
    virtual bool write_block(memory_block data);
    virtual ~i_memory();
};

#endif // I_MEMORY_H

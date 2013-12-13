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
    virtual unsigned short get_data(unsigned short address, int &delay);
    virtual void write_data(unsigned short address, unsigned short data, int &delay);
    virtual memory_block fetch_block(unsigned short address, int &delay);
    virtual void write_block(memory_block block, int &delay);
    virtual ~i_memory();
};

#endif // I_MEMORY_H

#ifndef I_MEMORY_H
#define I_MEMORY_H
#include <vector>
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
    virtual unsigned short get_data(unsigned short address, int &delay)= 0;
    virtual void write_data(unsigned short address,
                            unsigned short data, int &delay)= 0;
    virtual memory_block fetch_block(unsigned short address, int &delay)= 0;
    virtual void write_block(memory_block block, int &delay)= 0;
    virtual ~i_memory()= 0;
};

#endif // I_MEMORY_H

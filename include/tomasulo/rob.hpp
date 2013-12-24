#ifndef ROB_HPP
#define ROB_HPP
#include "rob_entry.hpp"
#include <algorithm>
#include <stdexcept>
#include <cstdint>
#include <string>
#include <list>




struct ROBException : std::runtime_error {
        ROBException(const std::string &what): std::runtime_error(what){}
        ROBException(const ROBException &) = default;
        ROBException(ROBException &&) = default;
};  

/*******************************************************
 * @brief This class Represents the Re-order Buffer.   *
 * @details Maximum size is 2^16 -1 i.e unsigned short.*
 * This is to gaurantee correct ids for ROB entries.   *
 *******************************************************/
class ROB{
private:
    std::list<ROB_entry> rob_entries;
    const unsigned short MAX_ENTRIES;
public:
    /****************************************
     * @param MAX_ENTRIES size of ROB.*
     ****************************************/
    ROB(const unsigned short size);

    auto getEntry(unsigned short id) {
        return std::find_if(rob_entries.begin(), rob_entries.end(),
                            [&id](ROB_entry & e) {return e.id == id;});
    }

    bool is_full(){return rob_entries.size() >= MAX_ENTRIES;}

    void tick(tomasulo &);

    /***************************************************
     * @brief Creates a new ROB Entry with a unique ID.*
     * @throws ROBException if ROB is full.            *
     * @return ID of newly created entry.              *
     ***************************************************/
    unsigned short create_entry();
};

#endif // ROB_HPP

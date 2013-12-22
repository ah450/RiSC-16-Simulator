#ifndef ROB_HPP
#define ROB_HPP
#include <list>
#include <algorithm>

class ROB{
private:
    std::list<ROB_entry> rob_entries;
    const unsigned int number_of_entries;

public:
    ROB(const unsigned int number_of_entries);
    auto getEntry(unsigned int id) {
        return std::find_if(rob_entries.begin(), rob_entries.end(),
                            [&id](ROB_entry & e) {return e.id == id;});
    }
    bool is_full(){return rob_entries.size()== number_of_entries;}
    void tick(tomasulo &);





};

#endif // ROB_HPP

#include "asm/internal/internal.hpp"

using namespace ass::internal;



FileState::FileState(const std::string name) : name(name), importedList(), exportedList(){

}
#include "asm/internal/internal.hpp"
using namespace ass::internal;


AssemblingStatus::AssemblingStatus( std::shared_ptr<ILogger> logger ) : error(
        false ),
    originDefined( false ),  mainDefined( false ), logger( logger ), files(),
    insts() {

}
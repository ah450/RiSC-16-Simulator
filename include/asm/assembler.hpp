#pragma once
#include "logger.hpp"
#include <boost/filesystem.hpp>
#include <memory>
#include <vector>

namespace ass {

    /**
     * @brief Assembles a list of sources
     * 
     * @param sources list of sources
     * @param  out  path to binary file to write.
     * @param l logger to use for error reporting
     * @return true if succesfull
     */
    bool assemble(const std::vector<boost::filesystem::path> &sources, const boost::filesystem::path &out, std::shared_ptr<ILogger> l);
  
}

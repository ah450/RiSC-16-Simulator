#include "asm/internal/internal.hpp"
#include <boost/tokenizer.hpp>
#include <iterator>
#include <string>
using namespace ass::internal;

const addr_t ass::internal::DEFAULT_ORIGIN = 0;


static bool parseLabel(file_path_pair_t &file, AssemblingStatus &state, std::size_t &lineNum);
static bool parseInstruct(file_path_pair_t &file, AssemblingStatus &state, std::size_t &lineNum);


inline std::vector<std::string> tokenizeLabels(const std::string &list) {
    boost::char_separator<char> sep(",");
    boost::tokenizer<boost::char_separator<char>> tok(list, sep);
    return std::vector<std::string>(tok.begin(), tok.end());
}

inline std::string removeComments(const std::string &orig) {
    return std::string(orig.begin(),  std::find(orig.begin(), orig.end(), ';'));
}




void ass::internal::setOrigin(const ifvector_t &files, AssemblingStatus &state) {

    using namespace ass;
    boost::smatch result;
    for(auto &pair : files) {
        auto originalGet = pair.first->tellg();
        std::string line;
        // read line
        std::getline(*(pair.first), line);
        line = removeComments(line);
        boost::algorithm::trim(line);
        // attemp to match line
        if(boost::regex_match(line, result, regex::originDirect)) {
            // regex matched
            if(state.originDefined) {
                // origin can only be defined once per program
                (*state.logger) << "Error : origin directory already redefinition encountered while processing file: "
                    << pair.second.generic_string() << "\n";
                state.error = true;
            }else {
                // first time origin directive encountered
               try {
                std::string addr = result[2];
                boost::trim(addr);
                int base = 10;
                // detect base
                if(addr.size() > 2) {
                    if(addr[0] == '0' && addr[1] == 'x'){
                        // base 16
                        base = 16;
                    }else if(addr[0] == '0' && addr[1] == 'b') {
                        // binary
                        base = 2;
                        addr = addr.substr(2); // skip 0b prefix
                    }
                } 
                // convert value   
                int value = std::stoi(addr, nullptr, base);
                // range checks
                if(value < 0) {
                    //negative origin
                    (*state.logger) << "Error : Invalid origin directive - address can not be negative - in file: "
                                    << pair.second.generic_string() << "\n";
                    state.error = true;

                }else if((value & 0x0000) != 0) {

                    throw std::out_of_range(std::to_string(value));
                }else {
                    // all checks passed
                    state.origin = value;
                    state.originDefined = true;
                }

               }catch (std::invalid_argument &ia) {
                    (*state.logger) << "Error : Could not parse paramater of origin directive in file: "
                                    << pair.second.generic_string() << "\n";
                    state.error = true;
               }catch (std::out_of_range &e) {
                    (*state.logger) << "Error : Origin paramater to large for 16 bits in file: "
                                    << pair.second.generic_string() << "\n";
                    state.error = true;
               } 

            }
        }else {
            // no origin directive. reset get pointer to beggining of file
            pair.first->seekg(originalGet);
        }
    } // end for each

    //default value
    if(!state.originDefined) {
        state.originDefined = true;
        state.origin = DEFAULT_ORIGIN;
    }
 

}


bool ass::internal::tryExport(file_path_pair_t &file, AssemblingStatus &state, std::size_t &lineNum) {

    auto originalGet = file.first->tellg();

    std::string line;
    std::getline(*file.first, line);
    line = removeComments(line);
    boost::trim(line);
    boost::smatch result;
    if(boost::regex_match(line, result, ass::regex::exportDirect)) {
        // is an export directive
        // remove spaces in label list
        std::string labelList = result[2];
        std::string noWhiteSpaceList;
        boost::regex_replace(std::back_inserter(noWhiteSpaceList), labelList.begin(), labelList.end(), boost::regex(R"(\s*)"), "");
        std::vector<std::string> labelVector = tokenizeLabels(noWhiteSpaceList);
        for(auto & l : labelVector) {
            // handle each label
            if(state.symbols.count(l) == 0) {
                // insert new symbol
                Sym s;
                s.defined = false; // to be set to true by actual definition
                s.name = l; // name is global not hashed
                s.global = true;
                state.symbols[l] = s;
                state.currentFileState.exportedSyms.push_back(s);
            }else {
                // trying to export an already existing label
                // if it was just imported in another file defined will be false
                if(state.symbols[l].defined) {
                    state.error = true;
                    (*state.logger) << "Error: Tying to export an already exported label : " << l << 
                        " " << file.second.generic_string() << "@" << std::to_string(lineNum) << " "
                            << "First defined in" << state.symbols[l].file.generic_string() 
                            << "@" << std::to_string(state.symbols[l].lineNum)
                            << "\n";
                }else {
                    // was just imported by another file
                    Sym &s = state.symbols[l];
                    s.global = true;
                    state.currentFileState.exportedSyms.push_back(s);
                }
            }
        }
        lineNum++;
        return true;
    }else {
        // not an exportDirective
        file.first->seekg(originalGet);
        return false;
    }


}

bool ass::internal::tryGlobal(file_path_pair_t &file, AssemblingStatus &state, std::size_t &lineNum) {
    auto originalGet = file.first->tellg();
    std::string line;
    std::getline(*file.first, line);
    line = removeComments(line);
    boost::trim(line);
    boost::smatch result;
    if(boost::regex_match(line, result, ass::regex::globalDirect)) {
        std::string labelList = result[2];
        std::string noWhiteSpaceList;
        boost::regex_replace(std::back_inserter(noWhiteSpaceList), labelList.begin(), labelList.end(), boost::regex(R"(\s*)"), "");
        std::vector<std::string> labelVect = tokenizeLabels(noWhiteSpaceList);

        for(auto &l : labelVect) {
              if(state.symbols.count(l) != 0) {
                // symbol already exists
                state.currentFileState.importedSyms.push_back(state.symbols[l]);
              }else {
                //create new undefined global label
                Sym s;
                s.defined = false;
                s.name = l; // name is global not hashed
                s.global = true;
                state.symbols[l] = s;
                // add label to imported syms list.
                state.currentFileState.importedSyms.push_back(s);
              }
        }
        lineNum++;
        return true;
    }else {
        // not a gloal directive
        file.first->seekg(originalGet);
        return false;
    }

}




bool ass::internal::parseLine(file_path_pair_t &file, AssemblingStatus &state, std::size_t &lineNum) {
    return parseLabel(file, state, lineNum) || parseInstruct(file, state, lineNum);
}




static bool parseLabel(file_path_pair_t &file, AssemblingStatus &state, std::size_t &lineNum) {
    
}
static bool parseInstruct(file_path_pair_t &file, AssemblingStatus &state, std::size_t &lineNum) {
    
}

#include "asm/internal/internal.hpp"
#include <iterator>
#include <string>
using namespace ass::internal;

const addr_t ass::internal::DEFAULT_ORIGIN = 0;







void ass::internal::setOrigin( const ifvector_t &files,
                               AssemblingStatus &state ) {
    bool found = false;

    for ( auto &f : files ) {
        // save get pointer
        auto originalg = f.first->tellg();
        std::string line;
        std::getline( *f.first, line );
        line = removeComments( line );
        boost::trim( line );
        boost::smatch result;

        if ( boost::regex_match( line, result, ass::regex::originDirect ) ) {
            try {
                std::uint16_t origin = convertNumber( std::string( result[2] ) );
                state.defineOrigin( origin );
                found = true;
            } catch ( ... ) {
                state << "Error parsing line: " << line << "\nFile: " <<
                      f.second.generic_string()
                      << '\n';
                state.signalError();
            }
        } else {
            // was not  an origin directive
            f.first->seekg( originalg );
        }
    }

    if ( !found ) {
        state.defineOrigin( DEFAULT_ORIGIN );
    }
}


void ass::internal::tryExport( file_path_pair_t &file, AssemblingStatus &state,
                               std::size_t &lineNum ) {


    // save get pointer
    auto originalg = file.first->tellg();
    std::string line;
    std::getline( *file.first, line );
    line = removeComments( line );
    boost::trim( line );
    boost::smatch result;
    FileState &fileState = state.getState(
                               file.second ); // create file state if not exist

    if ( boost::regex_match( line, result, ass::regex::exportDirect ) ) {
        lineNum++;
        auto labels = tokenizeLabels( std::string( result[2] ) );

        for ( auto &l : labels ) {
            // create global forward declerations
            SymReference r( std::make_shared<Sym>( l ) );

            if ( state.syms().count( r ) != 0 ) {
                // might be defined
                fileState.exportedSyms.insert( *state.syms().find( r ) );
            } else {
                fileState.exportedSyms.insert( r );
            }
        }
    } else {
        file.first->seekg( originalg );
    }

}

void ass::internal::tryGlobal( file_path_pair_t &file, AssemblingStatus &state,
                               std::size_t &lineNum ) {
    // save get pointer
    auto originalg = file.first->tellg();
    std::string line;
    std::getline( *file.first, line );
    line = removeComments( line );
    boost::trim( line );
    boost::smatch result;
    FileState &fileState = state.getState(
                               file.second ); // create file state if not exist

    if ( boost::regex_match( line, result, ass::regex::globalDirect ) ) {
        lineNum++;
        auto labels = tokenizeLabels( std::string( result[2] ) );

        for ( auto &l : labels ) {
            // create global forward decleration
            SymReference r( std::make_shared<Sym>( l ) );

            if ( state.syms().count( r ) != 0 ) {
                fileState.importedSyms.insert( *state.syms().find( r ) );
            } else {
                fileState.importedSyms.insert( r );
            }
        }
    } else {
        file.first->seekg( originalg );
    }



}





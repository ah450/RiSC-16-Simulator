#include "asm/internal/internal.hpp"
#include "asm/assembler.hpp"

static void processFile( ass::internal::file_path_pair_t &fp,
                         ass::internal::AssemblingStatus &state ) {
    using namespace ass;
    using namespace ass::internal;
    std::size_t lineNum = 0;
    tryExport( fp, state, lineNum );
    tryGlobal( fp, state, lineNum );
    FileState &file = state.getState( fp.second );

    for ( std::string line; std::getline( *fp.first, line ) ; ) {
        boost::trim( line );
        parseLine( removeComments( line ), state, file, ++lineNum );
    }

    if ( file.localDeps.size() != 0 ) {
        state << "Warning File finished with remaining local deps error will be signaled\n"
              << "File: " << fp.second.generic_string() << '\n';
        state.signalError();
    }
}

bool ass::assemble( const std::vector<boost::filesystem::path> &sources,
                    const boost::filesystem::path &out, std::shared_ptr<ILogger> l ) {

    ass::internal::ifvector_t files;

    for ( auto &p : sources ) {
        if ( boost::filesystem::is_regular_file( p ) ) {
            files.emplace_back( std::make_pair(
                                    std::unique_ptr<boost::filesystem::ifstream>( new boost::filesystem::ifstream(
                                                p ) ), p ) );
        } else if ( boost::filesystem::is_directory( p ) ) {
            ( *l ) << "Warning : directory path passed to ass::assemble";
            //TODO: implement recursive directory iteration.
        } else {
            ( *l ) << "Warning : path passed to ass::assemble which is neither a file nor a directory";
        }
    }

    ass::internal::AssemblingStatus state( l );
    setOrigin( files, state );

    for ( auto &fp : files ) {
        processFile( fp, state );
    }

    if ( state.ok() && state.isMainDefined() && state.isOriginDefined()
            && state.deps().size() == 0 ) {
        ass::internal::bfs::ofstream outFile( out, std::ios::binary );
        std::size_t size = 2  + state.instList().size();
        std::unique_ptr<std::uint16_t[]> stream ( new std::uint16_t [size] );
        stream[0] = state.origin();
        stream[1] = state.main();

        auto  iter = state.instList().begin();

        for ( std::size_t i = 2; i < size; i++, iter++ ) {
            stream[i] =  iter->data;
        }

        outFile.write( reinterpret_cast<char *>( stream.get() ),
                       sizeof( std::uint16_t ) * size );
        return true;
    } else {
        state << "Assemble final check failed. Please check state\n";
        return false;
    }


}



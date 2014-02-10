#include "asm/internal/inst_helpers.hpp"
#include "asm/internal/utility.hpp"
#include <iterator>
#include <map>

using namespace ass;
using namespace ass::internal;
static bool parseInstruction( const std::string &line , AssemblingStatus &state,
                              FileState &fs, std::size_t &lineNum );
static bool parseLabel( const std::string &line , AssemblingStatus &state,
                        FileState &fs, std::size_t &lineNum );
static void resolveDeps( SymReference &r, std::multimap<SymReference, pc_t> &m,
                         AssemblingStatus &state );


bool ass::internal::parseLine( const std::string &line ,
                               AssemblingStatus &state,
                               FileState &fs, std::size_t &lineNum ) {
    if ( parseInstruction( line, state, fs, lineNum )
            || parseLabel( line, state, fs, lineNum ) ) {
        return true;
    } else {
        std::string nonConst = line;

        boost::trim( nonConst );

        if ( removeComments( nonConst ).size() != 0 ) {
            state << "OMG I COULD NOT PARSE THIS LINE : " << nonConst
                  << "\n" << fs.name.generic_string() << "@" << std::to_string( lineNum )
                  << "\n";
            state.signalError();
        }

        return true;
    }
}


static bool parseInstruction( const std::string &line , AssemblingStatus &state,
                              FileState &fs, std::size_t &lineNum ) {

    for ( auto &f : functors ) {
        if ( f( line, state, lineNum, fs ) ) {
            return true;
        }
    }

    return false;
}
static bool parseLabel( const std::string &line , AssemblingStatus &state,
                        FileState &fs, std::size_t &lineNum ) {
    boost::smatch result;

    if ( boost::regex_match( line, result, ass::regex::labelReg ) ) {
        std::string name = result[1];
        bool found = false;

        for ( auto sr : fs.importedSyms ) {
            if ( sr.ref->name() == name ) {
                found = true;
                sr.ref->define( state.instList().size() * 2, SymType::PTR );
                resolveDeps( sr, fs.localDeps, state );
                resolveDeps( sr, state.deps(), state );

                if ( name == "main" ) {
                    state.defineMain( ( state.instList().size() * 2 ) + state.origin() );
                }
            }
        }

        if ( !found ) {
            for ( auto sr : fs.exportedSyms ) {
                if ( sr.ref->name() == name ) {
                    found = true;
                    sr.ref->define( state.instList().size() * 2, SymType::PTR );
                    resolveDeps( sr, fs.localDeps, state );
                    resolveDeps( sr, state.deps(), state );

                    if ( name == "main" ) {
                        state.defineMain( ( state.instList().size() * 2 ) + state.origin() );
                    }
                }
            }
        }

        if ( !found ) {
            for ( auto sr : fs.localSyms ) {
                if ( sr.ref->name() == name ) {
                    found = true;
                    sr.ref->define( fs.name, state.instList().size() * 2, SymType::PTR );
                    resolveDeps( sr, fs.localDeps, state );
                }
            }
        }

        if ( !found ) {
            // local sym no forward declarations
            auto sr = SymReference(
                          std::make_shared<Sym>( name, fs.name,
                                                 state.instList().size() * 2, SymType::PTR ) );
            fs.localSyms.insert( sr );
            resolveDeps( sr, fs.localDeps, state );
        }

        return true;
    } else {
        return false;
    }
}


static void resolveDeps( SymReference &r, std::multimap<SymReference, pc_t> &m,
                         AssemblingStatus &state ) {
    if ( m.count( r ) != 0 ) {
        auto p = m.equal_range( r );

        if ( p.first != m.end() ) {
            while ( p.first != p.second ) {
                auto i = state.instList().begin();
                std::advance( i, p.first->second );
                fillInstruction( *i,  r.ref->value() );
                p.first++;
            }
        } else if ( p.second != m.end() ) {
            while ( p.first != p.second ) {
                auto i = state.instList().begin();
                std::advance( i, p.second->second );
                fillInstruction( *i, r.ref->value() );
                p.second++;
            }
        }

        // remove from dep list
        m.erase( r );
    }
}


std::array<instFunct_t, 17> ass::internal::functors = {
    {
        {HaltF()}, {AddF()}, {SubF()}, {MulF()},
        {DivF()}, {AndF()}, {ORF()}, {XORF()}, {NOTF()},
        {LSWF()}, {AddiF()}, {JMPF()}, {LIF()}, {JALRF()},
        {JALIF()}, {RETF()}, {BEQF()}
    }
};





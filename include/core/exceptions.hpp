#pragma once
#include <stdexcept>
#include <string>
/**
 * @brief Throws by Instruction.
 */
struct InstructionException : std::runtime_error {
    InstructionException( const std::string &what ): std::runtime_error( what ) {}
    InstructionException( const InstructionException & ) = default;
    InstructionException( InstructionException && ) = default;
};


/**
 * @brief thrown by tomasulo
 */
struct StructureException : std::runtime_error {
    StructureException( const std::string &what ): std::runtime_error( what ) {}
    StructureException( const StructureException & ) = default;
    StructureException( StructureException && ) = default;
};


struct ROBException : std::runtime_error {
    ROBException( const std::string &what ): std::runtime_error( what ) {}
    ROBException( const ROBException & ) = default;
    ROBException( ROBException && ) = default;
};
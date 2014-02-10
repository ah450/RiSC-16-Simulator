#include "core/reservation_station.hpp"
#include "core/tomasulo.hpp"
#include <memory>
static unsigned int id_counter = 1;

reservation_station::reservation_station( FUNCTION_UNIT fu,
        cycle_t delay ) : id( id_counter++ ), OP_DELAY( delay ) {

    working = false;
    busy = false;
    this->fu = fu;
    delay_cycles = delay;
    state = STATE::IDLE;
    vj =  vk = qj = qk = 0;
    qjValid = qkValid = false;
    kReady = jReady = false;
}

void reservation_station::flush() {
    working = false;
    busy = false;
}

std::array<uint16_t, 5> reservation_station::get_inst_operands(
    const Instruction &inst ) {
    uint16_t operand1 = 0;
    uint16_t operand2 = 0;
    uint16_t operand3 = 0;

    uint16_t sum = 0;
    uint16_t has_imm = 0 ;

    switch ( fu ) {
        case FUNCTION_UNIT::ADD:
            operand1 = ( uint16_t )inst.get_operand( 1 );

            if ( inst.type == OP::ADD || inst.type == OP::SUB ) {
                operand2 = ( uint16_t )inst.get_operand( 2 );
                operand3 = ( uint16_t )inst.get_operand( 3 );
                sum = 3;
            } else if ( inst.type == OP::LI ) {
                operand2 = ( uint16_t )inst.get_immediate();
                sum = 2;
                has_imm = 1;
            } else {
                operand2 = ( uint16_t )inst.get_operand( 2 );
                operand3 = ( uint16_t )inst.get_immediate();
                sum = 3;
                has_imm = 1;
            }

            break;

        case FUNCTION_UNIT::LOGIC:
        case FUNCTION_UNIT::MULL:
            operand1 = ( uint16_t )inst.get_operand( 1 );
            operand2 = ( uint16_t )inst.get_operand( 2 );
            operand3 = ( uint16_t )inst.get_operand( 3 );
            sum = 3;
            break;

        case FUNCTION_UNIT::LOAD:
            operand1 = ( uint16_t )inst.get_operand( 1 );
            operand2 = ( uint16_t )inst.get_operand( 2 );
            operand3 = ( uint16_t )inst.get_immediate();
            sum = 3;
            has_imm = 1;
            break;

        case FUNCTION_UNIT::BRANCH:
            if ( inst.type == OP::JALR ) {
                operand1 = ( uint16_t )inst.get_operand( 1 );
                operand2 = ( uint16_t )inst.get_operand( 2 );
                sum = 2;
            } else if ( inst.type == OP::JALI || inst.type == OP::JMP ) {
                operand1 = ( uint16_t )inst.get_operand( 1 );
                operand2 = ( uint16_t )inst.get_immediate();
                sum = 2;
                has_imm = 1;
            } else if ( inst.type == OP::RET ) {
                operand1 = ( uint16_t )inst.get_operand( 1 );
                sum = 1;
            } else {
                operand1 = ( uint16_t )inst.get_operand( 1 );
                operand2 = ( uint16_t )inst.get_operand( 2 );
                operand3 = ( uint16_t )inst.get_immediate();
                sum = 3;
                has_imm = 1;
            }

            break;
    }

    std::array<uint16_t, 5> result = { {operand1, operand2, operand3, sum, has_imm} };
    return result;
}

void reservation_station::put_reg_or_rob( tomasulo &t,
        std::uint8_t reg_number, std::uint8_t index ) {

    if ( reg_number == 0 ) {
        if ( index == 1 ) {
            vj = 0;
            jReady = true;
            qjValid = false;
        } else if ( index == 2 ) {
            vk = 0;
            kReady = true;
            qkValid = false;
        }



    }

    if ( !t.regBoard.used( reg_number )
            || ( t.regBoard.retrieve( reg_number ) == rob_id ) ) {
        // not being used
        data_t value = t.regFile[reg_number];

        if ( index == 1 ) {
            vj = value;
            jReady = true;
            qjValid = false;
        } else if ( index == 2 ) {
            vk = value;
            kReady = true;
            qkValid = false;
        }

    } else {
        robid_t producer = t.regBoard.retrieve( reg_number );
        t.RegisterWaitForROB( producer,
                              id ); // register this station as waiting for producer

        if ( index == 1 ) {
            qj = producer;
            qjValid = true;

        } else if ( index == 2 ) {
            qk = producer;
            qkValid = true;

        }
    }
}

void reservation_station::takeThis( robid_t rob, data_t data ) {
    if ( qkValid & ( qk == rob ) ) {
        vk = data;
        qkValid = false;
        kReady = true;
    }

    if ( qjValid & ( qj == rob ) ) {
        vj = data;
        qjValid = false;
        jReady = true;
    }
}

void reservation_station::issue( Instruction inst, robid_t rob_id,
                                 tomasulo &t ) {
    qj = vk = vj = qk = imm = 0;
    kReady = jReady = qjValid = qkValid = false;
    pcAtIssue = inst.pc;
    state = STATE::ISSUED;
    this->rob_id = rob_id; // my rob entry
    working = false; // I am now doing something
    busy = true;
    type_of_instruction = inst.type; // save instruction type.

    switch ( type_of_instruction ) {
        case OP::LW:
            issueLW( inst, t );
            break;

        case OP::SW:
            issueSW( inst, t );
            break;

        case OP::AND:
        case OP::OR:
        case OP::XOR:
        case OP::MUL:
        case OP::DIV:
        case OP::ADD:
        case OP::SUB:
            issueTernary( inst, t );
            break;

        case OP::NOT:
            issueNot( inst, t );
            break;

        case OP::BEQ:
            issueBEQ( inst, t );
            break;

        case OP::RET:
            issueRet( inst, t );
            break;

        case OP::JMP:
            issueJmp( inst, t );
            break;

        case OP::JALR:
            issueJalr( inst, t );
            break;

        case OP::JALI:
            issueJali( inst, t );
            break;

        case OP::ADDI:
            issueAddi( inst, t );
            break;

        case OP::LI:
            issueLI( inst, t );
            break;

        case OP::HALT:
            throw std::runtime_error( "OMG WHY YOU GIV HALT TO RESERVATIONSTATIONZ?!\nWHY YOU DO THIZ?" );
    }

}

void reservation_station::issueLW( Instruction &inst, tomasulo &t ) {
    std::array<std::uint16_t, 5> operands = get_inst_operands( inst );
    imm = operands[2];
    rd = operands[0];
    unsigned int rs = operands[1];
    put_reg_or_rob( t, rs, 1 );
    kReady = true; // k not used in this instruction
    t.regBoard.record( rd, rob_id ); // record ourselves as producer of rd
}

void reservation_station::issueSW( Instruction &inst, tomasulo &t ) {
    std::array<std::uint16_t, 5> operands = get_inst_operands( inst );
    imm = operands[2]; // offset
    unsigned int rs = operands[1]; // offset base
    rd = operands[0]; // register to store
    put_reg_or_rob( t, rd, 1 );
    put_reg_or_rob( t, rs, 2 );
    // SW DOES NOT RECORD ANYTHING ON THE REGISTER BOARD !!!!!!!!!!!!!!!!!!!!!!!!
}

void reservation_station::issueNot( Instruction &inst, tomasulo &t ) {
    std::array<std::uint16_t, 5> operands = get_inst_operands( inst );
    unsigned int rs = operands[1];
    rd = operands[0];
    put_reg_or_rob( t, rs, 1 );
    kReady = true; // not used for this instruction
    t.regBoard.record( rd, rob_id );
}

void reservation_station::issueTernary( Instruction &inst, tomasulo &t ) {
    std::array<std::uint16_t, 5> operands = get_inst_operands( inst );
    rd = operands[0];
    unsigned int rs = operands[1];
    unsigned int rt = operands[2];
    put_reg_or_rob( t, rs, 1 );
    put_reg_or_rob( t, rt, 2 );
    t.regBoard.record( rd, rob_id );
}

void reservation_station::issueBEQ( Instruction &inst, tomasulo &t ) {
    std::array<std::uint16_t, 5> operands = get_inst_operands( inst );
    unsigned int rs = operands[0];
    unsigned int rt = operands[1];
    imm = operands[2];
    put_reg_or_rob( t, rs, 1 );
    put_reg_or_rob( t, rt, 2 );
    // NO REGISTER PRODUCED HERE !! MOVE ALONG
}

void reservation_station::issueRet( Instruction &inst, tomasulo &t ) {
    std::array<std::uint16_t, 5> operands = get_inst_operands( inst );
    unsigned int rs = operands[0]; // register to branch too
    put_reg_or_rob( t, rs, 1 );
    kReady = true;
}

void reservation_station::issueJmp( Instruction &inst, tomasulo &t ) {
    std::array<std::uint16_t, 5> operands = get_inst_operands( inst );
    imm = operands[1];
    unsigned int rs = operands[0];
    kReady = true;
    put_reg_or_rob( t, rs, 1 );
    // NO RECORDING FOR THIS INSTRUCTION !?!>!
}

void reservation_station::issueJalr( Instruction &inst, tomasulo &t ) {
    std::array<std::uint16_t, 5> operands = get_inst_operands( inst );
    rd = operands[0];
    unsigned int rs = operands[1];
    put_reg_or_rob( t, rs, 1 );
    kReady = true;
    t.regBoard.record( rd, rob_id );
}

void reservation_station::issueJali( Instruction &inst, tomasulo &t ) {
    std::array<std::uint16_t, 5> operands = get_inst_operands( inst );
    rd = operands[0];
    imm = operands[1];
    // no source registers for this instruction !
    kReady = jReady = true;
    t.regBoard.record( rd, rob_id );
}

void reservation_station::issueAddi( Instruction &inst, tomasulo &t ) {
    std::array<std::uint16_t, 5> operands = get_inst_operands( inst );
    imm = operands[2];
    rd = operands[0];
    unsigned int rs = operands[1];
    put_reg_or_rob( t, rs, 1 ); // I WAIT FOR RS IF NEED BE
    kReady = true; // NO RT
    t.regBoard.record( rd, rob_id ); // I MAKEZ RD !! YAY !
}

void reservation_station::issueLI( Instruction &inst, tomasulo &t ) {
    std::array<std::uint16_t, 5> operands = get_inst_operands( inst );
    imm = operands[1];
    rd = operands[0];
    kReady = jReady = true;
    t.regBoard.record( rd, rob_id ); // LOOK AT ME MA ! I AM A PRODUZER !
}


void reservation_station::forward( tomasulo &t ) {

    if ( t.signal_bus() ) {
        t.forward( rob_id, result );
    }
}

STATE reservation_station::tick_MULL( tomasulo &t ) {
    ;

    if ( !working ) {
        switch ( type_of_instruction ) {
            case OP::MUL:
                result = vj * vk;
                delay_cycles = OP_DELAY;
                delay_cycles--; // this cycle
                break;

            case OP::DIV:
                result = vj / vk;
                delay_cycles = OP_DELAY;
                delay_cycles--;
                break;

            default:
                throw std::runtime_error( "MULL got something other than DIV and MUL" );

        }

        working = true;
        return state;
    } else {
        // already working
        if ( !delay_cycles ) {
            delay_cycles--;

            if ( delay_cycles ) {
                state = STATE::EXECUTED;
                return STATE::EXECUTED;
            }

            return state;
        } else {
            forward( t );
            auto &rob_entry = *t.rob.getEntry( rob_id );
            rob_entry.data = result;
            rob_entry.dest = rd;
            rob_entry.is_ready = true;
            rob_entry.is_memory = false;
            rob_entry.type = type_of_instruction;
            working = false;
            busy = false;
            state = STATE::IDLE;
            return STATE::WRITTEN;
        }
    }
}

STATE reservation_station::tick_LOGICAL( tomasulo &t ) {
    if ( !working ) {
        switch ( type_of_instruction ) {
            case OP::AND:
                result = vj & vk;
                delay_cycles = OP_DELAY;
                delay_cycles--; // this cycle
                break;

            case OP::OR:
                result = vj | vk;
                delay_cycles = OP_DELAY;
                delay_cycles--; // this cycle
                break;

            case OP::XOR:
                result = vj ^ vk;
                delay_cycles = OP_DELAY;
                delay_cycles--; // this cycle
                break;

            case OP::NOT:
                result = ~vj;
                delay_cycles = OP_DELAY;
                delay_cycles--; // this cycle
                break;

            default:
                throw std::runtime_error( "LOGICAL TICK HAS INCORRECT OP" );
        }

        working = true;
        return state;
    } else {
        if ( !delay_cycles ) {
            delay_cycles--;

            if ( delay_cycles ) {
                state = STATE::EXECUTED;
                return STATE::EXECUTED;
            }

            return state;
        } else {
            forward( t );
            auto &rob_entry = *t.rob.getEntry( rob_id );
            rob_entry.data = result;
            rob_entry.dest = rd;
            rob_entry.is_ready = true;
            rob_entry.is_memory = false;
            rob_entry.type = type_of_instruction;
            working = false;
            busy = false;
            state = STATE::IDLE;
            return STATE::WRITTEN;

        }
    }
}


STATE reservation_station::tick_ADD( tomasulo &t ) {
    if ( !working ) {
        switch ( type_of_instruction ) {
            case OP::ADD:
                result = vj + vk;
                delay_cycles = OP_DELAY;
                delay_cycles--; // this cycle
                break;

            case OP::ADDI:
                result = vj + imm;
                delay_cycles = OP_DELAY;
                delay_cycles--; // this cycle
                break;

            case OP::SUB:
                result = vj - vk;
                delay_cycles = OP_DELAY;
                delay_cycles--; // this cycle
                break;

            case OP::LI:
                result = imm;
                delay_cycles = OP_DELAY;
                delay_cycles--; // this cycle
                break;

            default:
                throw std::runtime_error( "OMG ADD GOT A NON ADD INST" );
        }

        working = true;
        return state;
    } else {
        if ( !delay_cycles ) {
            delay_cycles--;

            if ( delay_cycles ) {
                state = STATE::EXECUTED;
                return STATE::EXECUTED;
            }

            return state;
        } else {
            forward( t );
            auto &rob_entry = *t.rob.getEntry( rob_id );
            rob_entry.data = result;
            rob_entry.dest = rd;
            rob_entry.is_ready = true;
            rob_entry.is_memory = false;
            rob_entry.type = type_of_instruction;
            working = false;
            busy = false;
            state = STATE::IDLE;
            return STATE::WRITTEN;
        }
    }
}


STATE reservation_station::tick_LOAD( tomasulo &t ) {
    if ( !working ) {
        switch ( type_of_instruction ) {
            case OP::LW:
                if ( t.lockMem( id ) ) {
                    address_t address = vj + imm;
                    int memory_delay = 0;
                    result = t.memory()->get_data( address , memory_delay );
                    delay_cycles = OP_DELAY + memory_delay;
                    delay_cycles--; // this cycle
                    break;
                }

            case OP::SW:
                result = vj + imm;
                delay_cycles = OP_DELAY;
                delay_cycles--; // this cycle
                break;

            default:
                throw std::runtime_error( "OMG LOAD GOT A NON LOAD INSTRUCTION!!" );
        }

        working = true;
        return state;
    } else {
        if ( !delay_cycles ) {

            delay_cycles--; // this cycle

            if ( delay_cycles ) {
                state = STATE::EXECUTED;
                t.unlockMem( id );
                return STATE::EXECUTED;
            }

            return state;
        } else {

            if ( type_of_instruction == OP::LW ) {
                forward( t );
                auto &rob_entry = *t.rob.getEntry( rob_id );
                rob_entry.data = result;
                rob_entry.dest = rd;
                rob_entry.is_ready = true;
                rob_entry.is_memory = false;
                rob_entry.type = type_of_instruction;
                working = false;
                busy = false;


            } else {
                auto &rob_entry = *t.rob.getEntry( rob_id );
                rob_entry.data = this->vk;
                rob_entry.dest = result;
                rob_entry.is_ready = true;
                rob_entry.is_memory = true;
                rob_entry.type = type_of_instruction;
                working = false;
                busy = false;

            }

            state = STATE::IDLE;
            return STATE::WRITTEN;
        }
    }
}


STATE reservation_station::tick_BRANCH( tomasulo &t ) {
    if ( !working ) {
        switch ( type_of_instruction ) {
            case OP::JMP:
                result = pcAtIssue + 2 + vj + imm;
                delay_cycles = OP_DELAY;
                delay_cycles--; // this cycle
                break;

            case OP::JALR:
                result = vj + pcAtIssue + 2;
                delay_cycles = OP_DELAY;
                delay_cycles--; // this cycle
                break;

            case OP::JALI:
                result = imm + pcAtIssue + 2;
                delay_cycles = OP_DELAY;
                delay_cycles--; // this cycle
                break;

            case OP::RET:
                result = vj;
                delay_cycles = OP_DELAY;
                delay_cycles--; // this cycle
                break;

            case OP::BEQ:
                result = vj - vk;
                delay_cycles = OP_DELAY;
                delay_cycles--; // this cycle
                break;

            default:
                throw std::runtime_error( "OMG BRANCH GOT A NON BRANCH INSTRUCTION" );
        }

        working = true;
        return state;
    } else {
        // wait delay_cycles
        if ( !delay_cycles ) {
            delay_cycles--;

            if ( delay_cycles ) {
                state = STATE::EXECUTED;
                return STATE::EXECUTED;
            }

            return state;
        } else {
            // Done with waiting.
            auto &rob_entry = *t.rob.getEntry( rob_id );
            rob_entry.type = type_of_instruction;
            rob_entry.is_memory = false;
            rob_entry.is_jump = true;
            rob_entry.is_ready = true;

            if ( type_of_instruction == OP::BEQ ) {

                if ( result == 0 ) {
                    rob_entry.dest = imm + pcAtIssue + 2;

                    // should have been taken
                    if ( imm < 0 ) {
                        // was actually taken
                        rob_entry.should_jump = false;
                    } else {
                        // was not taken
                        rob_entry.should_jump = true;
                    }
                } else {
                    // should not have been taken
                    rob_entry.dest = pcAtIssue + 2;

                    if ( imm < 0 ) {
                        // was actually taken
                        rob_entry.should_jump = true;
                    } else {
                        // was not taken
                        rob_entry.should_jump = false;
                    }
                }
            } else if ( type_of_instruction == OP::RET || type_of_instruction == OP::JMP ) {
                // uncoditional jump
                rob_entry.dest = result;
                rob_entry.should_jump = true;
            } else if ( type_of_instruction == OP::JALR ) {
                // uncoditional jump
                rob_entry.dest = result;
                rob_entry.should_jump = true;
                rob_entry.extra = rd;
                rob_entry.data = pcAtIssue + 2;
                result = rob_entry.data;
                forward( t );

            } else if ( type_of_instruction == OP::JALI ) {
                rob_entry.dest = result;
                rob_entry.should_jump = true; // will not actually jump
                rob_entry.data = pcAtIssue + 2;
                rob_entry.extra = rd;
                result = rob_entry.data;
                forward( t );

            }


            state = STATE::IDLE;
            working = false;
            busy = false;
            return STATE::WRITTEN;

        }
    }
}

STATE reservation_station::tick( tomasulo &t ) {
    if ( jReady & kReady & ( !( state == STATE::IDLE ) ) ) {
        switch ( fu ) {
            case FUNCTION_UNIT::MULL:
                return tick_MULL( t );

            case FUNCTION_UNIT::LOGIC:
                return tick_LOGICAL( t );

            case FUNCTION_UNIT::ADD:
                return tick_ADD( t );

            case FUNCTION_UNIT::BRANCH:
                return tick_BRANCH( t );

            case FUNCTION_UNIT::LOAD:
                return tick_LOAD( t );
        }
    } else {
        return state;
    }
}
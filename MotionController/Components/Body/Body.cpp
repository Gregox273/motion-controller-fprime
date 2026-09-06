// ======================================================================
// \title  Body.cpp
// \author greg
// \brief  cpp file for Body component implementation class
// ======================================================================

#include "MotionController/Components/Body/Body.hpp"

namespace {
void assertParamValid(Fw::ParamValid& valid) {
    FW_ASSERT(valid.e == Fw::ParamValid::VALID || valid.e == Fw::ParamValid::DEFAULT, valid.e);
}
}  // namespace

namespace MotionController {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

Body ::Body(const char* const compName) : BodyComponentBase(compName) {}

Body ::~Body() {}

// ----------------------------------------------------------------------
// Handler implementations for typed input ports
// ----------------------------------------------------------------------

void Body ::actuatorForce_handler(FwIndexType portNum, F32 value) {
    applyForce(value);
}

void Body ::run_handler(FwIndexType portNum, U32 context) {
    Fw::ParamValid isValid;

    const F32 frictionCoeff = this->paramGet_FrictionCoeff(isValid);
    assertParamValid(isValid);
    const F32 mass = this->paramGet_Mass(isValid);
    FW_ASSERT(mass != 0.0);
    assertParamValid(isValid);
    const F32 dt = this->paramGet_Timestep(isValid);
    assertParamValid(isValid);

    /* Euler integration */
    const F32 resultantForce = m_actuatorForce - (frictionCoeff * m_velocity);
    m_acceleration = resultantForce / mass;
    m_velocity += m_acceleration * dt;
    m_position += m_velocity * dt;

    this->tlmWrite_ResultantForce(resultantForce);
    this->tlmWrite_Acceleration(m_acceleration);
    this->tlmWrite_Velocity(m_velocity);
    this->tlmWrite_Position(m_position);
    
    this->plantState_out(0, m_position);
}

// ----------------------------------------------------------------------
// Handler implementations for commands
// ----------------------------------------------------------------------

void Body ::APPLY_FORCE_cmdHandler(FwOpcodeType opCode, U32 cmdSeq, F32 force) {
    applyForce(force);
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void Body ::RESET_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    reset();
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}
// ----------------------------------------------------------------------
// Private functions
// ----------------------------------------------------------------------
void Body ::reset() {
    m_actuatorForce = 0.0;
    m_position = 0.0;
    m_velocity = 0.0;
    m_acceleration = 0.0;
}

void Body ::applyForce(F32 force) {
    m_actuatorForce = force;
}

}  // namespace MotionController

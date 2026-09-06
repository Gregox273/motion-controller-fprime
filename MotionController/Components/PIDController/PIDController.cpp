// ======================================================================
// \title  PIDController.cpp
// \author greg
// \brief  cpp file for PIDController component implementation class
// ======================================================================

#include "MotionController/Components/PIDController/PIDController.hpp"

namespace {
template <typename T>
T clamp(T value, T min, T max) {
    T temp = value < min ? min : value;
    return temp > max ? max : temp;
}

void assertParamValid(Fw::ParamValid& valid) {
    FW_ASSERT(valid.e == Fw::ParamValid::VALID || valid.e == Fw::ParamValid::DEFAULT, valid.e);
}
}  // namespace

namespace MotionController {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

PIDController ::PIDController(const char* const compName) : PIDControllerComponentBase(compName) {}

PIDController ::~PIDController() {}

// ----------------------------------------------------------------------
// Handler implementations for typed input ports
// ----------------------------------------------------------------------

void PIDController ::measurement_handler(FwIndexType portNum, F32 value) {
    m_measurement = value;
}

void PIDController ::run_handler(FwIndexType portNum, U32 context) {
    if (!m_enabled) {
        return;
    }
    Fw::ParamValid isValid;

    const F32 Kp = this->paramGet_Kp(isValid);
    assertParamValid(isValid);
    const F32 Ti = this->paramGet_Ti(isValid);
    const F32 Ki = (Ti == 0.0) ? 0.0 : Kp / Ti;
    assertParamValid(isValid);
    const F32 Kd = Kp * this->paramGet_Td(isValid);
    assertParamValid(isValid);
    const F32 dt = this->paramGet_TimeStep(isValid);
    FW_ASSERT(dt != 0.0);
    assertParamValid(isValid);

    const F32 outputMin = this->paramGet_OutputMin(isValid);
    assertParamValid(isValid);
    const F32 outputMax = this->paramGet_OutputMax(isValid);
    assertParamValid(isValid);
    const F32 integralMin = this->paramGet_IntegralMin(isValid);
    assertParamValid(isValid);
    const F32 integralMax = this->paramGet_IntegralMax(isValid);
    assertParamValid(isValid);

    const F32 error = m_setpoint - m_measurement;

    /* Proportional term */
    const F32 pTerm = Kp * error;

    /* Integral term with anti-windup */
    F32 iTerm = m_integral + Ki * error * dt;
    iTerm = clamp<F32>(iTerm, integralMin, integralMax);
    m_integral = iTerm;

    /* Derivative term.
     * Operates on measurement rather than error so that it isn't affected by sudden changes in setpoint.
     */
    const F32 dTerm = -Kd * (m_measurement - m_previousMeasurement) / dt;
    m_previousMeasurement = m_measurement;

    F32 output = pTerm + iTerm + dTerm;
    output = clamp<F32>(output, outputMin, outputMax);

    this->tlmWrite_Error(error);
    this->tlmWrite_PTerm(pTerm);
    this->tlmWrite_ITerm(iTerm);
    this->tlmWrite_DTerm(dTerm);
    this->tlmWrite_Output(output);

    this->controlOutput_out(0, output);
}

void PIDController ::setpoint_handler(FwIndexType portNum, F32 value) {
    setSetpoint(value);
}

// ----------------------------------------------------------------------
// Handler implementations for commands
// ----------------------------------------------------------------------

void PIDController ::SETPOINT_cmdHandler(FwOpcodeType opCode, U32 cmdSeq, F32 setpoint) {
    setSetpoint(setpoint);
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void PIDController ::ENABLE_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    m_enabled = true;
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void PIDController ::DISABLE_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    m_enabled = false;
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void PIDController ::RESET_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    reset();
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

// ----------------------------------------------------------------------
// Private functions
// ----------------------------------------------------------------------

void PIDController ::reset() {
    m_setpoint = 0.0;
    m_measurement = 0.0;

    m_integral = 0.0;
    m_previousMeasurement = 0.0;

    m_enabled = false;
}

void PIDController ::setSetpoint(F32 setpoint) {
    m_setpoint = setpoint;
}

}  // namespace MotionController

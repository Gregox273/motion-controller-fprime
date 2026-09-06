// ======================================================================
// \title  PIDController.hpp
// \author greg
// \brief  hpp file for PIDController component implementation class
// ======================================================================

#ifndef MotionController_PIDController_HPP
#define MotionController_PIDController_HPP

#include "MotionController/Components/PIDController/PIDControllerComponentAc.hpp"

namespace MotionController {

class PIDController final : public PIDControllerComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct PIDController object
    PIDController(const char* const compName  //!< The component name
    );

    //! Destroy PIDController object
    ~PIDController();

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for measurement
    void measurement_handler(FwIndexType portNum,  //!< The port number
                             F32 value) override;

    //! Handler implementation for run
    //!
    //! Port to receive calls from the rate group
    void run_handler(FwIndexType portNum,  //!< The port number
                     U32 context           //!< The call order
                     ) override;

    //! Handler implementation for setpoint
    void setpoint_handler(FwIndexType portNum,  //!< The port number
                          F32 value) override;

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for commands
    // ----------------------------------------------------------------------

    //! Handler implementation for command SETPOINT
    void SETPOINT_cmdHandler(FwOpcodeType opCode,  //!< The opcode
                             U32 cmdSeq,           //!< The command sequence number
                             F32 setpoint) override;

    //! Handler implementation for command ENABLE
    void ENABLE_cmdHandler(FwOpcodeType opCode,  //!< The opcode
                           U32 cmdSeq            //!< The command sequence number
                           ) override;

    //! Handler implementation for command DISABLE
    void DISABLE_cmdHandler(FwOpcodeType opCode,  //!< The opcode
                            U32 cmdSeq            //!< The command sequence number
                            ) override;

    //! Handler implementation for command RESET
    void RESET_cmdHandler(FwOpcodeType opCode,  //!< The opcode
                          U32 cmdSeq            //!< The command sequence number
                          ) override;

  private:
    // ----------------------------------------------------------------------
    // Private functions
    // ----------------------------------------------------------------------
    void reset();
    void setSetpoint(F32 setpoint);

    // ----------------------------------------------------------------------
    // State variables
    // ----------------------------------------------------------------------
    F32 m_setpoint{0.0};
    F32 m_measurement{0.0};

    F32 m_integral{0.0};
    F32 m_previousMeasurement{0.0};

    bool m_enabled{false};
};

}  // namespace MotionController

#endif

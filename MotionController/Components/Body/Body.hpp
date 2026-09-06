// ======================================================================
// \title  Body.hpp
// \author greg
// \brief  hpp file for Body component implementation class
// ======================================================================

#ifndef MotionController_Body_HPP
#define MotionController_Body_HPP

#include "MotionController/Components/Body/BodyComponentAc.hpp"

namespace MotionController {

class Body final : public BodyComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct Body object
    Body(const char* const compName  //!< The component name
    );

    //! Destroy Body object
    ~Body();

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for actuatorForce
    void actuatorForce_handler(FwIndexType portNum,  //!< The port number
                               F32 value) override;

    //! Handler implementation for run
    void run_handler(FwIndexType portNum,  //!< The port number
                     U32 context           //!< The call order
                     ) override;

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for commands
    // ----------------------------------------------------------------------

    //! Handler implementation for command APPLY_FORCE
    void APPLY_FORCE_cmdHandler(FwOpcodeType opCode,  //!< The opcode
                                U32 cmdSeq,           //!< The command sequence number
                                F32 force) override;

    //! Handler implementation for command RESET
    void RESET_cmdHandler(FwOpcodeType opCode,  //!< The opcode
                          U32 cmdSeq            //!< The command sequence number
                          ) override;

  private:
    // ----------------------------------------------------------------------
    // Private functions
    // ----------------------------------------------------------------------
    void reset();
    void applyForce(F32 force);

    // ----------------------------------------------------------------------
    // State variables
    // ----------------------------------------------------------------------
    F32 m_actuatorForce{0.0};
    F32 m_position{0.0};
    F32 m_velocity{0.0};
    F32 m_acceleration{0.0};
};

}  // namespace MotionController

#endif

module MotionController {

    @ PID Controller
    passive component PIDController {

        ##############################################################################
        #### Uncomment the following examples to start customizing your component ####
        ##############################################################################

        # @ Example async command
        # async command COMMAND_NAME(param_name: U32)

        sync command SETPOINT(setpoint: F32)
        sync command ENABLE
        sync command DISABLE
        sync command RESET

        @ Telemetry report integrator value
        telemetry Error: F32
        telemetry PTerm: F32
        telemetry ITerm: F32
        telemetry DTerm: F32
        telemetry Output: F32

        # @ Example event
        # event ExampleStateEvent(example_state: Fw.On) severity activity high id 0 format "State set to {}"

        sync input port setpoint: ScalarF32
        sync input port measurement: ScalarF32

        @ Port to receive calls from the rate group
        sync input port run: Svc.Sched

        output port controlOutput: ScalarF32

        param Kp: F32 default 10.0
        @ Zero to disable integral term
        param Ti: F32 default 0.0
        param Td: F32 default 1.0
        param TimeStep: F32 default 0.1

        param OutputMin: F32 default -10.0
        param OutputMax: F32 default 10.0
        param IntegralMin: F32 default -10.0
        param IntegralMax: F32 default 10.0

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Enables command handling
        import Fw.Command

        @ Enables event handling
        import Fw.Event

        @ Enables telemetry channels handling
        import Fw.Channel

        @ Port to return the value of a parameter
        param get port prmGetOut

        @Port to set the value of a parameter
        param set port prmSetOut

    }
}

module MotionController {
    @ Plant model simulated using numeric integration

    passive component Body {

        # @ Example async command
        # async command COMMAND_NAME(param_name: U32)
        sync command APPLY_FORCE(force: F32)
        sync command RESET

        # @ Example telemetry counter
        # telemetry ExampleCounter: U64
        telemetry Position: F32
        telemetry Velocity: F32
        telemetry Acceleration: F32
        telemetry ResultantForce: F32

        # @ Example event
        # event ExampleStateEvent(example_state: Fw.On) severity activity high id 0 format "State set to {}"

        sync input port actuatorForce: ScalarF32
        sync input port run: Svc.Sched
        output port plantState: ScalarF32

        param Mass: F32 default 1.0
        param FrictionCoeff: F32 default 1.0
        param Timestep: F32 default 0.1

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

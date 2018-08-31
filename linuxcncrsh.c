/*
  Using linuxcncrsh:

  linuxcncrsh {-- --port <port number> --name <server name> --connectpw <password>
             --enablepw <password> --sessions <max sessions> --path <path>
             -ini<inifile>}

  With -- --port Waits for socket connections (Telnet) on specified socket, without port
            uses default port 5007.
  With -- --name <server name> Sets the server name to specified name for Hello.
  With -- --connectpw <password> Sets the connection password to 'password'. Default EMC
  With -- --enablepw <password> Sets the enable password to 'password'. Default EMCTOO
  With -- --sessions <max sessions> Sets the maximum number of simultaneous connextions
            to max sessions. Default is no limit (-1).
  With -- --path Sets the base path to program (G-Code) files, default is "../../nc_files/".
            Make sure to include the final slash (/).
  With -- -ini <inifile>, uses inifile instead of emc.ini. 

  There are six commands supported, Where the commands set and get contain LinuxCNC
  specific sub-commands based on the commands supported by linuxcncrsh, but where the 
  usual prefix ( "emc_") is omitted. Commands and most parameters are not case sensitive.
  The exceptions are passwords, file paths and text strings.
  
  The supported commands are as follows:
  
  ==> HELLO <==
  
  Hello <password> <client> <version>
  If a valid password was entered the server will respond with
  
  HELLO ACK <Server Name> <Server Version>
  
  Where server name and server version are looked up from the implementation.
  if an invalid password or any other syntax error occurs then the server 
  responds with:
  
  HELLO NAK
  
  ==> Get <==
  
  The get command includes one of the LinuxCNC sub-commands, described below and
  zero or more additional parameters. 
  
  ==> Set <==
  
  The set command inclides one of the LinuxCNC sub-commands, described below and
  one or more additional parameters.
  
  ==> Quit <==
  
  The quit command disconnects the associated socket connection.
  
  ==> Shutdown <==
  
  The shutdown command tells LinuxCNC to shutdown before quitting the connection. This
  command may only be issued if the Hello has been successfully negotiated and the
  connection has control of the CNC (see enable sub-command below). This command
  has no parameters.
  
  ==> Help <==
  
  The help command will return help information in text format over the telnet
  connection. If no parameters are specified, it will itemize the available commands.
  If a command is specified, it will provide usage information for the specified
  command. Help will respond regardless of whether a "Hello" has been
  successsfully negotiated.
  
  
  LinuxCNC sub-commands:
  
  echo on | off
  With get will return the current echo state, with set, sets the echo
  state. When echo is on, all commands will be echoed upon receipt. This
  state is local to each connection.
  
  verbose on | off
  With get will return the current verbose state, with set, sets the
  verbose state. When in verbose mode is on, all set commands return
  positive acknowledgement in the form SET <COMMAND> ACK. In addition,
  text error messages will be issued when in verbose mode. This state
  is local to each connection.
  
  enable <pwd> | off
  With get will return On or Off to indicate whether the current connection
  is enabled to perform control functions. With set and a valid password,
  the current connection is enabled for control functions. "OFF" may not
  be used as a password and disables control functions for this connection.
  
  config [TBD]
  
  comm_mode ascii | binary
  With get, will return the current communications mode. With set, will
  set the communications mode to the specified mode. The binary protocol 
  is TBD.
  
  comm_prot <version no>
  With get, returns the current protocol version used by the server,
  with set, sets the server to use the specified protocol version,
  provided it is lower than or equal to the highest version number
  supported by the server implementation.

  INIFILE
  Returns the path and file name of the current configuration inifile.

  plat
  Returns the platform for which this was compiled, e.g., linux_2_0_36

  ini <var> <section>
  Returns the string value of <var> in section <section>, in EMC_INIFILE

  debug {<new value>}
  With get, returns the integer value of EMC_DEBUG, in LinuxCNC. Note that
  it may not be true that the local EMC_DEBUG variable here (in linuxcncrsh and
  the GUIs that use it) is the same as the EMC_DEBUG value in the LinuxCNC. This
  can happen if LinuxCNC is started from one .ini file, and the GUI is started
  with another that has a different value for DEBUG.
  With set, sends a command to the LinuxCNC to set the new debug level,
  and sets the EMC_DEBUG global here to the same value. This will make
  the two values the same, since they really ought to be the same.

  set_wait none | received | done
  Set the wait for commands to return to be right away (none), after the
  command was sent and received (received), or after the command was
  done (done).

  wait received | done
  Force a wait for the previous command to be received, or done. This lets
  you wait in the event that "emc_set_wait none" is in effect.

  set_timeout <timeout>
  Set the timeout for commands to return to <timeout>, in seconds. Timeout
  is a real number. If it's <= 0.0, it means wait forever. Default is 0.0,
  wait forever.

  update (none) | none | auto
  With no arg, forces an update of the LinuxCNC status. With "none", doesn't
  cause an automatic update of status with other emc_ words. With "auto",
  makes emc_ words automatically update status before they return values.

  error
  Returns the current LinuxCNC error string, or "ok" if no error.

  operator_display
  Returns the current LinuxCNC operator display string, or "ok" if none.

  operator_text
  Returns the current LinuxCNC operator text string, or "ok" if none.

  time
  Returns the time, in seconds, from the start of the epoch. This starting
  time depends on the platform.

  estop (none) | on | off
  With no arg, returns the estop setting as "on" or "off". Otherwise,
  sends an estop on or off command.

  machine (none) | on | off
  With no arg, returns the machine setting as "on" or "off". Otherwise,
  sends a machine on or off command.

  mode (none) | manual | auto | mdi
  With no arg, returns the mode setting as "manual", "auto", or "mdi".
  Otherwise, sends a mode manual, auto, or mdi command.

  mist (none) | on | off
  With no arg, returns the mist setting as "on" or "off". Otherwise,
  sends a mist on or off command.

  flood (none) | on | off
  With no arg, returns the flood setting as "on" or "off". Otherwise,
  sends a flood on or off command.

  lube (none) | on | off
  With no arg, returns the lubricant pump setting as "on" or "off".
  Otherwise, sends a lube on or off command.

  lube_level
  Returns the lubricant level sensor reading as "ok" or "low".

  spindle (none) | forward | reverse | increase | decrease | constant | off
  With no arg, returns the value of the spindle state as "forward",
  "reverse", "increase", "decrease", or "off". With arg, sends the spindle
  command. Note that "increase" and "decrease" will cause a speed change in
  the corresponding direction until a "constant" command is sent.

  brake (none) | on | off
  With no arg, returns the brake setting. Otherwise sets the brake.

  tool
  Returns the id of the currently loaded tool

  tool_offset
  Returns the currently applied tool length offset

  load_tool_table <file>
  Loads the tool table specified by <file>

  home 0 | 1 | 2 | ... 
  Homes the indicated joint.

  jog_stop joint_number|axis_letter
  Stop the axis or joint jog

  jog joint_number|axis_letter speed
  Jog the indicated joint or axis at <speed>; sign of speed is direction

  jog_incr joint_number|axis_letter <speed> <incr>
  Jog the indicated joint or axis by increment <incr> at the <speed>; sign of
  speed is direction

  feed_override {<percent>}
  With no args, returns the current feed override, as a percent. With
  argument, set the feed override to be the percent value

  spindle_override {<percent>}
  With no args, returns the current spindle override, as a percent. With
  argument, set the spindle override to be the percent value

  abs_cmd_pos 0 | 1 | ...
  Returns double obj containing the XYZ-SXYZ commanded pos in abs coords,
  at given index, 0 = X, etc.

  abs_act_pos
  Returns double objs containing the XYZ-SXYZ actual pos in abs coords

  rel_cmd_pos 0 | 1 | ...
  Returns double obj containing the XYZ-SXYZ commanded pos in rel coords,
  at given index, 0 = X, etc., including tool length offset

  rel_act_pos
  Returns double objs containing the XYZ-SXYZ actual pos in rel coords,
  including tool length offset

  joint_pos
  Returns double objs containing the actual pos in absolute coords of individual
  joint/slider positions, excludes tool length offset

  pos_offset X | Y | Z | R | P | W
  Returns the position offset associated with the world coordinate provided

  joint_limit 0 | 1 | ...
  Returns "ok", "minsoft", "minhard", "maxsoft", "maxhard"

  joint_fault 0 | 1 | ...
  Returns "ok" or "fault"

  joint_homed 0 | 1 | ...
  Returns "homed", "not"

  mdi <string>
  Sends the <string> as an MDI command

  task_plan_init
  Initializes the program interpreter

  open <filename>
  Opens the named file

  run {<start line>}
  Without start line, runs the opened program from the beginning. With
  start line, runs from that line. A start line of -1 runs in verify mode.

  pause
  Pause program execution

  resume
  Resume program execution

  step
  Step the program one line

  program
  Returns the name of the currently opened program, or "none"

  program_line
  Returns the currently executing line of the program

  program_status
  Returns "idle", "running", or "paused"

  program_codes
  Returns the string for the currently active program codes

  joint_type <joint>
  Returns "linear", "angular", or "custom" for the type of the specified joint

  joint_units <joint>
  Returns "inch", "mm", "cm", or "deg", "rad", "grad", or "custom",
  for the corresponding native units of the specified axis. The type
  of the axis (linear or angular) is used to resolve which type of units
  are returned. The units are obtained heuristically, based on the
  EMC_AXIS_STAT::units numerical value of user units per mm or deg.
  For linear joints, something close to 0.03937 is deemed "inch",
  1.000 is "mm", 0.1 is "cm", otherwise it's "custom".
  For angular joints, something close to 1.000 is deemed "deg",
  PI/180 is "rad", 100/90 is "grad", otherwise it's "custom".
 
  program_units
  program_linear_units
  Returns "inch", "mm", "cm", or "none", for the corresponding linear 
  units that are active in the program interpreter.

  program_angular_units
  Returns "deg", "rad", "grad", or "none" for the corresponding angular
  units that are active in the program interpreter.

  user_linear_units
  Returns "inch", "mm", "cm", or "custom", for the
  corresponding native user linear units of the LinuxCNC trajectory
  level. This is obtained heuristically, based on the
  EMC_TRAJ_STAT::linearUnits numerical value of user units per mm.
  Something close to 0.03937 is deemed "inch", 1.000 is "mm", 0.1 is
  "cm", otherwise it's "custom".

  user_angular_units
  Returns "deg", "rad", "grad", or "custom" for the corresponding native
  user angular units of the LinuxCNC trajectory level. Like with linear units,
  this is obtained heuristically.

  display_linear_units
  display_angular_units
  Returns "inch", "mm", "cm", or "deg", "rad", "grad", or "custom",
  for the linear or angular units that are active in the display. 
  This is effectively the value of linearUnitConversion or
  angularUnitConversion, resp.

  linear_unit_conversion {inch | mm | cm | auto}
  With no args, returns the unit conversion active. With arg, sets the
  units to be displayed. If it's "auto", the units to be displayed match
  the program units.
 
  angular_unit_conversion {deg | rad | grad | auto}
  With no args, returns the unit conversion active. With arg, sets the
  units to be displayed. If it's "auto", the units to be displayed match
  the program units.

  probe_clear
  Clear the probe tripped flag.

  probe_tripped
  Has the probe been tripped since the last clear.

  probe_value
  Value of current probe signal. (read-only)

  probe
  Move toward a certain location. If the probe is tripped on the way stop
  motion, record the position and raise the probe tripped flag.

  teleop_enable
  Should motion run in teleop mode? (No args
  gets it, one arg sets it.)

  kinematics_type
  returns the type of kinematics functions used identity=1, serial=2,
  parallel=3, custom=4
  
  override_limits on | off
  If parameter is on, disables end of travel hardware limits to allow
  jogging off of a limit. If parameters is off, then hardware limits
  are enabled.

  optional_stop  none | 0 | 1
  returns state of optional setop, sets it or deactivates it (used to stop/continue on M1)
  
  <------------------------------------------------>
  
  To Do:
  
  1> Load / save connect and enable passwords to file.
  2> Implement commands to set / get passwords
  3> Get enable to tell peer connections which connection has control.
  4> Get shutdown to notify LinuxCNC to actually shutdown.
*/

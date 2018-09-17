[6.9.2018]
	
	1) Define the format of the command for buttons.

	2) Implement the gcodeview.
	3) Implement glview
	4) Make data polling from linuxcncrsh
	5) Make update of the data components
	6) Fix command lines for buttons.

[9.09.2018]

	GCodeView displays the current loaded code.
	When Openfile pressed the command associated woth open file is executed.

	1) Make a connection to the server and polling of the data.

[10.9.2018]
	
	There are 3 types of elements on layout:
	
1) The first type of elements are buttons which can have many states, controlled by themself or
	other layout elements.
	The buttons send message only if operator press on them.
	The status of the button can be changed as immideately or according to the polling of lcnc.

2) The second type are indicators, which are updating their states frequently, and according to the
	states set.

	As soon as the polling status of element is determined by it's state, the state should include
	update command, of update period of element.

	Indicators have a value variable, which is loading with some polling interval.
	These indicators are 

	gcodeview has more parameters which are updating, like
	gcode filename opened, position of the current line executed.

	To pick up the values of indicators which come in group one should make a special request and peek up the value from reply.
	for instance it's much faster to make
	"get joint_pos" and parse result than "get joint 0;; get joint 1;; ..."
	In this case I have to use group_update command flag instead of default individual update.
	It means that a group of layout elements will do single request and after peek up it's value from reply.


	Buttons for which I should track the state:
	estop (get estop)
	start (get machine)
	stop (get machine)
	manual (get mode)
	auto (get mode)
	mdi (get mode)

3) For the beginning I'll use indicators as a buttons with one state as soon as they display only one value (number of text)


[17.09.2018]

	1) CNC layout mainly implemented, 40% of buttons are working through linuxcnc.
	2) Gcodeview loads the code. Load of the code should trigger interpreter to process g-code.
	3) rs274ngc should be implemented to display trajectory in space.
	4) GL viewer should be implemented to display the position of the cutter, stock, chuck (for lathe), vise etc.
	5) Tool viewer should be able to display position of the tool as well as the geometry.







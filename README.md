# Things to do to run #

### Install OSQP (embedded version for running on hardware) ###

Our version of osqp_embedded is not yet public.

### Install ASIF++ ###

This is all of the code for the barriers. This is not yet public.

### Build this project as a ROS project ###

### Running ###

Important: Modify manual_teleop launch files with your keyboard input device (/dev/input/event*). Find this using 

cat /proc/bus/input/devices

Then search for your input device.

Launch main.launch for the single agent case, or two_drones.launch for the multi-agent case (the launch file for the example in the paper).

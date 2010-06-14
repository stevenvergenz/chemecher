
Hello developers,

Here we will be specific with particular goals for this program. Below is a list of what needs to happen for this program to be complete:

1) The program shall have a graphical user interface (GUI).

1.1) The GUI shall support the creation of new chemical mechanisms, steps, and species.

1.1.1) A mechanism shall consist of an unbounded set of steps, a name, and global settings, which will be discussed later.
1.1.2) A step shall consist of a long name, a short name, a set of up to three reactants, a set of up to three products, a forward rate and a backward rate.
1.1.3) A specie shall consist of a name, a state, a concentration, and a velocity. May also have a threshold for heterogeneous species (solid, liquid).

1.2) The GUI shall provide a means of modifying existing chemical mechanisms, steps, and species.

1.3) The GUI shall provide a means of managing input and output files.

1.3.1) The GUI shall 


==================================

FREE FORM IDEA NOTEPAD FOLLOWS:

for mechanism editor: have a canvas that supports multiple independent subwindows, so user can open multiple steps/species simultaneously while being able to arrange them however they see fit

output host: have a network host that consolidates all output done from any connected machine and puts output and log entries in an external repo, instead of on each local machine

non-interactive flag causes gui to not launch, but instead loads the given simulation and runs it without user input. this allows batch processing

output file name/path settings: allow a mechanism to specify a preferred filename/scheme and path so logging is consistent and the output is easy to find

functional concentrations over time: allow the user to define a special step that defines the concentration of one specie as a function of time instead of as a sum of reaction-product reactions.





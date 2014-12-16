P-Log - A probabilistic extention of ASP

=============
Introduction
=============

This project was aimed at augmenting the current reasoning of the robots to take advantage of
probabilities that doors are open.  To do this, I use P-Log, which is a language very similar to
what the robots use (ASP) but adds in probabilistic reasoning.  In the long term, I hope that this
project will be fully integrated into the robot, remove the need to reason about doors within ASP,
and have the robot learn the probabilities of doors being open.

This project requires the plog reasoner to be installed and accessed with the "p-log" command.


=============
Files
=============

I avoided storing everything in one large file in order to keep the components separate from
eachother.  Probabilities and goals can change, and it should be simple to go in and change just one
thing.  The data has to be separate since the program is reading in the same data file that the
robot is using to plan paths around the building.

assemble.cpp - 
  A simple program that combines all of the components into "everything.plg" so that the reasoner
  can take in one file.  It only rebuilds everything.plg if one of the components is updated.

data.plg - 
  Actually built from the robot's navigation_facts.asp to construct the set of all possible doors
  and rooms. P-Log and ASP work differently in defining sets, so I have to make the ASP file work
  for the P-Log reasoner.

doors_logic.plg - 
  Contains the heart of the project and actually finds out which door is most likely to be open.
  Should not have to be touched once completed.

goal.plg - 
  One line to specify the two rooms the reasoner should consider.

prbabilities.plg - 
  Stores the probabilities of certain doors being open.  As of now these probabilities are
  hard-coded and assumed to be 1/2.

plog.sh - 
  Runs both the assembler and the p-log program.

plog_demo.cpp - 
  A simple demo for the robots to show that everything works and can be used.
  

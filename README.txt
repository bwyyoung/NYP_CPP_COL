+==============================+
| Project COL (Circle of Life) |
| Version 0.38 ALPHA	       |
| Simian Softwerks 2007	       |
+==============================+

Welcome to Team Simian's AI Project. The goal of our project is to simulate 
life (using AI) in any area. Integrating 4 different classes of creatures 
(Plants, Prey, Predators and Chuck Norris), we intend on creating a moderately
accurate (and entertaining) model of wildlife.

To run Project COL, simply execute "ProjectCOL.exe" in the bin folder.

Team Members:
   /\                                            /\
  /__\    ==================================    /__\
 /\  /\  /Bryan Dismas Yeo Jun Rong (Leader)\  /\  /\
/__\/__\ ==================================== /__\/__\

-Predator FSM
-Graphical Framework
-Keyboard & Mouse Input
-Base Class Programming
-Cone-Search algorithm
-Chase algorithm
-Wander algorithm
-Selecting Entities
-UI System
-Minimap + Minimap Scrolling
-Status Panel
-Moving Entities
-Spawning Panel 
-Keyboard Scrolling
-Killing Entities
-States & State Manager
-Status Pane
-Moose FSM

   /\                                            /\
  /__\    ==================================    /__\
 /\  /\  /       Brian Young Wei Yan        \  /\  /\
/__\/__\ ==================================== /__\/__\

-Prey FSM
-Evading algorithm

   /\                                            /\
  /__\    ==================================    /__\
 /\  /\  /     Israel Chua Jia Liang        \  /\  /\
/__\/__\ ==================================== /__\/__\

-Chuck Norris FSM
-Timer Class

   /\                                            /\
  /__\    ==================================    /__\
 /\  /\  /        Jonan Gan Ming'en         \  /\  /\
/__\/__\ ==================================== /__\/__\

-Plant FSM

+=================+
| Project History |
+=================+
15 Nov 07
---------
-Added keyboard scrolling
-Kill Entities with the "Delete" key
-States and State Manager Done
-Game State added
-Creature spawning added.

14 Nov 07
---------
-Finished Status Pane
-Added grass to Minimap
-Added drag and drop mode for units.
-Updated Readme file.
-Added control & scope to the minimap (click the minimap to scroll), added buttons to the second panel for cursor mode.
-Status panel added. Basic UI coding done.

12 Nov 07
---------
-Minimap added.
-Wave Of Death Sprite Completed
-Wave Of Death now renders when 2 Chucklets collide
-Chuck Norris: RAGE
  -When Chuck Norris gets too hungry, he becomes RAGED
  -His speed and sight range are amplified.
  -He "viewing cone" is 360 degrees; He is completely aware of his surroundings.
  -The "Rage" word flashes above Chuck's head when he is raged.
  -Only when Chuck's hunger goes above 70 that he stops being raged.

11 Nov 07
---------
-Added better sprite rendering.

10 Nov 07
---------
-Added CHUCK NORRIS!!!
  -State Machine Completed: State changing works.
  -Chuck Norris can target any Entity
  -Wave Of Death Completed: A circular wave of destruction is produced when 2 Chucklets attack each other. Anything within a radius of 500 dies.
YOU GOTTA SEE THE MAN IN ACTION!!!! :D
-Added sprites and completed cursor and selection.
-Fixed depth sorting.

9 Nov 07
--------
-Object Manager and scrolling done.
-Added cursor and cursor loading code.
-Updated Readme file.
-Updated with the latest simian framework (which has mouse and keyboard 
support).
-TileGenerator done

8 Nov 07
--------
-finished predators
-Added more variables to Entity
-Added search condition checking.
-Comments added to Predator
-Modified library files..

7 Nov 07
--------
-Minor Edit, readme added.
-Base class is done! Chasing and Searching added. See Predator.h for usage.
-Wandering done. Example on how to use wandering in Predator.h/Predator.cpp

6 Nov 07
--------
-Base fsm class done. Predator sample added.
-Minor updates to rendering loop.
-Error checking and basic resource manager done.
-Base version for project COL added.



COPYRIGHT (C) Simian Softwerks 2007, Bryan Dismas, Brian Young, Israel Chua, Jonan Gan. All rights 
reserved. No source code can be destributed without the owners' prior knowledge.
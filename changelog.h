/*

TODO
Finished - I have marked inefficient code segments with disabled breakpoints, fix these.
Finished - Use class prototypes and declare the circularlry dependant functions after both classes are written
Finished - Add a source value for the second bus
Finished - Fix a bug that causes radiation shields to not drain power properly
Finished - Fix a bug that causes bus to still read as having power on it after the power source is destroyed
      NF - Totally in the wrong place, but set up some sort of active server program. It will serve as a placebo, as well as just checking what computers it can see every 10 seconds or so. To do this, give every computer a server (Put something in here to set it up) with a set of return codes. Have the computers update their local tables when they hit an error (Ex: set a local variable to 2 on fail to connect)
      NF - Fix a bug that causes the user to still be able to use bus 1 as if it is powered so long as they do not release a key (This is probably true for bus 2 also, cannot test yet)
      NF - Fix a bug that causes the bus watt values to fluctuate while modifying radiation shields (I have NO idea what causes this, kudos if you solve it)
      NF - Find out why sources other than Hab Reactor cant power primary bus
	  NF - Maybe find a better way to set up bitmaps than passing x and y to all of them? Excepting this, make good X and Y values. The overeall layout of ENG should stay the same so there is as little confusion as possible
	 WIP - Create images and make it draw things that aren't just text
Finished - Make a debouncer for the keys
	  NF - Make coolants do stuff
Finished - Clean up class functions using seperate source files
     WIP - Use a unique ID to track when the database was last changed

CHANGELOG
8/28/14  - Did everything that isn't documented because I started a changelog way too late
8/28/14  - Added a Changelog
8/28/14  - Added offline mode (Enter offline by setting the OFFLINE flag to 1)
8/29/14  - Removed 3 functions and instead used class prototypes to save 9 lines of code and much efficiency (As per earlier TODO)
9/4/14   - Made various changes for readibility on git
9/8/14   - Saved about 1000 cycles per tick at the cost of readability (Which is unnessecary, we only need to feed raw data to Allegro)
9/8/14   - Added support for destroyed module. When reported as destroyed, a module will be unable to be heated, cooled, or powered.
9/8/14   - Fixed multiple bugs surrounding destroyed power source module (See TODO)
9/9/14   - Fixed radiation shield power bug
9/9/14   - Added a source value to the secondary bus
9/9/14   - Against my better judgement, I have decided we will be using Allegro. SDL is too complicated, and everyone in Lisgar ICS alreeady knows Allegro, making this easier
9/12/14  - Allegro has been implemented
9/19/14  - Added framework required for displaying images with Allegro (No images yet, the code is commented ATM)
9/20/14  - Cleaned up the class structure and pre-main code a LOT
9/20/14  - Made program draw things rather than just print text
9/21/14  - Added a debouncer
9/21/14  - Put classes in their own headers
9/23/14  - Tried multiple ways to put functions with class headers, did not work, 3/10, would not reccomend to a friend
10/3/14  - Hey past me, you are dumb. I did classes for real now and its better
10/4/14  - After many many many many hours of work, I've finished the whole classes thing. It's now so much easier to read overall.
10/18/14 - Added ifndef guards to allow for machines without SQLAPI to run the offline mode

*/

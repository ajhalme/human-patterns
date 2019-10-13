# Human Patterns

Antti Halme, October 2019

### Summary

Human Patterns is a play concept that brings together Twister-like full body physical controls, a "Big Brother" themed hardware configuration, and a computer vision based pattern matching system.

This repository contains the first version of the software system that facilitates the play and brings all the parts together.

HumanPattern is a C++ application that leverages the UI library Qt, and the computer vision library OpenCV.

To run the game, you need a video source (designed for RTSP, any webcam should get you going) as well as AruCo markers (from the default marker set). Four markers define the play area.

Built with:

- OpenCV 5.12.4
- Qt Creator 4.10.0 (Qt 5.13.1)
- Ubuntu Mate 18.04

## Introduction

I started this project as a way to explore playful human-computer interaction. I've always been interested in the role of *play* in human culture, as a strange yet essential thing that we people do. Not just the games we play, but play more abstractly, more generally. 

Earlier in the year I had the pleasure of attending a workshop of sorts on physical interaction, and I got to thinking about the human body as a controller for collaborative play. Human play enabled by and in concert with a computer system. I firmly believe there is appetite for meaningful play experiences that do not reduce to tapping or clicking at a screen. I also wanted to explore a more artistic or expressive form of play, rather than one strictly focused on points scoring or classic arcade style action.

Movement based input controls have been around for a long time. Nintendo Wii, Microsoft's Kinect, and friends brought the motion sensor and sensing technology all the way to consumer markets already 10-15 years ago — and they certainly weren't the first ones to try. At the same time home computers and even mobile devices have become formidable computers capable of complex real-time input processing. Virtual and augmented reality apps are, if not fully mainstream, certainly well-established. People have been building fun stuff with these things in recent years, and I wanted to give it a try myself.

On the technical side, I've been meaning to make something with OpenCV for a while now, so this was a great opportunity. I wrote some proof of concept things in Python (opencv-python), but then switched to C++ for performance. (I haven't used C++ for years and years, so probably best not to read into the code too much.) I soon realised I needed a fully fledge UI framework, and decided to go with Qt as it appeared to play reasonably well with OpenCV libraries.

Human Patterns is not about new technology or powerful software. This is more about making fun use of available things: a playful bricolage of tech, if you will. I wanted to build something conceptually simple, but something that is intrisically playful in an unbounded way. Something whimsical and open and hackable.

While playing around with ideas of technology and play, I came across [Mozilla Festival](https://www.mozillafestival.org/en/). They had an open call for session proposals for the 2019 edition. I pitched an idea, and to my delight it got accepted. So I had an actual deadline and had to get cracking. My thing is perhaps not a great fit for Mozilla's current priorities, but I do think that demonstrating something like this to youths might inspire future makers to build their own fun, silly things — to shape their own play.


## Human Patterns

Human Patterns is a play concept that brings together Twister-like full body physical controls, a "Big Brother" themed hardware configuration, and a computer vision based pattern matching system.

The idea is that players try to mimic the patterns presented to them. The players form shapes with their bodies (and potentially some props) and the computer vision system matches this human input to the target pattern. The play is in the challenge of forming the target shapes. The patterns are of sufficient complexity that they typically cannot be completed with just one player, so in theory the play should become an exercise in communication and cooperative coordination.

As an additional challenge, the computer vision system makes use of a vertical orientation in which the camera is placed high up overhead. In other words, the players need to be aware of their body in three dimensions, and reason about their position from an unusual perspective.

The top-down camera view and related control was very much inspired by [Dynamicland](https://dynamicland.org/).


#### Play modes

Human Patterns currently has three play modes: Free, Simple, and Challenge.

The Free play mode works as a fun free-form activity, but its primarily function id help players understand the system and the controls. In Free play mode the game does not advance.

The more gamelike play modes are the two version of a timed challenge. There are currently two scoring functions for the timer mode: Simple and Challenge. In the Simple version players get points for "filling up" the target pattern, comprehensively lining up their shape inputs and the target pattern.

The Challenge mode works just like Simple, but players are also penalised for excess or overspill inputs that *do not* overlap with the target pattern. The scoring is based on pixel counting (an OpenCV standard): Simple is the percentage of pattern covered, while Challenge is the difference between the Simple score and scaled relative overspill (the amount of area covered outside the pattern, relative to the size of the target pattern). That is, for play score *Q*:

	- In freeplay, Q = 0
	- In Simple, Q = S
	- In Challenge, Q = S - (0.5 * E)

where *S* is the Simple score (relative area of covered pattern pixels to pattern area), and *E* is Excess (relative area of covered non-pattern pixels to pattern area).

The play advances to the next pattern when a score threshold is reached. The threshold is **60** by default, but can be configured on the fly and through the system configuration file.


## Human Patterns: The UI 

Human Patterns comprises two separate windows: the main controller and the game display. The main controller is the UI for the "host", the person running the play, while the game display is intended for players (and spectators).

### Main controller

The main controller has two parts to it: a video feed display and a set of controls running along the right hand side and bottom of the UI.

#### Connecting to the camera

Human Patterns play is set up by conneting to the camera and capturing the play area. The green square "Start" button at the bottom of the screen connects to the camera at the designated address marked "Camera (RTSP address)."

"Game Directory" indicates the currently selected "game", which in effect is a directory of pattern files. See Appendix A for the included games / pattern sets.

*Note that the default camera address is the one that matches the static IP of the PoE camera I happen to have available. The address can be changed through the config file.*

Once the camera connection is established, the main view should show the camera feed.

#### Play area capture

With the feed ready, the host running the game can establish the play area. This is done with the help of [ArUco AR markers](https://docs.opencv.org/trunk/d5/dae/tutorial_aruco_detection.html), which are supported by OpenCV out of the box. You can use this [handy ArUco marker generator](https://tn1ck.github.io/aruco-print/) to print out some markers. Note that Human Patterns is configured to use the original "default" ArUco marker set.

Placing four (arbitrary) ArUco markers in view of the camera will establish the play area. The detection algorithm makes the assumption that the markers are in the four quadrants around the centre point of the video feed. You can test whether the system is detecting the play area correctly by ticking "Show Markers", to show the detected markers, and "Show Play Area" to draw a green line between the detected markers using the marker corners closest to the centre of the view.

Once the play area is so described, it can be captured for use in the game. Clicking "Capture Playarea" switches the main controller to the game view, which yields a sequence of processed versions of the play area transformed into a square. The first target pattern is also displayed, as well as various combinations of the two.

"Save Baseline" takes a snapshot of the video feed and establishes the baseline image frame that is used by the system to determine what is background for the visual control.

"Launch a Game Display" launches a separate window (on secondary display, if available) to display game state information to players.

#### Game controls

The main controller window features several UI controls for configuring the system. The purposes of all of these controls is to make the game flexibly suitable and even playable for different environments and players. Humans come in many shapes.

"Save" and "Load" allow storing/retrieving configurations from the file system.

"Calibration" group controls modify the raw video feed, and can be used to improve the fidelity of the pattern matching inputs. "Blur" is helpful for reducing signal noise, and "Thresh" polarisation threshold can be used to determine the appropriate amount of input generated from a player stimulus.

"Audio" section has the audio on/off toggle, as well as sample sound buttons for demonstration or manual game directing purposes.

"Game" section controls manage the target patterns of the game. On the left, the radio controls select between play modes (as outlined in the previous section). "Game Dir..." button can be used to select between different "games", target pattern directories. The arrows switch patterns manually. Play/Reset/END can be used to control the state of the game and the timer on the game display.

"Adv.Q" stands for 'advancing score', which is the necessary game score needed to advance the player to the next pattern in the game, if any. Lowering the advancing score makes the game easier, and vice versa: this can be adjusted to the player level. Note that extremely high values make the game effectively unplayable, as matching the patterns perfectly is practically impossible.

"MinArea" stands for minimum area, and is another switch that helps in making the game playable. The minimum area is necessary with very small target patterns, as human players have a certain minimum size themselves. And, again, minimum input size varies according to the player.

On the far left end of the main window there are three colour coded number displays that show the simple score, the spill, and the game score — blue, red, and green, respectively.

#### Processed video feeds

In the game mode, the main controller shows a series of ten processed video feeds all in one.

The top row from left:

 - Baseline: baseline still of the game feed
 - Game feed: current raw input, with a square transformation on the marker defined play area
 - Target pattern
 - Polarised feed, an intermediate processing stage
 - Binary thresholding, the feed reduced to ones and zeros; "on" and "off" pixels
 
The bottom row from left:

 - A 50-50 blend of raw feed and pattern
 - Inverse of the pattern (useful for comparisons)
 - Covered area of the inverted target pattern
 - Spill area, covered area outside inverted target
 - Combination of covered and uncovered target and the spill, the "game view"
 

### Game Display

The game display is launched from the "Launch a Game Display" button on the main controller.

The main view of the game display is reserved for the "game view", which shows the covered and uncovered target as well as the "spillover", the covered pixels outside the target pattern. The target pattern is on the top right corner, and below it is the polarised and binary thresholded input feed, that is, the players' input.

The timer shows remaining play time. The timer can be paused and restarted from the main controller.

The three colour coded numbers are exactly those in the main controller: blue for covered pattern area, red for covered non-pattern area — both relative — and finally green for the game score, a function of blue and red, that depends on the play mode.

## Human Patterns: Code

Human Patterns is a UI application that leverages [QT](https://www.qt.io/) and the popular computer vision library [OpenCV](https://opencv.org/).

Built with:

- OpenCV 5.12.4
- Qt Creator 4.10.0 (Qt 5.13.1)
- Ubuntu Mate 18.04


### File walkthrough

#### Project files

- *humanpatterns-qt.pro* — The main project file for the app, a Qmake spec mostly managed by Qt Creator (Note Qt module 'multimedia', which is necessary for the audio)

- *opencv.pri* — Project include file for opencv

#### Components

- *common.h* — Common header file, a couple of shared image processing functions and a global enum

- *config.[h/cpp]* — Almost globally available app config type, with built-in file export/import <br />(The different configuration options are described in a separate section below)

- *frameprocessor.[h/cpp]* — Raw video processing: ArUco marker detection, play area capture algorithms, square transformation

- *gameDisplay.[h/cpp]* — Game display controller (players) and basic gameplay logic for timer and visuals

- *humanpatterns.[h/cpp]* — Main application: loads all other components, video device registration, frame capture, all main controller operations, game state management

- *main.cpp* — Application entry point

- *patternloader.[h/cpp]* — Pattern files manager: loading patterns and games, tracking game pattern selections

- *patternMatcher.[h/cpp]* — Main computer vision logic: input frame and pattern data processing, pattern matching, game state scoring

- *soundPlayer.[h/cpp]* — All things audio

#### Forms

- *hpgamedisplay.ui* — Game display: main game view, input pattern and input frame, score, timer

- *humanpatterns.ui* — Main UI: video feed view, app controls, calibration, game controls, and a copy of the current score

#### Resources

- *sounds.qrc* — Sounds as an embedded resource file: Start, Level up and Finish. All audio pulled from [Freesound.org](https://freesound.org/).

### Game configuration

Default config file is **config.yaml** in the application root directory. Other config files can be given as the first parameter to the app itself.

#### Parameters

- *baseDir* : application root directory
- *patternDirectory* : directory for debug pattern files
- *gamesDirectory* : directory for different games (pattern directories, see Appendix A)
- *defaultGamesDirectory* : default game
- *cameraAddress* : address for the camera video feed (RTSP)
- *blurValue* : pixel width of the blur applied to the raw feed
- *threshValue* : polarisation binary threshold, processing an image to shadeless black and white
- *gameMode* : 0 = Free, 1 = Simple, 2 = Challenge
- *advanceThreshold* : minimum game score to advance to the next pattern of the game
- *audioOn* : 0 = off, 1 = on
- *minPatternScore* : minimum pixel area of input, helps with small patterns
- *rotation* : camera rotation, 0 = 0 deg, 1 = 90 deg CCW, 2 = 180 deg, 3 = 270 deg CCW

## Appendix A: Pattern sets

 - *debug* : a few basic patterns
 - *mozfest* : letters M, O, Z, F, E (in reverse), S, and T as patterns
 - *mozfest2019* : 'mozfest' and four numbers, too
 - *mozfest_low* : 'mozfest', but with the pattern aligned to bottom, in case the game is played horizontally
 - *mozfest2019_low* : ditto for 'mozfest2019'
 - *shapes* : a library of untested shapes, for building sets that make sense for specific groups of players
 
Each target pattern is a B&W png image (can be whatever OpenCV can parse), 750x750 pixels, with the black pixels forming the target shape. The patterns in a directory are played in alphabetical filename order.

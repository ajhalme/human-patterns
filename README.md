## Human Patterns

Antti Halme, October 2019

#### Summary

Human Patterns is a play concept that brings together Twister-like full body physical controls, a "Big Brother" themed hardware configuration, and a computer vision based pattern matching system.

This repository contains the first version of the software system that facilitates the play, and brings all the parts together.

HumanPatternV1 is a C++ application that leverages the UI library Qt, and the computer vision libray OpenCV.

To run the game, you need a video source (designed for RTSP, any webcam should get you going) as well as AruCo markers (from the default marker set). Four markers define the play area.

Built with:
- OpenCV 5.12.4
- Qt Creator 4.10.0 (Qt 5.13.1)
- Ubuntu Mate 18.04

### Introduction

I started this project as a way to explore playful human-computer interaction. I've always been interested in the role of *play* in human culture, as a strange yet essential thing that we people do.Not just the games we play, but play more abstractly, more generally. There are so many questions — just to throw out a few:

- What is play? What is not play?
- *Why* do we play?
- Why is playing *fun*? Or *meaningful*?
- What's the connection between play and *flow*?
- Why do we stop playing, or play less when we get older? Or do we?
- What impact does *playing* have on our overall health and wellbeing?
- Can *playing* be creative?
- What is co-operative play?
- Can *machines* play? Can machines play with *us*?
- What is the role of play in a serious world?
- How does human play transfer to the digital realm?

Earlier in the year I had the pleasure of attending a workshop of sorts on human physical interaction, and I got to thinking about the human body as a controller for collaborative play. Human play enabled by and in concert with a computer system. I firmly believe there is appetite for meaningful play experiences that do not reduce to tapping or clicking at a screen. I also wanted to explore a more artistic or expressive form of play, rather than one strictly focused on points scoring or classic arcade style games.

Around the same time as I was thinking about play, I heard about [Mozilla Festival 2019](https://www.mozillafestival.org/en/). They had an open call for session proposals, so I pitched an idea and it got accepted. My thing is perhaps not a great fit for Mozilla's current priorities, but I do think that demonstrating something like this to youths might inspire future makers to build their own fun, silly things — to shape their own play, and not just consume the polished games big publishers put out.

On the technical side, I've been meaning to make something with OpenCV for a while now, so this was a great opportunity. I wrote some proof of concept things in Python (opencv-python), but then switched to C++ for performance. (I haven't used C++ for years and years, so probably best not to read into the code too much. I soon realised I needed a fully fledge UI framework, and decided go with Qt as it appeared to play reasonably well with OpenCV libraries.

### Human Patterns

Human Patterns is a play concept that brings together Twister-like full body physical controls, a "Big Brother" themed hardware configuration, and a computer vision based pattern matching system.

The idea is that players try to mimic the patterns presented to them by the system. The players form shapes with their bodies (and potentially some props) and the computer vision system then tries to match the human input with the target pattern. The play is in the challenge of forming the target shapes. The shapes are of sufficient complexity that they cannot be completed with just one player, so in theory the play should become an exercise in communication and coordination.

As an additional challenge, the computer vision system makes use of a vertical orientation in which the camera is high up overhead. In other words, the players need to be aware of their body in three dimensions, as seen from an unusual perspective.

#### Play Modes

Human Patterns has three play modes: Free, Simple, and Challenge.

The Free play mode works as a fun free-form activity by itself, but is mostly helpful for understandign the system and the controls. 

The more gamelike play modes are the two version of a timed challenge. There are currently two scoring functions for the timer mode: Simple and Challenge. In the Simple version players get points for "filling up" the target pattern, comprehensively lining up their shape inputs and the target pattern.

The Challenge mode works just like Simple, but players are also penalised for excess or overspill inputs that *do not* overlap with the target pattern. The scoring is based on pixel counting (an OpenCV standard): Simple is the percentage of pattern covered, while Challenge is the difference between the Simple score and scaled relative overspill (the amount of area covered outside the pattern, relative to the size of the target pattern). That is,

	- In freeplay, *Q* = 0
	- In Simple, *Q* = *S*
	- In Challenge, *Q* = *S* - (0.5 * E)

where *Q* is the play score, *S* is the Simple score (relative area of covered pattern pixels to pattern area), and *E* is Excess (relative area of covered non-pattern pixels to pattern area).

The play advances to the next pattern when a score threshold is reached. The threshold is **60** by default, but can be configured on the fly and through the system configuration file.


### Human Patterns V1: The UI 

TBW

### Human Patterns V1: Code

#### Game Configuration


#### Appendix A: Pattern sets

 - "debug" : a few basic patterns
 - "mozfest" : letters M, O, Z, F, E (in reverse), S, and T as patterns
 - "mozfest2019" : 'mosfest' and four numbers, too
 - "mozfest_low" : 'mosfest', but with the pattern aligned to bottom, in case the game is played horizontally
 - "mozfest2019_low" : ditto for 'mozfest2019'
 - "shapes" : a library of untested shapes, for building sets that make sense for specific groups of players
 
Each target pattern is a black&white png image (can be whatever OpenCV can parse), 750x750 pixels, with the black pixels forming the target shape. The patterns in a directory are played in alphabetical filename order.

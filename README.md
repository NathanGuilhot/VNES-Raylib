# ▼VИES: Raylib version▲

![VNES LOGO](VNES_LOGO.png)

VNES is an simple Visual Novel engine in [Raylib](https://www.raylib.com/), aimed to be ported to multiple platforms including console.

This repo is a port of [the original VNES](https://github.com/NightenDushi/VNES_Engine), developed for the NES in C.

---

## ▲Features

- Typewriter effect
- Branching
- Labels
- Easily define characters in script
  - Load their images automatically from a given prefix
- Flags and conditional jump (only with unsigned integer)
- Move, hide and change expression of characters in script
- Basic Save/Load system
- Backgrounds *(finally)*
- Music and sound system *(hell yeah)*

## ▲Planned

- Complete UI rewrite
- Draw images freely on screen
- Better Music effect (fadein, out, transitions)
- Tweening

[You can see my actual TODO-list here](todo.md)

---

## ▲FAQ

### ▹Is it a [Ren'Py](https://renpy.org/) competitor? 

No! Despite having a cool logo on Github, this engine is very humble.
Its goal is to be as lightweight as possible, and to be ported to as many platform as possible; especially homebrew libraries for retro console.

### ▹Is this production ready?

Oh my god no. It is still **very** early in development, and I'm almost learning C as I go.
This engine is primarily for my personal use, but I hope to make it more robust with time so that other peoples can enjoy it .

### ▹What consoles are you planning to support? When?

I will start porting the engine to other libraries once I'm happy with the essential features a basic visual novel would need.

I'm not decided on the consoles yet, but I hope to port it to Nintendo DS and PSP first because of backward compatibility.

Right now, thanks to Raylib, VNES is *technically* available on Windows, Linux, Mac, FreeBSD, Raspberry Pi, Android and HTML (and even more if you are feeling adventurous)

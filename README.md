# SithCodec
Converts audio formats for _Star Wars: Knights of the Old Republic_ & _Star Wars: Knights of the Old Republic II - The Sith Lords_

## Introduction
The audio files of _Star Wars: Knights of the Old Republic_ and _Star Wars: Knights of the Old Republic II: The Sith Lords_ (_KOTOR_) contain extra headers of what seem to be superfluous data that obfuscate access to the original audio outside of the video games. Most audio software will refuse to open the files, or interpret the extra bytes as audio data, leading to glitches. It has also been observed anecdotally that files lacking an appropriate header may fail to playback in certain releases of the games.

SithCodec is an application that can automatically detect and remove an extraneous header from a _KOTOR_ audio file, or alternatively prepend a _KOTOR_ header to a standard audio file and allow for the introduction of new audio content to these games.

## Requirements
- C++ 17 or later
- No third party libraries required
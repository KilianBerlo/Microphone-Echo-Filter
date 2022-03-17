# Microphone Echo Cancellation
In this project an echo cancellation application is designed and implemented on a Raspberry Pi.
First a sequential application is created after which the program is partitioned in parts in order to
enable the program to execute tasks on different processes at the same time. With the switch from
sequential to parallel, it is possible to apply echo cancellation in real time while maintaining the
functional behaviour, just as is the case in real-life situations.

## Hardware
Hardware used in the project:
- Raspberry PI
- USB Audio Adapter
- Microphone
- Speaker

## Technologies
Languages, libraries and versions used in the project:
- C++
- CMake 3.5
- GStreamer 1.0
- GStreamer Audio 1.0
- GStreamer App 1.0

## Launch
Build the project using CMake and then run ./Microphone-echo-filter.

## Contact
Kilian van Berlo - S2613352 - k.vanberlo@student.utwente.nl

Project Link: https://github.com/KilianBerlo/Microphone-Echo-Filter

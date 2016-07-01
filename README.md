# smssend
C++ plugin for YateBTS to easily send SMS through command line

## Installation
First of all, you need to install yate and yateBTS.
I chose to use evilBTS which you can find here:

```
https://github.com/evilsocket/evilbts
```

It contains Yate and YateBTS, with all patches for the bladeRF which I use.

When you have installed those, you can just use the given Makefile, running it as:

```
make && make install
```

Warning, the Makefile assumes that you installed yate at the following path: /usr/local/include/yate/

If you encounter any issues, please fill an issue on github.

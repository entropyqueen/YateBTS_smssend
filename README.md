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

## Usage

First, launch Yate, then connect to the remote interface:
```
$ rlwrap nc localhost 5038
```
(The use of rlwrap here is purely aesthetic, it provides the command line with history and line editing).

type the following command to get the help message:
```
help smssend
```
which outputs the following.
```
  smssend <MSISDN> <CALLER> <message>
send text messages to a particular MS
```

* MSISDN is the MSISDN of the target device.
* CALLER is the caller id, can be a MSISDN or garbage, or pretty much anything you want.
* message is the text message

You can retrieve the MSISDN of the device you want by running 

```
nib list registered
MSI            MSISDN 
--------------- ---------------
XXXXXXXXXXXXXXX 331969692
```
Then send the sms running:
```
smssend 331969692 0 this is a test
```

If the phone received the SMS, congratulation, everything works fine!


## Contributing

If you encounter any problems, please fill an issue on github, and if you have ideas and the time to develop them, do not hesitate to make forks or pull requests!

http://wiki.yatebts.com/index.php/Ybts_module

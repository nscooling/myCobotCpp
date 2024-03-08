
```
$ socat -d -d -v pty,rawer  pty,rawer
2024/03/07 11:18:09 socat[4777] N PTY is /dev/ttys007
2024/03/07 11:18:09 socat[4777] N PTY is /dev/ttys013
2024/03/07 11:18:09 socat[4777] N starting data transfer loop with FDs [5,5] and [7,7]

```

This command creates two connected pseudo terminal devices. The -d -d option increases the debug level to show the names of the created devices.

socat -d -d -d -v pty,rawer pty,rawer

* socat       : Multipurpose relay (SOcket CAT)
* -d -d       : Prints fatal, error, warning, and notice messages.
* -d -d -d    : Prints fatal, error, warning, notice, and info messages.
* -v          : Writes the transferred data not only to their target streams, but also to stderr.

* pty,rawer 
    PTY : Generates a pseudo terminal (pty) and uses its master side.
    rawer : Makes terminal rawer than raw option. This option implicitly turns off echo.

** NOTE ** `raw,echo=0` is obsolete use `rawer` instead


## Getting written output

```
$ xxd -l 5 -g 1 < /dev/ttys013
00000000: fe fe 02 14 fa   
```
-l 5 : exit after 5 bytes
-g 1 : group by 1-byte (default is 2)

## Running

```
./build/myCobotSimple-Serial /dev/ttys007 /dev/ttys013
```
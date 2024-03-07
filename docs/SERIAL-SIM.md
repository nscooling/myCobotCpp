
```
socat -d -d pty,raw,echo=0 pty,raw,echo=0
2024/03/05 17:03:44 socat[75389] N PTY is /dev/ttys025
2024/03/05 17:03:44 socat[75389] N PTY is /dev/ttys026

./build/SerialPortProject /dev/ttys025

cat < /dev/ttys026 > file.bin
xxd -g 1 file.bin > bytes.txt 
```

This command creates two connected pseudo terminal devices. The -d -d option increases the debug level to show the names of the created devices.

socat -d -d -d -v pty,rawer pty,rawer


socat       : Multipurpose relay (SOcket CAT)
-d -d       : Prints fatal, error, warning, and notice messages.
-d -d -d    : Prints fatal, error, warning, notice, and info messages.
-v          : Writes the transferred data not only to their target streams, but also to stderr.

pty,rawer 
    PTY : Generates a pseudo terminal (pty) and uses its master side.
    rawer : Makes terminal rawer than raw option. This option implicitly turns off echo.
pty,raw,echo=0

** NOTE ** `raw,echo=0` is obsolete use `rawer` instead


## Getting written output

```
$ xxd -l 5 -g 1 < /dev/ttys013
00000000: fe fe 02 14 fa   
```
-l 5 : exit after 5 bytes
-g 1 : group by 1-byte (default is 2)
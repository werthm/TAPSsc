TAPSsc
======

TAPS slowcontrol software

Installation
------------

### Dependencies
* ROOT 5.34 (with MySQL support)

### Installation
* Compile the software using `make clean ; make`
* Set some shell variables, e.g.

```
export TAPSSC="$HOME/TAPSsc"
export LD_LIBRARY_PATH="$TAPSSC/lib:$LD_LIBRARY_PATH"
```

Changelog
---------

### 0.0.2
November 12, 2013
* added QAC calibration
* improved MySQL query performance

### 0.0.1
November 4, 2013
* initial import


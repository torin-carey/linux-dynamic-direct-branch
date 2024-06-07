# Dynamic Direct Branch (x86-64)

This repo showcases a simple technique for branching to a dynamically obtained address with a near direct call in a Linux kernel module.

## Usage

**WARNING**: Don't try this at home. Safety of the system cannot be guaranteed.

_Note_: We assume that the _lockdown_ security policy isn't in place such that the loading of unsigned kernel modules is permitted.

The example requires the usual `make` as well as headers for the current kernel.

The kernel module can be compiled simply by running
```sh
$ make
```

and loaded with
```sh
# insmod ./ddb.ko && rmmod ddb
```

The output of this can be seen with `dmseg`:
```console
[  216.563935] mod: local_func : ffffffffc1f0c180
[  216.563942] mod: target     : ffffffff9f5a5540
[  216.563944] mod: rel        : ffffffffdd6993bb
[  216.563946] mod: e9 bb 93 69 dd
[  216.563951] Hello, World!
[  221.209151] mod: removing module
```

The `Hello, World!` log message is done via a call to a runtime modified stub function.

## Why?

Indirect calls/branches can cause issues with Indirect Branch Tracking (IBT), a control flow integrity security feature which requires the target to be marked for indirect branches (with `endbr`).

Turning this into a direct call can allow kernel modules to call functions that it obtains the addresses of at runtime without falling foul of IBT.

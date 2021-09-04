## Havoc Study

# Environment

ESC servers with 128-core 2.6GHz AMD EPYC™ROME 7H12 CPUs and 256 GiB RAM. The machine runs on Linux 4.15.0-147-generic Ubuntu 18.04 with RTX 2080ti.

## Setup
### Fuzzing with the original fuzzers
To fuzz with the original fuzzers, please refer to the following links which are the corresponding tutorials:

- [AFL](https://github.com/google/AFL)
- [AFL++](https://github.com/AFLplusplus/AFLplusplus.git)
- [FairFuzz](https://github.com/carolemieux/afl-rb.git)
- [MOPT](https://github.com/puppet-meteor/MOpt-AFL.git)
- [QSYM](https://github.com/sslab-gatech/qsym.git)
- [Neuzz](https://github.com/Dongdongshe/neuzz.git)
- [MTFuzz](https://github.com/rahlk/MTFuzz.git)

### Fuzzing with integrating Havoc via socket

Take FairFuzz integrated with the modified havoc as an example

```shell
# make the socket version of havoc & fairfuzz
$ cd AFL/afl-socket           && make -j
$ cd FairFuzz/fairfuzz-socket && make -j

# start the fuzzers respectively (Fairfuzz & Havoc)
# PORT_NUM: port number for the socket communication
# RUN_TIME: running time for each round in seconds (default=3600)
# ROUND_NUM: total number of execution rounds (default=12)
# FUZZ_IN: input seed directory
# CLIENT_OUT：output path for the client
# SERVER_OUT: output path for the server, i.e., Havoc

$ fairfuzz-socket/afl-fuzz -p $PORT_NUM -c -l $RUN_TIME -w $ROUND_NUM -i $FUZZ_IN -o $CLIENT_OUT -- /path/to/program [params] @@
$ afl-socket/afl-fuzz -p $PORT_NUM -s -l $RUN_TIME -w $ROUND_NUM -d -i $FUZZ_IN -o $SERVER_OUT -- /path/to/program [params] @@
```

### Fuzzing with Havoc DMA

```shell
# build
$ cd Havoc_DMA && make -j

# start fuzz
$ Havoc_DMA/afl-fuzz -d -i $FUZZ_IN -o $FUZZ_OUT -- /path/to/program [params] @@
```
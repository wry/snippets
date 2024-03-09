# captail

a simple tool to extract bytes from a pcap stream on stdin where each row represents a single packet

## use

$0 [\<format\>]

* $0 0
  * hex string encoded bytes
* $0 1
  * comma separated uint8\_t values for inclusion in code
* $0 2
  * ascii output

## examples

### extract bytes from stored capture

```shell
tcpdump -X -v -r capture.pcap | captail.pl
```

### extract bytes from live stream

```shell
tcpdump -X -v -i en0 -s 0 | captail.pl
```

### extract bytes from live stream, convert them to binary data and pass it to hexdump (or nc/ncat/socat/etc)

```shell
tcpdump -X -v -i en0 -s 0 | captail.pl | perl -pe 'chomp;$_=pack"H*",$_' | hexdump -C
```

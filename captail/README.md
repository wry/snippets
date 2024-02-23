# captail

a simple tool to extract bytes from a pcap stream on stdin
where each row represents a single packet

use :

* $0 0
  * hex string encoded bytes
* $0 1
  * comma separated uint8\_t values for inclusion in code

## examples

### extract bytes from stored capture

```shell
tcpdump -X -v -r capture.pcap | captail.pl
```

### extract bytes from live stream

```shell
tcpdump -i en0 -s 0 -X -v | captail.pl
```

### convert output to bytestream and pass it to hexdump/nc/etc

```shell
tcpdump -i en0 -s 0 -X -v | captail.pl | perl -pe 'chomp;$_=pack"H*",$_' | hexdump -C
```


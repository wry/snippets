# snif

A [libpcap](https://www.tcpdump.org/) based packet sniffer created for a specific use case, but kept as simple as possible so it could serve as boilerplate in the future.

## Features

* Capturing on interface
* Reading packet dump captured using `tcpdump -w`
* Several output formats to analyse packets
  * Plain hex string
  * Color coded hex string
  * Ascii

## Use

The options you can provide are limited, but some match those of tcpdump's.
* `-i interface` to select an interface to capture on
* `-r path` to read a previously captured packet dump
* `-f filter` to provide a tcpdump filter expression
* `-t output_format` to select an output format
  * `0` to select plain hex string
  * `1` to select color coded hex string
  * `2` to select ascii

## When should you use it

In general you shouldn't, since it in itself is **very** limited compared to tcpdump, so you're much better off using that. However, if you know some unix-fu you'll find ways, I'm sure.

If you want something more flexible, you might be interested in [captail](https://github.com/wry/snippets/tree/master/captail). It doesn't have the color coded hex string output, but it can help you process tcpdump's live/stored captures that you can then either replay to another host (piping it to nc/ncat/socat/etc) or generate uint8_t arrays for your code.

## License

All components of the project use the MIT license.
# gentxt

`gentxt` is a tiny program that renders text as a video stream.

## history

while assisting in the development of an embedded libreelec based video player the need came up to show relevant information to users. since the player has no GUI and uses the fb device to stream video there's no easy way to render text into the stream.

the idea of developing a solution to the problem was quickly dismissed and the company opted to pre-render the text messages instead. however, as the aftermath of a showerthought i created this tiny player over the weekend.

## details

to get fonts primitive enough to rasterize i chose the simtel dos font blobs, which are easy to find on the net. each 8 byte represents 8 pixel rows of a single font. these then get converted to a byte array and stored in `font.i`, so it can be included in `main.c`. the output can either be `float` or `uint8_t`, with latter then used for video playback using `ffplay`.

## use

* get the fntcol16.zip package, eg. [mirror](https://ftp.sunet.se/mirror/archive/ftp.sunet.se/pub/simtelnet/msdos/screen/fntcol16.zip))
* get a font blob, eg. `unzip -p fntcol16.zip VGA-ROM.F08 | perl -ne '$hex = unpack "H*"; print join(",", map{"0x$_"} ($hex=~m/../g))."\n";' > font.i`
* build the prog with `make`
* display the text using ffmpeg, eg. `./gentxt 'hello world!' | ffplay -f rawvideo -pixel_format rgba -video_size 300x300 -`

alternatively, if you change the typedef of `CType` to `float`, you can store it in `out.png`, with :

`./gentxt 'hello world!' | convert -size 300x300 -depth 32 -define quantum:format=floating-point -endian lsb rgba:- out.png`

## related

`vidgen`, which renders an animation also uses ffmpeg and is also part of my `snippets` collection.

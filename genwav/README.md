# genwav

`genwav` is a tiny program that renders a [wav](https://en.wikipedia.org/wiki/WAV) file.

## use

if you just compile the program and run it, it will output a valid WAV file to stdout.

```shell
./genwav > audio.wav && ffplay audio.wav
``` 

you can also directly play the audio using ffplay's `pipe` input

```shell
./genwav | ffplay -i pipe:0
```

the program can also serve as the base for pcm generation, if you don't generate the WAV header. in this case you'll have to pass the attributes of the stream to ffplay :

```shell
./genwav | ffplay -f s16le -ar 44100 -ac 1 -i pipe:0
```
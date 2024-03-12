# libclang

a libclang example that generates cpp macro calls in the form `MACRO(CLASS, FIELD)`, to generate getters to structs found in a header file.

# compilation

in its current form it requires `llvm` to be installed through brew, so if you've installed `libclang` elsewhere, just update the Makefile and run `make`.

# history

originally created for my gdl90 library's wasm binding, to facilitate getting the values of fields in the struct instances returned by the library. you can search for `GDL90_SYNTHESIZE` in its repo to see how it works.

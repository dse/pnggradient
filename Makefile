SRC = src/pnggradient.c
HDR = src/pnggradient.h
BIN = bin/pnggradient

$(BIN): $(SRC) $(HDR) Makefile
	cc -o $(BIN) $(SRC) `pkg-config --libs --cflags libpng`

examples: $(BIN) Makefile
	mkdir -p examples
	find examples -type f -name '*.png' -exec rm {} +
	bin/pnggradient examples/example.png
	bin/pnggradient -o examples/horizontal.png
	bin/pnggradient -d examples/dithered.png
	bin/pnggradient -c '#ff0000' -C '#0000ff' examples/redblue.png
	bin/pnggradient -o -c '#ff0000' -C '#0000ff' examples/horizontal-redblue.png
	bin/pnggradient -c '#ff0000' -C '#ff000000' examples/alpha.png

.PHONY: examples

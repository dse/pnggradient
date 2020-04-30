# pnggradient

Generates gradient `.png` images.

Can be horizontal or vertical.  Can be randomly dithered.

## Usage

```
pnggrain [<options> ...] [<filename>]
```

Default filename is `pnggradient.png`.  Specify a *filename* argument
to override it.  Use `-` as your *filename* argument to write the PNG
to stdout.  Use `./-` to write to a file called `-`.

## Options

-   `-W <int>` specifies image width (there's a default, it's 256).
-   `-H <int>` specifies image height (there's a default, it's 256).
-   `-c <color>` specifies the start color
-   `-C <color>` specifies the end color
-   `-d` randomly dither.
-   `-o` horizontal gradient (vertical by default).
-   `-h` or `-?` displays a help message.

## Colors can be:

-   `#rgb`
-   `#rgba`
-   `#rrggbb`
-   `#rrggbbaa`

## Examples

-   ![basic example](examples/example.png)
-   ![dithered](examples/dithered.png)
-   ![red to blue](examples/redblue.png)
-   ![alpha gradient](examples/alpha.png)
-   ![horizontal gradient](examples/horizontal.png)
-   ![horizontal red to blue](examples/horizontal-redblue.png)

## Build Process and Requirements

Requires [libpng](http://libpng.org/pub/png/libpng.html), any C
compiler, and a `make` utility.

If you don't have the
[`pkg-config`](https://www.freedesktop.org/wiki/Software/pkg-config/)
utility, then you have no soul and I hate you.

(Not really.)

To build the executable, simply type `make`, and the compiler will
store the executable as `bin/pnggradient`.

There is no `make install` process at this time.  Simply copy
`bin/pnggradient` to `/usr/local/bin` or something.

There is no manual page.  Simply type `pnggradient -h` for help.
There are no long options.

## See Also

-   [pnggrain](https://github.com/dse/pnggrain)

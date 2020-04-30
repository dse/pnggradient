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

-   ![basic example](example.png)
-   ![dithered](dithered.png)
-   ![red to blue](redblue.png)
-   ![alpha gradient](alpha.png)
-   ![horizontal gradient](horizontal.png)
-   ![horizontal red to blue](horizontal-redblue.png)

# DDR info generation

## Generating DDR debug information

### `ddrgen`
TODO

### `macro-tool`
TODO

### In-Source Annotations
C/C++ source annotations can be used to configure how DDR generates it's debug information.

Following are the different annotations provided by OMR.

`@ddr_namespace <format>`: Set the DDR namespace of all following C macros.

Possible values for `<format>`:
- `default`: Place macros in the default DDR namespace, named after the input source.
- `map_to_type=<type>`: Place macros in the DDR namespace `<type>`.

Setting the DDR namespace allows users to organize generated DDR info into logical categories. `@ddr_namespace` may be
called more than once. When DDR translates DDR macro info into Java sources, the Java class name is derived from the
DDR namespace.

`@ddr_options <option>`: Configure additional processing options.

Possible values for `<option>`:
- `valuesonly`: All *constant-like* macros (E.G. `#define x 1`) are added as a constant to the DDR namespace.
  *function-like* and empty macros are ignored. This is the default.
- `buildflagsonly`: All empty `#define`d and `#undef`ed symbols are added as constants to the DDR namespace. Macros are
  assigned a value of `1` if `#define`d, or `0` if `#undef`ed.
- `valuesandbuildflags`: This option combines the behavior of both `valuesonly` and `buildflagsonly`.

### Properties files

### Blacklisting Sources

# `omr_main_function`

`omr_main_function` is a static library that helps OMR developers write portable executables. This library defines the `int main()` function, which handles conversion of `wchar_t` or EBCDIC command-line options into OMR's internal string format (UTF-8), before calling out to the user provided `omr_main_entry` function.

## Using omr_main_function

link privately against the `omr_main_function` library. The main entry to your program is now:

```c++
extern "C" int
omr_main_entry(int argc, char **argv, char **envp);
```

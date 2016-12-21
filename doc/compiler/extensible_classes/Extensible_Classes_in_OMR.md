# Extensible Classes in OMR  {#OMRExtensibleClasses}

This document describes the use of extensible classes in the Eclipse OMR source code. For a more general description of extensible
classes, see the `Extensible_Classes.md` (\\subpage ExtensibleClasses) document. If you are unfamiliar with extensible classes, it is strongly recommended
that you read that document first.

## Projects

We currently have a few projects using extensible classes in the OMR suite.
These are:

| Project    | Location |
|------------|----------|
| OMR        | A collection of infrastructural compiler components that can be used to construct optimizing compilers for language front ends or virtual machines. |
| JitBuilder | A framework of components that abstracts away some of the complexity of JIT compilers. |
| Test       | A compiler testing framework. |

## Namespaces

Each project is given a namespace as follows:

- `OMR`
- `JitBuilder`
- `Test`

Each project also provides some specializations for the various supported architectures. These are also assigned
namespaces as follows:

- `ARM`: for the ARM architecture
- `Power`: for IBM Power architecture
- `X86`: for general x86
  - `AMD64`: for 64-bit x86 (x86-64)
  - `i386`: for 32-bit x86 (x86-32)
- `Z`: for the IBM z Systems architecture

Additionally, there is a `TR` namespace, used by all projects. This namespace
holds all concrete classes.

## Directories

As is facilitated by the use of extensible classes, our directory structure closely reflects the organization of the various
projects and their specializations. The project directories are:

- `omr`
- `jitbuilder`
- `test`

The architecture sub-directories are:

- `arm`: for ARM
- `p`: for IBM's Power architecture
- `x`: for x86
  - `amd64`: for x86-32
  - `i386`: for x86-64
- `z`: for IBM z Systems architecture

## Header Guards

The following style is used to guard all our header files:

```c++
#ifndef <ALL CAPS NAMESPACE>_<ALL CAPS CLASS NAME>_INCL
#define <ALL CAPS NAMESPACE>_<ALL CAPS CLASS NAME>_INCL

// ...

#endif
```

As an example:

```c++
#ifndef OMR_X86_CODECACHE_INCL
#define OMR_X86_CODECACHE_INCL

// ...

#endif
```
and also:

```c++
#ifndef TR_CODECACHE_INCL
#define TR_CODECACHE_INCL

// ...

#endif
```
## Connectors

Class extension connectors are named by taking the extension class' name and
appending `Connector` .
As an example:

```c++
namespace OMR { namespace X86 { class CodeCache; } }
namespace OMR { typedef OMR::X86::CodeCache CodeCacheConnector; } // `OMR` is specified for clarity
```

These are located above `#include` directives in header files, and are guarded
from redefinition by defining a macro at the same time. This ensures the
connector is `typedef`ed to the most specialized version of a class. For example:

```c++
#ifndef OMR_OPTIMIZATION_CONNECTOR
#define OMR_OPTIMIZATION_CONNECTOR
namespace OMR { class Optimization; }
namespace OMR { typedef OMR::Optimization OptimizationConnector; }
#endif
```

## Linting

Extensible classes can be linted for common problems using `OMRChecker`, a
clang-based linter. This can be run on the Test compiler using `make lint` in
the root directory, or other projects by using their `linter.mk` makefiles.

For the linter to know a class is extensible, they are marked as extensible by
adding an `OMR_EXTENSIBLE` annotation to their definition:
```c++
class OMR_EXTENSIBLE Optimization
```

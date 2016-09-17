# Extensible Classes in OMR  {#OMRExtensibleClasses}

This document describes the use of extensible classes in the Eclipse OMR source code. For a more general description of extensible
classes, see the `Extensible_Classes.md` (\\subpage ExtensibleClasses) document. If you are unfamiliar with extensible classes, it is strongly recommended
that you read that document first.

## Projects

We currently have X projects as part of our OMR suite. These are

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
- `TR` (namespace for concrete classes)

Each project also provides some specializations for the various supported architectures. These are also assigned
namespaces as follows:

- `ARM`: for the ARM architecture
- `Power`: for IBM Power architecture
- `X86`: for general x86
  - `AMD64`: for 64-bit x86 (x86-64)
  - `i386`: for 32-bit x86 (x86-32)
- `Z`: for the IBM z Systems architecture

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

Class extension connectors are named by taking the extension class's name and appending `Connector` (yes, it's un-creative but it works).
As an example:

```c++
namespace OMR { namespace X86 { class CodeCache; } }
namespace OMR { typedef OMR::X86::CodeCache CodeCacheConnector; } // `OMR` is specified for clarity
```

[guard convention?]

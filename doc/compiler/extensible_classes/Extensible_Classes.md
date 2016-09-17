# Extensible Classes {#ExtensibleClasses}

## Introduction

Extensible classes are a way of implementing static polymorphism in C++. They allow various projects to override and extend the
functionality of an existing class, through inheritance, in order to suit their specific needs. This is done by carefully organizing a
project in a way that lets the compiler find the most extended implementation of a function.

This document describes the general characteristics of extensible classes as well as their implementation. Although there are alternative
extensible classes implementations, of all the approaches explored, the one presented here has been the most effective.

## What is an extensible class?

Suppose we have a group of projects that are similarly structured. They share similar features and also have similar implementation.
It makes sense to abstract away the common functionality and structure so that it may be reused more effectively.

Extensible classes organize code in a way that allows the structure and implementation of a particular class to be extended.
Conceptually, when extending a class, we do not create a new type. Instead, we take a previously implemented type and extend its functionality
to suit our needs. We can access an instance of an extended class through a reference to the non-extended version because they are
really just the same type (though doing so also hides the extended functionality). It is therefore common to refer to an entire
extensible class hierarchy as a single class.

In addition, extensible classes also make it possible to define specialized extensions for use is more specific situations. These can be very
useful, for example, in cases where some special functionality is required to support a specific hardware architecture
(a common problem in compiler development).

## Implementation

In essence, an extensible class is just a hierarchy of normal C++ classes. What makes extensible classes special is how the project files,
directories, and class hierarchies are organized.

### Organization

Extensible classes organize the implementation of a class into a few distinct parts. These are: the class extensions, extension
specializations, and the concrete classes.

A class extension is a class that extends the implementation of an extensible class. Class extensions are abstract. They cannot be
instantiated; only extended. An extension inherits the functionality of the parent extension and adds its own functionality to it
(*extends* it). It can also *override* the parent's functionality.

A special case of a class extension is the base class that gets extended by other classes. Although it does not extend the structure or
functionality of any existing class, it is treated the same was as all other class extensions.

Extension specializations are classes that specialize the functionality of a particular class extension. They allow us to
define different implementations of the same class extension. We can then ask the compiler to use a specific specializations.
This is particularly useful for organizing platform specific code.

Concrete classes are at the bottom of an extensible class hierarchy. They realize the implementation of the class extension they
inherit from. These are the only classes in an extensible hierarchy that can be instantiated.

### self()

The fundamental principle of extensible classes is to let the compiler find the most derived (extended) implementation of class functions.
This is accomplished by casting `this` to a pointer to the most derived type. So, when calling a function on a class, the compiler
starts searching for the function's definition from the bottom of the class hierarchy. If the function is not defined in a given class, then
the compiler will search the class's parent.

> Note that in order for the compiler to search the entire class hierarchy, the down cast *must* always be to the most derived type.

We can tell the compiler what the most derived type is with a forward declaration. We decide upfront what the name of the most derived
type will beand restrict ourselves to only using this name. This is why only the concrete class may be instantiated. By ensuring that
only the most derived type can be instantiated, we guaranty that the cast is always to the type of the object referenced by `this`.

For convenience, we define a special function in the base class called `self()`. It simply does a `static_cast` of the `this` pointer
to a pointer to the most derived type (which we forward declared) and returns the result. All calls to member functions from within an
extensible class must be prefixed with `self()->`. Using the implicit `this` pointer **must be avoided** as it circumvents the down cast
and can lead to strange and unpredictable behaviour.

> To enforce this rule, we use a clang-based linter that uses a class annotation to verify that extensible classes
are implemented correctly.

### Naming and Namespaces

As previously mentioned, we must know the name of the concrete class in a hierarchy (most extended/derived type) when we declare the
base class. Since we consider all extension classes, extension specializations, and the concrete class as essentially the same
class, we give them all the same name and use namespaces to distinguish them.

Each project has its own namespace to hold whatever class extensions it has. Nested namespaces are used for the extension
specializations provided by projects. Concrete classes all go in their own, separate namespace.

### Connectors

Extending a class in a project is done in the source code itself by creating a new class that inherits from the class being extended.
However, selecting a particular extension specialization is done by setting the include path. As a result, it is not always clear
which extension to inherit from.

In order to create a more uniform way of inheriting from a class extension, we do not inherit from the class directly but instead use
what we call a "connector". A connector is a `typedef` of the class extension/specialization. Instead of being in the same namespace
as its class, the connector is declared in the project's namespace. Each specialization (including the _non-specialized_ class
extension) overrides the class extension's connector. `#define` guards are used to ensure only one connector is ever defined for a
given class extension. In addition, to ensure the correct connector is selected by the compiler, its definition must be placed before
any `#include` statements.

Using connectors has the advantage of allowing a different extension specialization to be selected by simply changing a compiler
flag, without having to change the source code itself.

### Files and Directories

Another key aspect of extensible classes is how files and directories are organized.

The simplest way to organize extensible class hierarchies is to create a separate directory for each level of extension. That is, a
directory for just the base classes, a directory containing the first level of extensions, a directory containing the second, etc.
This will usually reflect the separation of the different projects (one directory per project that extends a class). We therefore
call these "project directories".

A project directory must, as a minimum, declare the class extensions it needs and re-declare (or simply declare if the project
is the base) the concrete classes as inheriting from the new extensions. By convention, the name of a file for a class extensions
should be the project's namespace followed by the class name. Files for concrete classes should just be named after the class name.

Project specializations must be put in sub-directories of a project directory. These do not need to (but may if needed) re-declare the
concrete class.

If directories are used to organize the (potentially large number of) files in a project, the organizational structure *must* be maintained
across all projects and specializations. So, if one projects puts certain classes in a specific directory, all other projects must put those
same classes in a directory with the same name. Also, to maintain best compatibility across different compilers, the name of these
directories should be put as part of `#include` directives. This is because certain compilers (e.g. `gcc`) always search the current
directory first, before looking in the include path.

### The Include Path

The last important aspect of extensible classes is how the include path (i-path) is set. In order for the compiler to find the most
extended version of a class, the path to the most specialized/extended classes must be specified first, followed by the more general
ones, and so on until the path to the base project.

Likewise, header files for extensible classes must be included from most extended to most general. The only exception to this is the
inclusion of the header file for a particular class in the corresponding source file. In this case, the file should be included first.

### The Linter and class annotations

When using extensible classes, there are many things to keep track of and many things that can go wrong.
This is especially true when it comes to the use of `self()`. Forgetting to use it can result in some very strange behaviour.

To help avoid this problem, we use a [clang tool](http://clang.llvm.org/docs/ClangTools.html) (a linter) that detects and reports
these kinds of errors in the OMR project. When a class is annotated with `__attribute__((annotate("OMR_Extensible"))`, the tool will
ensure that:

* `self()` is used were needed
* the base of extensible hierarchies are in the correct namespaces
* concrete classes are in the correct namespace, and
* the annotation is use in all classes of the hierarchy

For convenience, we also define the macro `OMR_EXTENSIBLE` to make the annotation easier to read (and use) and to maintain
compatibility with other compilers.

Currently, however, the tool only searches for errors in the use of extensible classes in the OMR project specifically.


## Extensible enums

[TODO] ?

## Pros and cons of extensible classes

[TODO]

Pros:

* does not produce run-time overhead
* encourages good code base organization

Cons:

* large number of files
* no multiple inheritance
* easy to get wrong

## Comparison with other approaches

### Runtime polymorphism

### The CRTP

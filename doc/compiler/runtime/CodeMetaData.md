!--
Copyright (c) 2017, 2017 IBM Corp. and others

This program and the accompanying materials are made available under
the terms of the Eclipse Public License 2.0 which accompanies this
distribution and is available at https://www.eclipse.org/legal/epl-2.0/
or the Apache License, Version 2.0 which accompanies this distribution and
is available at https://www.apache.org/licenses/LICENSE-2.0.

This Source Code may also be made available under the following
Secondary Licenses when the conditions for such availability set
forth in the Eclipse Public License, v. 2.0 are satisfied: GNU
General Public License, version 2 with the GNU Classpath
Exception [1] and GNU General Public License, version 2 with the
OpenJDK Assembly Exception [2].

[1] https://www.gnu.org/software/classpath/license.html
[2] http://openjdk.java.net/legal/assembly-exception.html

SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
-->

# Code Metadata and Code Metadata Manager in the Eclipse OMR compiler

## Introduction

This document explains the concepts and implementation of runtime
code metadata and code metadata manager in the Eclipse OMR compiler.

## Code Metadata

Code metadata contains runtime metadata information about a method. 
Metadata may include the starting address of allocated code memory and its size, 
the entry point address of compiled code (when it is invoked from either 
an interpreter or another piece of compiled code), the address where the 
compiled code ends, the compilation hotness level, a list of runtime assumptions 
made during the compilation, and any other information required by the runtime.

An instance of code metadata will be created for each method after
it is compiled. The code metadata instance will exist for the duration of the 
method's runtime until it is unloaded or invalidated. Storage and 
lookup of code metadata instances are carried out by the code metadata manager.

In the Eclipse OMR compiler, an instance of code metadata is uniquely identified 
by its start and end PC addresses. Two instances of code metadata with PC address 
ranges that overlap are not allowed.

## Code Metadata Manager

The code metadata manager is a singleton class created in the runtime to manage 
code metadata instances. The manager class constructor allocates an AVL tree 
in persistent memory to facilitate storage and lookup of code metadata instances 
created by the compiler. Each tree node in the AVL tree is a hash table representing 
a piece of code cache segment. A node is created and added to the AVL tree when a 
new code cache segment is allocated. For [Eclipse OpenJ9](https://github.com/eclipse/openj9), 
code cache segment size is configured to be 2MB. Entries in each hash table correspond 
to equal chunks of the associated code cache. For [Eclipse OpenJ9](https://github.com/eclipse/openj9), 
each hash table has 4096 entries and each entry is responsible for 512 bytes of the 2MB 
code cache. A hash table entry points to a metadata pointer or a list of metadata 
pointers whose compiled code occupies a part of the chunk or the entire chunk of 
the code cache.

For efficient insertion, deletion, and lookup of a particular metadata pointer, 
please note that to add or remove nodes from the AVL tree requires exclusive VM access, 
which means threads with VM access have to be stopped before the operation 
can be carried out. However, to add an entry to a hash table or to look up a metadata 
pointer given a PC address, only VM access is required, which means the operation can be 
carried out concurrently with other threads holding VM access. Removing a metadata 
pointer from a hash table in [Eclipse OpenJ9](https://github.com/eclipse/openj9) is 
done at the end of garbage collection when a method is unloaded or recompiled and 
reclaimed, and exclusive VM access is already obtained.

The following is the procedure for adding a metadata pointer with the start 
and end PC addresses of its compiled code:

1. Find out which code cache the PC address range belongs to, i.e. which node in 
the AVL tree contains the PC address range;
2. Calculate which chunks of the code cache correspond to the PC address 
range, i.e. what entries in the hash table will be updated;
3. Add the metadata pointer to each hash table entry found in step #2. If 
the hash table entry has one metadata pointer, a list is created and 
prepended with the new one and the existing metadata pointer (with its
low bit set) will terminate the list. If the entry is already a list of metadata 
pointers, either add the new metadata to the list if there is enough space 
or allocate space to copy the existing list and then add the new metadata pointer.


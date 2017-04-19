(c) Copyright IBM Corp. 2016

 This program and the accompanying materials are made available
 under the terms of the Eclipse Public License v1.0 and
 Apache License v2.0 which accompanies this distribution.

     The Eclipse Public License is available at
     http://www.eclipse.org/legal/epl-v10.html

     The Apache License v2.0 is available at
     http://www.opensource.org/licenses/apache2.0.php

Contributors:
   Multiple authors (IBM Corp.) - initial implementation and documentation

DDR: The Data Dump Reader
=========================

Building DDR
------------
On Linux, DDR depends on libdwarf and libelf.  For more instructions on
obtaining these packages, see INSTALL.

OMR must be configured to enable the DDR component. As of writing, this is not
the default. DDR will be built after the main files are built.

```sh
./configure --enable-debug --enable-DDR CPPFLAGS=-I/usr/include/libdwarf
make
```

Running the DDR tests and samples
---------------------------------
How to run new ddrgen on the test samples:

1. Run the macros script from the top directory:
```tools/ddrgen/src/macros/getMacros.sh "tools/ddrgen/test"```
2. Run ddrgen with the files to scan and macro list as parameters
```./ddrgen ./ddrgentest --macrolist test/macroList```
3. Print the generated blob
```./blob_reader blob.dat > blob.dat.strings```
4. Compare the output to the expected output. For linux x86 (Note that ubuntu has slightly different results):
```diff superset.out tools/ddrgen/test/expected_output/xa64```
```diff tools/ddrgen/test/expected_output/xa64/blob.dat.strings```

Using DDR for OMR
-----------------
Make sure that OMR was configured to output debug information (Enabled by
default, or pass --enable-debug as above)

```sh
find -name "*.dbg" > filelist
make -f ddr_artifacts.mk TOP_SRCDIR=. DBG_FILE_LIST=./filelist
```

Using DDR for other projects
----------------------------
The project must be compiled with debug symbols.  You must populate a file with
a list of all files with debug information.  If you are building a single
executable, then that is all you have to list in that file.  If you build any
other artifacts, such as libraries, you will need to add them as well.

You need to call a special makefile, which will run ddrgen on your project.

```bash
make -f omr/ddr_artifacts.mk TOP_SRCDIR=. DBG_FILE_LIST=./<filelist>
```


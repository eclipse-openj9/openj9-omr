<!--
Copyright IBM Corp. and others 2018

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
[2] https://openjdk.org/legal/assembly-exception.html

SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0-only WITH Classpath-exception-2.0 OR GPL-2.0-only WITH OpenJDK-assembly-exception-1.0
-->

# RSSReport

**RSSReport** is a singleton class that prints resident set sizes (RSS) for an arbitrary number of memory regions.

# RSSRegion

**RSSRegion** represents a memory region for which RSS data will be printed. **RSSRegion** can be added to **RSSReport** using the `addRegion()` method. **RSSRegion** has starting address, size, and name. The size of the region can be updated after it was created. A region can grow from low to high or from high to low addresses, which can be specified in its constructor.

# RSSItem

**RSSItem** is a way to describe some part of **RSSRegion**. When **RSSItem** is added to **RSSRegion** and it overlaps with an existing item the latter will be removed. When a detailed RSS report is printed, it detects if there are gaps between the items. It is also possible to attach debug counters to **RSSItem**. This might be useful if you would like to find out why some page is in RSS.

# Options
`-Xjit:verbose={RSSReport|RSSReportDetailed}`

# Example
For example, `-Xjit:verbose={RSSReport}` may print:

```
#PERF:  RSS Region name:             cold code         cold code
#PERF:  RSS Region start:          0x7f90895ff1a0    0x7f90897ff1a0
#PERF:  RSS Region rss/size:       580/  591( 98%)   360/  356(101%)  all 2 regions   940/  947( 99%) KB
```

`-Xjit:verbose={RSSReportDetailed}` may print:

```
#PERF:  RSS item at addr=00007F1A6B969FC0 size=16 itemDebugCount=0 HEADER
#PERF:  RSS item at addr=00007F1A6B969FD0 size=15 itemDebugCount=0 COLD BLOCKS
#PERF:  RSS item at addr=00007F1A6B969FDF size=24 itemDebugCount=0 OVERESTIMATE
#PERF:  RSS item at addr=00007F1A6B969FF7 size=9 itemDebugCount=0 ALIGNMENT
#PERF:  RSS: Page at addr 00007F1A6B969FC0 has 64 bytes of 4 items pageDebugCount=0
```
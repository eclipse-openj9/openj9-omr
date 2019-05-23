<!--
Copyright (c) 2016, 2019 IBM Corp. and others

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

# Contributing to Eclipse OMR

Thanks for your interest in this project.

We welcome and encourage all kinds of contribution to the project, not only code.
This includes bug reports, user experience feedback, assistance in reproducing
issues and more.

## Issues

This project uses GitHub Issues to track ongoing development, discuss project
plans, and keep track of bugs.  Be sure to search for existing issues before
you create another one.

Visit [our Issues page on GitHub to search and submit](https://github.com/eclipse/omr/issues)

### Labelling

Our GitHub issues get labelled by committers in order to allow easier navigation,
and to flag certain issues as being of interest to certain groups. A PR or Issue
may have multiple labels, as many as needed to provide adequate categorization.

A subset of the labels are documented below.

* [**`beginner`**](https://github.com/eclipse/omr/labels/beginner) generally
  refers to a task that would be suitable for someone new to the project with
  little experience with either the technology or even open-source projects and
  processes. They are intended for anyone who wants to gently get their feet
  wet on building, testing, or familiarizing themselves with part of the code
  base.

* [**`bug`**](https://github.com/eclipse/omr/labels/bug) issues are functional
  problems, errors or unexpected behaviour.

* [**`build/configure`**](https://github.com/eclipse/omr/labels/build%20%2F%20configure)
  labels are used to describe issues with the build and configure system (e.g.,
  makefiles, autotools configure).

* [**`ci`**](https://github.com/eclipse/omr/labels/ci) labels are used for
  issues and enhancements with the continuous integration system for pull
  request testing (e.g., Jenkins, Travis, AppVeyor, etc.)

* [**`cmake`**](https://github.com/eclipse/omr/labels/cmake) labels are similar
  to build/configure but they apply specifically to the Cmake configuration.

* [**`compiler arch review`**](https://github.com/eclipse/omr/labels/compiler%20arch%20review)
  labels are used to indicate a review of this issue or pull request at the
  [OMR Compiler Architecture meeting](https://github.com/eclipse/omr/issues/2316)
  is requested prior to committing.

* [**`documentation`**](https://github.com/eclipse/omr/labels/documentation)
  labels are used for issues or enhancements to the documentation (either in
  the source code itself or stand-alone documentation files).

* [**`epic`**](https://github.com/eclipse/omr/labels/epic) issues are used to
  group together related issues and to track larger goals in the project across
  issues.

* [**`GSoC Project`**](https://github.com/eclipse/omr/labels/GSoC%20project)
  labels are for potential ideas for Google Summer Of Code projects.

* [**`help wanted`**](https://github.com/eclipse/omr/labels/help%20wanted)
  issues have value to the project but no immediate human resources to
  undertake it. Those that are looking to complete a task that someone isn't
  already working on can consider these.

* [**`license`**](https://github.com/eclipse/omr/labels/license) labels are
  used to annotate issues concerning the source code license.

* [**`meeting`**](https://github.com/eclipse/omr/labels/meeting) labels are
  used to annotate issues pertaining to project meeting agendas or minutes.

* [**`tooling`**](https://github.com/eclipse/omr/labels/tooling) labels are for
  issues concerning support tooling needed to support any of the code or
  processes within the project.

Labels are also used to classify issues and pull requests by the dominant
Eclipse OMR technology component they affect.  For instance,

| Label | Component | Principal Directories |
| :---- | :---- | :---- |
[**`comp:compiler`**](https://github.com/eclipse/omr/labels/comp%3Acompiler) | Compiler | `compiler`
[**`comp:core`**](https://github.com/eclipse/omr/labels/comp%3Acore) | Core OMR functionality | `include_code`, `omr`
[**`comp:diagnostic`**](https://github.com/eclipse/omr/labels/comp%3Adiagnostic) | Diagnostic services | `ddr`
[**`comp:doc`**](https://github.com/eclipse/omr/labels/comp%3Adoc) | OMR documentation | `doc`
[**`comp:gc`**](https://github.com/eclipse/omr/labels/comp%3Agc) | Garbage collector | `gc`
[**`comp:glue`**](https://github.com/eclipse/omr/labels/comp%3Aglue) | Glue code | `glue`
[**`comp:jitbuilder`**](https://github.com/eclipse/omr/labels/comp%3Ajitbuilder) | JitBuilder | `jitbuilder`
[**`comp:port`**](https://github.com/eclipse/omr/labels/comp%3Aport) | Port library | `port`
[**`comp:test`**](https://github.com/eclipse/omr/labels/comp%3Atest) | Unit tests and testing framework | `fvtest`
[**`comp:thread`**](https://github.com/eclipse/omr/labels/comp%3Athread) | Thread library | `thread`
[**`comp:tril`**](https://github.com/eclipse/omr/labels/comp%3Atril) | Tril infrastructure and tests | `fvtest/tril`
[**`comp:utilities`**](https://github.com/eclipse/omr/labels/comp%3Autil) | OMR utilities | `util`

Further classification by processor architecture, operating system, and bitness
can be achieved with the following labels:

* [**`arch:aarch32`**](https://github.com/eclipse/omr/labels/arch%3Aaarch32)
* [**`arch:aarch64`**](https://github.com/eclipse/omr/labels/arch%3Aaarch64)
* [**`arch:power`**](https://github.com/eclipse/omr/labels/arch%3Apower)
* [**`arch:x86`**](https://github.com/eclipse/omr/labels/arch%3Ax86)
* [**`arch:z`**](https://github.com/eclipse/omr/labels/arch%3Az)
<br/>

* [**`os:aix`**](https://github.com/eclipse/omr/labels/os%3Aaix)
* [**`os:linux`**](https://github.com/eclipse/omr/labels/os%3Alinux)
* [**`os:macos`**](https://github.com/eclipse/omr/labels/os%3Amacos)
* [**`os:windows`**](https://github.com/eclipse/omr/labels/os%3Awindows)
* [**`os:zos`**](https://github.com/eclipse/omr/labels/os%3Azos)
<br/>

* [**`bits:32`**](https://github.com/eclipse/omr/labels/bits%3A32)
* [**`bits:64`**](https://github.com/eclipse/omr/labels/bits%3A64)

## Submitting a contribution

You can propose contributions by sending pull requests through GitHub.
Following these guidelines will help us to merge your pull requests smoothly:

1. If you're not sure your contribution would be accepted, and want to validate
   your approach or idea before writing code, feel free to open an issue. However,
   not every feature or fix needs an issue. If the problem and fix are cleanly
   connected, and you have the fix in hand, feel free to just submit a pull request.

2. Your pull request is an opportunity to explain both what changes you'd like
   pulled in, but also _why_ you'd like them added. Providing clarity on why
   you want changes makes it easier to accept, and provides valuable context to
   review.

3. Please read carefully and adhere to the legal considerations and
   copyright/license requirements outlined below.

4. For each file that you modify, ensure that the current copyright year in the
   header at the top of the file is up-to-date with the current year.  Note that
   renaming, moving, or copying a file does not require a copyright change.

5. Follow the coding style and format of the code you are modifying (see the
   [coding standards](doc/CodingStandard.md)). The code base is yet to be unified
   in style however, so if the file you are editing seems to have a diffferent
   style, defer to the style of the file as you found it.

6. Follow the commit guidelines found below.

7. We encourage you to open a pull request early, and mark it as "Work In Progress"
   (prefix the PR title with WIP). This allows feedback to start early, and helps
   create a better end product. Committers will wait until after you've removed
   the WIP prefix to merge your changes.

8. If you are contributing a change to the compiler technology that involves modifications
   to the Testarossa IL (including, but not limited to, adding a new IL opcode, changing
   the properties of an opcode, or adding a new datatype) or, in the opinion of a committer,
   a fundamental element of compiler infrastructure, a committer will request that this
   pull request be presented at an upcoming
   [OMR Compiler Architecture meeting](https://github.com/eclipse/omr/issues/2316)
   to invite community discussion prior to merging.  Issues of the same nature may
   also be asked to be discussed before the same architecture meeting prior to
   producing a pull request.

### Downstream Dependencies

If a downstream project consuming OMR has a dependency on the contents of your pull
request, i.e. the pull request should not be merged until all downstream preparations
have been complete, you as the contributor of the pull request may use the WIP prefix in
addition to comment in the said pull request to inform the committer of your
particular circumstances for merging. That is, you may request a review of the pull
request even though it's marked WIP, informing the committer of your desire to
coordinate when the PR is merged. Once all downstream preparations have been complete
it is your responsibility as the contributor to remove the WIP prefix and inform the
committers that the pull request is ready for merging, pending reviews have been
completed.

## Commit Guidelines

The first line describes the change made. It is written in the imperative mood,
and should say what happens when the patch is applied. Keep it short and
simple. The first line should be less than 70 characters, where reasonable,
and should be written in sentence case preferably not ending in a period.
Leave a blank line between the first line and the message body.

The body should be wrapped at 72 characters, where reasonable.

Include as much information in your commit as possible. You may want to include
designs and rationale, examples and code, or issues and next steps. Prefer
copying resources into the body of the commit over providing external links.
Structure large commit messages with headers, references etc. Remember however
that the commit message is always going to be rendered in plain text.

Please add `[skip ci]` to the commit message when the change doesn't require a
compilation, such as documentation only changes, to avoid unnecessarily wasting
the project's build resources.

Use the commit footer to place commit metadata. The footer is the last block of
contiguous text in the message. It is separated from the body by one or more
blank lines, and as such cannot contain any blank lines. Lines in the footer are
of the form:

```
Key: Value
```

When a commit has related issues or commits, explain the relation in the message
body. You should also leave an `Issue` tag in the footer. However, if this is the
final commit that fixes an issue you should leave a `Closes` tag (or one of the
tags described [here](https://help.github.com/articles/closing-issues-using-keywords/)
in the footer instead which will automatically close the referenced issue when
this pull request is merged.

For example, if this is just one of the commits necessary to address Issue 1234
then the following is a valid commit message:

```
Correct race in frobnicator

This patch eliminates the race condition in issue #1234.

Issue: #1234
```

However, if this is the final commit that addresses the issue then the following
is an acceptable commit message:

```
Correct race in frobnicator

This patch eliminates the race condition in issue #1234.

Closes: #1234
```

Sign off on your commit in the footer. By doing this, you assert original
authorship of the commit and that you are permitted to contribute it. This can
be automatically added to your commit by passing `-s` to `git commit`, or by
hand adding the following line to the footer of the commit.

```
Signed-off-by: Full Name <email>
```

Remember, if a blank line is found anywhere after the `Signed-off-by` line, the
`Signed-off-by:` will be considered outside of the footer, and will fail the
automated Signed-off-by validation.

It is important that you read and understand the legal considerations found
below when signing off or contributing any commit.

### Example commits

Here is an example of a *good* commit:

```
Update and expand the commit guidelines

Elaborate on the style guidelines for commit messages. These new
style guidelines reflect the conversation found in #124.

The guidelines are changed to:
- Provide guidance on how to write a good first line.
- Elaborate on formatting requirements.
- Relax the advice on using issues for nontrivial commits.
- Move issue references from the first line to the message footer.
- Encourage contributors to put more information into the commit
  message.

Issue: #124
Signed-off-by: Robert Young <rwy0717@gmail.com>
```

The first line is meaningful and imperative. The body contains enough
information that the reader understands the why and how of the commit, and its
relation to any issues. The issue is properly tagged and the commit is signed
off.

The following is a *bad* commit:

```
FIX #124: Changing a couple random things in CONTRIBUTING.md.
Also, there are some bug fixes in the thread library.
```

The commit rolls unrelated changes together in a very bad way. There is not
enough information for the commit message to be useful. The first line is not
meaningful or imperative. The message is not formatted correctly, the issue is
improperly referenced, and the commit is not signed off by the author.

### Other resources for writing good commits

- http://chris.beams.io/posts/git-commit/

### Install a git commit message hook script to validate local commits

We have a script that attempts to enforce some of these commit guidelines. You
can install it by following instructions similar to the ones below.

```
cd omr_repo_dir
cp scripts/commit-msg .git/hooks
chmod +x .git/hooks/commit-msg
```

If the hook declines your commit, the message will remain in
`omr_repo_dir/.git/COMMIT_EDITMSG`.

Be sure to update your version of the script occasionally as it may evolve as
our commit guidelines evolve.

## Legal considerations

Please read the [Eclipse Foundation policy on accepting contributions via Git](http://wiki.eclipse.org/Development_Resources/Contributing_via_Git).

Your contribution cannot be accepted unless you have a signed [ECA - Eclipse Foundation Contributor Agreement](http://www.eclipse.org/legal/ECA.php) in place. If you have an active signed Eclipse CLA
([the CLA was updated by the Eclipse Foundation to become the ECA in August 2016](https://mmilinkov.wordpress.com/2016/08/15/contributor-agreement-update/)),
then that signed CLA is sufficient. You will have to sign the ECA once your CLA expires.

Here is the checklist for contributions to be _acceptable_:

1. [Create an account at Eclipse](https://dev.eclipse.org/site_login/createaccount.php).
2. Add your GitHub user name in your account settings.
3. [Log into the project's portal](https://projects.eclipse.org/) and sign the ["Eclipse ECA"](https://projects.eclipse.org/user/sign/cla).
4. Ensure that you [_sign-off_](https://wiki.eclipse.org/Development_Resources/Contributing_via_Git#Signing_off_on_a_commit) your Git commits.
5. Ensure that you use the _same_ email address as your Eclipse account in commits.
6. Include the appropriate copyright notice and license at the top of each file.

Your signing of the ECA will be verified by a webservice called 'ip-validation'
that checks the email address that signed-off on your commits has signed the
ECA. **Note**: This service is case-sensitive, so ensure the email that signed
the ECA and that signed-off on your commits is the same, down to the case.

### Copyright Notice and Licensing Requirements

**It is the responsibility of each contributor to obtain legal advice, and
to ensure that their contributions fulfill the legal requirements of their
organization. This document is not legal advice.**

OMR is dual-licensed under the Eclipse Public License 2.0 and the Apache
License v2.0. Any previously unlicensed contribution should be released under
the same license.

* If you wish to contribute code under a different license, you must consult
with a committer before contributing.
* For any scenario not covered by this document, please discuss the copyright
notice and licensing requirements with a committer before contributing.

The template for the copyright notice and dual-license is as follows:

```c
/*******************************************************************************
 *  Copyright (c) 2017, 2017 ${author} and others
 *
 *  This program and the accompanying materials are made available under
 *  the terms of the Eclipse Public License 2.0 which accompanies this
 *  distribution and is available at https://www.eclipse.org/legal/epl-2.0/
 *  or the Apache License, Version 2.0 which accompanies this distribution and
 *  is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 *  This Source Code may also be made available under the following
 *  Secondary Licenses when the conditions for such availability set
 *  forth in the Eclipse Public License, v. 2.0 are satisfied: GNU
 *  General Public License, version 2 with the GNU Classpath
 *  Exception [1] and GNU General Public License, version 2 with the
 *  OpenJDK Assembly Exception [2].
 *
 *  [1] https://www.gnu.org/software/classpath/license.html
 *  [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 *  SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/
```

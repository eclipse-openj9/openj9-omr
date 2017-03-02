# Contributing to Eclipse OMR
Thanks for your interest in this project.

## Project description
The Eclipse OMR project consists of a highly integrated set of open source C
and C++ components that can be used to build robust language runtimes that will
support many different hardware and operating system platforms. These components
include but are not limited to: memory management, threading, platform port
(abstraction) library, diagnostic file support, monitoring support,
garbage collection, and native Just In Time compilation. The long term goal for 
the Eclipse OMR project is to foster an open ecosystem of language runtime
developers to collaborate and collectively innovate with hardware platform
designers, operating system developers, as well as tool and framework developers
and to provide a robust runtime technology platform so that language
implementers can much more quickly and easily create more fully featured
languages to enrich the options available to programmers.

- [https://projects.eclipse.org/projects/technology.omr](https://projects.eclipse.org/projects/technology.omr)


## GitHub Issues

This project uses GitHub Issues to track ongoing development and issues. Be sure
to search for existing bugs before you create another one. Contributions are always welcome!

- [https://github.com/eclipse/omr/issues](https://github.com/eclipse/omr/issues)

## Submitting a contribution
You can propose contributions by sending pull requests through GitHub. Following these guidelines
will help us to merge your pull requests smoothly:

1. It is generally a good idea to file an issue to explain your idea before
   writing code or submitting a pull request.
2. Please read carefully and adhere to the legal considerations and
   copyright/license requirements outlined below.
3. Follow the coding style and format of the code you are modifying (see the
   [coding standards](doc/CodingStandard.md)).
4. Follow the commit guidelines found below.
5. Ensure that `make test` passes all tests and `make lint` passes before you
   submit a Pull Request.

## Commit Guidelines

The first line describes the change made. It is written in the imperative mood,
say what happens when the patch is applied. Keep it short and simple. The first
line should be less than 50 characters, sentence case, and does not end in a
period. Leave a blank line between the first line and the message body.

The body should be wrapped at 72 characters, where reasonable. Write your commit in
[github style markdown](https://guides.github.com/features/mastering-markdown/).
Use headers and code snippets where it makes sense.

Include as much information in your commit as possible. You may want to include
designs and rationale, examples and code, or issues and next steps. Prefer
copying resources into the body of the commit over providing external links.
Structure large commit messages with markdown headers.

Use the commit footer to place commit metadata. The footer is the last block of
contiguous text in the message. It is separated from the body by one or more
blank lines, and as such cannot contain any blank lines. Lines in the footer are
of the form:

```
Key: Value
```

When a commit has related issues or commits, explain the relation in the message
body. You should also leave an `Issue` tag in the footer. For example:

```
This patch eliminates the race condition in issue #1234.

Issue: #1234
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

### Copyright Notice and Licensing Requirements

**It is the responsibility of each contributor to obtain legal advice, and
to ensure that their contributions fulfill the legal requirements of their
organization. This document is not legal advice.**

OMR is dual-licensed under the Eclipse Public License v1.0 and the Apache
License v2.0. Any previously unlicensed contribution should be released under
the same license.

* If you wish to contribute code under a different license, you must consult
with a committer before contributing.
* For any scenario not covered by this document, please discuss the copyright
notice and licensing requirements with a committer before contributing.

The template for the copyright notice and dual-license is as follows:
```c
/*******************************************************************************
 *
 * <Insert appropriate copyright notice here>
 *
 *  This program and the accompanying materials are made available
 *  under the terms of the Eclipse Public License v1.0 and
 *  Apache License v2.0 which accompanies this distribution.
 *
 *      The Eclipse Public License is available at
 *      http://www.eclipse.org/legal/epl-v10.html
 *
 *      The Apache License v2.0 is available at
 *      http://www.opensource.org/licenses/apache2.0.php
 *
 * Contributors:
 *    <First author> - initial implementation and documentation
 *******************************************************************************/
```

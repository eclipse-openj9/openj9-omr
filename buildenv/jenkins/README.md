<!--
Copyright (c) 2019, 2019 IBM Corp. and others

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

[Eclipse OMR Jenkins Builds](https://ci.eclipse.org/omr/)

This folder contains Jenkins pipeline scripts that are used in the OMR Jenkins builds.

### Pipeline script from SCM subtleties with Rocket Git for z/OS

Configuring Jenkins to pull the pipeline scripts from Git on z/OS is a non-trivial effort. The Rocket Git for z/OS [1] port does not support _https_ transport protocol, thus specifying the repository in the _Repository URL_ with an `https://` prefix will not work as Jenkins will encounter errors when trying to execute commands on the slave:

```
Fetching changes from the remote Git repository
Cleaning workspace
ERROR: Error fetching remote repo 'origin'
hudson.plugins.git.GitException: Failed to fetch from https://github.com/eclipse/omr.git
	at hudson.plugins.git.GitSCM.fetchFrom(GitSCM.java:894)
	at hudson.plugins.git.GitSCM.retrieveChanges(GitSCM.java:1161)
	at hudson.plugins.git.GitSCM.checkout(GitSCM.java:1192)
	at org.jenkinsci.plugins.workflow.steps.scm.SCMStep.checkout(SCMStep.java:120)
	at org.jenkinsci.plugins.workflow.steps.scm.SCMStep$StepExecutionImpl.run(SCMStep.java:90)
	at org.jenkinsci.plugins.workflow.steps.scm.SCMStep$StepExecutionImpl.run(SCMStep.java:77)
	at org.jenkinsci.plugins.workflow.steps.SynchronousNonBlockingStepExecution.lambda$start$0(SynchronousNonBlockingStepExecution.java:47)
	at java.util.concurrent.Executors$RunnableAdapter.call(Executors.java:511)
	at java.util.concurrent.FutureTask.run(FutureTask.java:266)
	at java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1149)
	at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:624)
	at java.lang.Thread.run(Thread.java:748)
Caused by: hudson.plugins.git.GitException: Command "git fetch --tags --progress https://github.com/eclipse/omr.git +refs/heads/*:refs/remotes/origin/*" returned status code 128:
stdout: 
stderr: fatal: Unable to find remote helper for 'https'
```

Thus we must use the `git://` prefix to specify the _Repository URL_ in the Jenkins Git Plugin. This is a supported configuration [2]. However one may run into issues when using the `git://` transport protocol when attempting to launch a Jenkins build on z/OS:

```
hudson.plugins.git.GitException: Command "git fetch --tags --progress origin +refs/heads/master:refs/remotes/origin/master --prune" returned status code 128:
stdout: 
stderr: fatal: unable to connect to github.com:
github.com[0: 192.30.253.113]: errno=Connection timed out


	at org.jenkinsci.plugins.gitclient.CliGitAPIImpl.launchCommandIn(CliGitAPIImpl.java:2042)
	at org.jenkinsci.plugins.gitclient.CliGitAPIImpl.launchCommandWithCredentials(CliGitAPIImpl.java:1761)
	at org.jenkinsci.plugins.gitclient.CliGitAPIImpl.access$400(CliGitAPIImpl.java:72)
	at org.jenkinsci.plugins.gitclient.CliGitAPIImpl$1.execute(CliGitAPIImpl.java:442)
	at jenkins.plugins.git.GitSCMFileSystem$BuilderImpl.build(GitSCMFileSystem.java:351)
	at jenkins.scm.api.SCMFileSystem.of(SCMFileSystem.java:198)
	at jenkins.scm.api.SCMFileSystem.of(SCMFileSystem.java:174)
	at org.jenkinsci.plugins.workflow.cps.CpsScmFlowDefinition.create(CpsScmFlowDefinition.java:108)
	at org.jenkinsci.plugins.workflow.cps.CpsScmFlowDefinition.create(CpsScmFlowDefinition.java:67)
	at org.jenkinsci.plugins.workflow.job.WorkflowRun.run(WorkflowRun.java:293)
	at hudson.model.ResourceController.execute(ResourceController.java:97)
	at hudson.model.Executor.run(Executor.java:429)
Finished: FAILURE
```

The reason this happens is because the native `git://` transport uses TCP port 9418 which may not be open on the Jenkins master, hence why we end up timing out. To fix this there are two options:

1. Open port 9418 on the Jenkins master
2. Alias `git://` transport protocol to `https://` [3]

   This can be achieved by navigating to the master node _Script Console_ in Jenkins and executing the following Groovy script:

   ```
   println "git config --global url.https://.insteadOf git://".execute().text
   ```

[1] https://www.rocketsoftware.com/product-categories/mainframe/git-for-zos
[2] https://wiki.jenkins.io/display/JENKINS/Git+Plugin
[3] https://github.com/angular/angular-phonecat/issues/141#issuecomment-40481120
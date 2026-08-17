<img src="logo/bustub-whiteborder.svg" alt="BusTub Logo" height="200">

-----------------

[![Build Status](https://github.com/bsb20/bustub/actions/workflows/cmake.yml/badge.svg)](https://github.com/bsb20/bustub/actions/workflows/cmake.yml)

This is a fork of BusTub used for COMP 421 at UNC Chapel Hill.

BusTub is a relational database management system built at [Carnegie Mellon University](https://db.cs.cmu.edu) for the [Introduction to Database Systems](https://15445.courses.cs.cmu.edu) (15-445/645) course. This system was developed for educational purposes and should not be used in production environments.

BusTub supports basic SQL and comes with an interactive shell. You can get it running after finishing all the course projects.

<img src="logo/sql.png" alt="BusTub SQL" width="400">

**WARNING: IF YOU ARE A STUDENT IN THE CLASS, DO NOT DIRECTLY FORK THIS REPO. DO NOT PUSH PROJECT SOLUTIONS PUBLICLY. THIS IS AN HONOR CODE VIOLATION, EVEN AFTER THE COURSE HAS ENDED.**


**IF YOU ARE A NOT A STUDENT, PLEASE DO NOT MAKE YOUR SOLUTION PUBLICLY AVAILABLE.** Thank you for creating a fair learning environment.

## Cloning this Repository

The following instructions are adapted from the GitHub documentation on [duplicating a repository](https://docs.github.com/en/github/creating-cloning-and-archiving-repositories/creating-a-repository-on-github/duplicating-a-repository). The procedure below walks you through creating a private BusTub repository that you can use for development.

1. [Create a new repository](https://github.com/new) under your account. Pick a name (e.g. `bustub-private`) and select **Private** for the repository visibility level.
2. On your development machine, create a bare clone of the public BusTub repository:
   ```console
   $ git clone --bare https://github.com/bsb20/bustub.git bustub-public
   ```
3. Next, [mirror](https://git-scm.com/docs/git-push#Documentation/git-push.txt---mirror) the public BusTub repository to your own private BusTub repository. Suppose your GitHub name is `STUDENT` and your repository name is `bustub-private`. The procedure for mirroring the repository is then:
   ```console
   $ cd bustub-public
   
   # If you pull / push over HTTPS
   $ git push https://github.com/STUDENT/bustub-private.git master

   # If you pull / push over SSH
   $ git push git@github.com:STUDENT/bustub-private.git master
   ```
   This copies everything in the public BusTub repository to your own private repository. You can now delete your local clone of the public repository:
   ```console
   $ cd ..
   $ rm -rf bustub-public
   ```
4. Clone your private repository to your development machine:
   ```console
   # If you pull / push over HTTPS
   $ git clone https://github.com/STUDENT/bustub-private.git

   # If you pull / push over SSH
   $ git clone git@github.com:STUDENT/bustub-private.git
   ```
5. Add the public BusTub repository as a second remote. This allows you to retrieve changes from this repository and merge them with your solution throughout the semester:
   ```console
   $ git remote add public https://github.com/bsb20/bustub.git
   ```
   You can verify that the remote was added with the following command:
   ```console
   $ git remote -v
   origin	https://github.com/STUDENT/bustub-private.git (fetch)
   origin	https://github.com/STUDENT/bustub-private.git (push)
   public	https://github.com/bsb20/bustub.git (fetch)
   public	https://github.com/bsb20/bustub.git (push)
   ```
6. You can now pull in changes from the public BusTub repository as needed with:
   ```console
   $ git pull public master
   ```
7. **Disable GitHub Actions** from the project settings of your private repository; otherwise, you may run out of GitHub Actions quota.
   ```
   Settings > Actions > General > Actions permissions > Disable actions.
   ```

We suggest working on your projects in separate branches. If you do not understand how Git branches work, [learn how](https://git-scm.com/book/en/v2/Git-Branching-Basic-Branching-and-Merging). If you fail to do this, you might lose all your work at some point in the semester, and nobody will be able to help you.

## Build

We recommend developing BusTub on Ubuntu 22.04, or using a provided container image of Ubuntu 22.04. We do not officially support any other environments (i.e., do not open issues or come to office hours to debug them). We do not support WSL. You may be able to build the project on other Ubuntu versions, as well as MacOS, but do so at your own risk.  The grading environment runs
Ubuntu 22.04.

While there are many ways to run in a Linux environment these days, we provide two ways for students to do this using Docker.
The first step for both options is to follow the instructions for [setting up Docker](https://docs.docker.com/get-started/get-docker/) on your host machine.

### Option 1: Docker
If you prefer to develop directly in a Linux environment, you can start and attach to a pre-configure container with all necessary packages installed by running:
```./docker_exec.sh```
This script will create and set up a container image for bustub, or attach to a running bustub container if one already exists.

Some MacOS users have reported a `command not found: docker` error when first running this script.  If you see this error, you most likely need to [add docker to your PATH](https://stackoverflow.com/questions/64009138/docker-command-not-found-when-running-on-mac).

### Option 2: VS Code Dev Containers
If you prefer to use VS Code, this repository is set up to integrate with the VS Code Dev Containers extension. In this workflow you clone your personal course repository onto your **host** machine and open that folder inside a container. Because your code lives in a normal clone on the host, your commits, branches, and git credentials stay on the host and your work is never trapped inside (or lost with) the container.

1.  Install the [Dev Containers](https://code.visualstudio.com/docs/devcontainers/containers) extension in VS Code.

2.  Clone your personal course repository if you have not already:

    ```console
    $ git clone <your-personal-repo-url>
    ```

3.  Open VS Code.  From the command palette (<kbd>F1</kbd>), run ***Dev Containers: Open Folder in Container...*** and select the folder you cloned your repo into. Wait for the container to build. The first time, it automatically installs all required packages, so this may take a few minutes. Subsequent launches are fast.

4.  Once the container is running, we must set the build configuration using the **CMake** tab. Click the CMake icon (triangle with a wrench) on the left side bar. Select a *Configure Preset*.  We provide some preset configurations:

    - **Debug** &mdash; debug build with [AddressSanitizer](https://github.com/google/sanitizers) (the default).
    - **Debug (TSan)** &mdash; debug build with ThreadSanitizer, equivalent to `-DBUSTUB_SANITIZER=thread`. Use this for assignments and tests that require the thread sanitizer.
    - **Release** &mdash; optimized build with no sanitizer.
    - **RelWithDebInfo** &mdash; optimized build with debug info.

5.  To build and debug a specific test, click to select a target under the CMake **Launch** or **Debug** options. For example, type the test name you want when prompted.  With a target selected, you can now click Launch/Debug from within **CMake** or at the bottom of the window.

### Linux (Recommended) / macOS (Experimental)

You can now follow these build instructions to run code in your container or on your Ubuntu machine.  The following steps may work on macOS, but are not officially supported.
Your Docker container should have automatically installed all required dependencies at creation time.
However, to reinstall packages or to install the proper packages on an Ubuntu machine, run the following script:

```console
# Linux
$ sudo build_support/packages.sh
# macOS
$ build_support/packages.sh
```

Then run the following commands to build the system:

```console
$ mkdir build
$ cd build
$ cmake ..
$ make
```

If you want to compile the system in debug mode, pass in the following flag to cmake:
Debug mode:

```console
$ cmake -DCMAKE_BUILD_TYPE=Debug ..
$ make -j`nproc`
```
This enables [AddressSanitizer](https://github.com/google/sanitizers) by default.

If you want to use other sanitizers,

```console
$ cmake -DCMAKE_BUILD_TYPE=Debug -DBUSTUB_SANITIZER=thread ..
$ make -j`nproc`
```

***Caution for macOS:*** While these steps may work for native macOS builds, there are some differences between macOS and Linux (i.e., mutex behavior) that might cause test cases
to produce different results on different platforms. We recommend that students use a Linux VM for running
test cases and reproducing errors whenever possible.

## Debugging
Command line users should run test/benchmark binaries directly with an appropriate debugger such as `gdb`

VS Code users should make use of the visual debugger.  To leverage this workflow, install [the clangd extension](https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd).  To debug the project tests, you may find it helpful to use the example config files `./build_support/launch.json` and `./build_support/tasks.json` by dragging them into your `.vscode` directory once it is generated.

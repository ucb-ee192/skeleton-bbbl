# skeleton-bbbl

## Building
This repository is set up to build under either Eclipse IDE with cross-compilation, or make on the target device.

Important: both are distinct build flows, and likely will not produce equivalent binaries because of possible differences in compiler versions and flags. We recommend you standardize and stick to one.

### Eclipse IDE cross-compiling
See the setup instructions [here](https://docs.google.com/document/d/1s1otjflh8VYQb7MRY7I221TIImY-fodieaWao4NJLk0/edit#) to configure your BeagleBoneBlue and host machine.
**Despite the higher initial setup complexity, we recommend this option because of the superior code navigation, interactive static analysis, and debugging capabilities provided by an IDE.**

You can directly import this repository as a project in Eclipse.
However, you will need to update your `arm-linux-gnueabihf-` toolchain path in the project settings.
Otherwise, this repository includes the necessary librobotcontrol library stubs (precompiler shared object and headers) and relevant project configuration for Eclipse to do static code analysis and compile.

You can configure Eclipse to automatically deploy and execute binaries on the target.
Otherwise, compiled binaries are in the `Debug/` folder.

You can symlink `/etc/robotcontrol/link_to_startup_program` to your binary to run your program on boot.

### make on target device
You can run make in the repository root. All `.c` and `.cpp` files in `src/` will be automatically detected for compilation, and create a binary called `build/main`. 

The make target `runonboot` is provided to run the compiled target on the BeagleBoneBlue's boot.


## Updating from skeleton-bbbl
This repository may be updated once in a while.
There are two ways to pull these into your team repository:

### git pull
On the command line, add the `skeleton-bbbl` repository as a remote (this only needs to be done once per cloned repository / local copy):
```
git remote add skeleton https://github.com/ucb-ee192/skeleton-bbbl.git
```

To update from `skeleton-bbbl`, run:
```
git pull skeleton master
```

You may need to fix merge conflicts.

### manual copy-paste
On each release, we will post a summary of changed files.
You can copy-paste these into your repository.

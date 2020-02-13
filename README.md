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

If you configured a remote debug target per the setup instructions above, Eclipse will copy the compiled binary onto the BeagleBoneBlue every time you run a debug session.
In the example configuration, this would be `/MyBinary`.
Compiled binaries are on your local machine in the `Debug/` folder in the repository root.
Build artifacts, including the `Debug/` folder, are ignored by git.

You can symlink `/etc/robotcontrol/link_to_startup_program` to your binary to run your program on boot.
If your "Remote Absolute File Path for C/C++ Application" in your debug configuration is `/MyBinary`, for example, you would run this command on the BeagleBoneBlue to have it launch by default:
```
ln -s -f /MyBinary /etc/robotcontrol/link_to_startup_program
```
Note that since this is a symlink (as opposed to a copy), it will always point to the latest version of the binary, not necessarily the verson as it existed when you ran the symlinking command.

### make on target device
You can run make in the repository root. All `.c` and `.cpp` files in `src/` will be automatically detected for compilation, and create a binary called `build/main`. 
Build artifacts, including the `build/` folder, are ignored by git.

The make target `runonboot` is provided to run the compiled target on the BeagleBoneBlue's boot.

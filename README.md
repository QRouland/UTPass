# UTPass

A Ubuntu Touch password management app aiming to be compatible with [ZX2C4’s pass command line application](https://www.passwordstore.org/) the standard unix password manager.

# Build & Tests

# Building

* Install [clickable](https://github.com/bhdouglass/clickable).
* Clone this repo : ```git clone --recursive  https://github.com/qrouland/UTPass```
* Move to app directory: ```cd UTPass```
* Build third parties : ```clickable third_build```
* Build & Run the app : ```clickable```

For more options/details see the [clickable documentation](http://clickable.bhdouglass.com/en/latest/index.html)       

## Custom clickable command
* ```clickable third_build ``` : build third dependencies for arm architecture
* ```clickable third_build_d ``` : build third dependencies for amd64 architecture
* ```clickable third_clean ``` : clean third parties
* ```clickable style ``` : reformat the code (Required [astyle](https://astyle.sourceforge.net) & [qmlfmt](https://github.com/jesperhh/qmlfmt) to be installed)

## Testing 

To switch to the tests build you need to add the following arguments to the build command : -DTEST_RUNNER=ON.

To do so with clickable you need to use the following commands:

* Set CLICKABLE_BUILD_ARGS environnment variable : ```export CLICKABLE_BUILD_ARGS='-DTEST_RUNNER=ON'```
* Build & Run the tests ```clickable ```

To comeback to the standart app build :
* Unset CLICKABLE_BUILD_ARGS environnment variable ```unset CLICKABLE_BUILD_ARGS```

# Contributing

Any contributions are welcome using the github issue & pull request system.
Please respect the code style format by running ```clickable style``` before committing.

# Features

The goal is to be closest possible of the features offer by [ZX2C4’s pass command line application](https://www.passwordstore.org/). 
See to the FEATURES.MD file for details.

# License

    Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.


# Useful Links

Some useful links related to UTpass development :
* [Ubports](https://ubports.com/) : Ubuntu Touch Community
* [ZX2C4’s pass command line application](https://www.passwordstore.org/) : the standard unix password manager.
* [Clickable](https://github.com/bhdouglass/clickable) : Compile, build, and deploy Ubuntu Touch click packages
* [Gpgme](https://www.gnupg.org/software/gpgme/index.html) : GnuPG Made Easy (GPGME) is a library designed to make access to GnuPG easier for applications

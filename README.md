# UTPass

A Ubuntu Touch password management app aiming to be compatible with [ZX2C4’s pass command line application](https://www.passwordstore.org/) the standard unix password manager.

## Installation

UTPass is avalaible on the [OpenStore](open-store.io)

[![OpenStore](https://open-store.io/badges/en_US.png)](https://open-store.io/app/utpass.qrouland)


## Features

The goal is to be closest possible of the features offer by [ZX2C4’s pass command line application](https://www.passwordstore.org/). 
See [Features wiki page](https://taiga.rdrive.ovh/project/utpass/wiki/contributing) for details.

## Export/Import

Assuming that there are already passwords in another device using [ZX2C4’s pass command line application](https://www.passwordstore.org/) and, therefore, that [gpg keys](https://gnupg.org/) have been previously generated for encryption purposes, these may be helpful commands:

Export gpg private keys in order to decrypt passwords:
```
gpg --output keys.gpg --export-secret-keys <key>
```

If your password store is already hosted in a Git repository that provides HTTP or HTTP with authentication for cloning (we're working to have support for SSH soon), you can clone your password store directly from the app.
Otherwise, follow these steps to export it to a ZIP file for importing.

Export passwords to a ZIP archive, assuming they reside in the *.password-store* folder:
```
zip passwords.zip -r .password-store/
```

Both files have the correct format for UTPass to import them and work as intended. It is highly recommended to remove them after imported to **UTPass**.


## Build & Tests

See [Build & Tests wiki page](https://github.com/QRouland/UTPass/wiki/Build-&-Tests)

## Contributing & Issues

See [Contributing wiki page](https://github.com/QRouland/UTPass/wiki/Contributing)


## Useful Links

Some useful links related to UTPass development :
* [Ubports](https://ubports.com/) : Ubuntu Touch Community
* [ZX2C4’s pass command line application](https://www.passwordstore.org/) : the standard unix password manager.
* [Clickable](https://github.com/bhdouglass/clickable) : Compile, build, and deploy Ubuntu Touch click packages
* [GnuPG](https://gnupg.org/): The GNU Privacy Guard
* [Gpgme](https://www.gnupg.org/software/gpgme/index.html) : GnuPG Made Easy (GPGME) is a library designed to make access to GnuPG easier for applications


## License

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

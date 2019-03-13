# UTPass

A password management app for Ubuntu Touch aiming to be compatible with [ZX2C4’s pass command line application](https://www.passwordstore.org/) the standard unix password manager.


# Building

* Install [clickable](https://github.com/bhdouglass/clickable).
* Clone this repo : ```git --recursive clone https://github.com/qrouland/UTPass```
* Build the app : ```cd UTPass && clickable```

For more options/details see the [clickable documentation](http://clickable.bhdouglass.com/en/latest/index.html)       

# Contributing

Any contributions are welcome using the github issue & pull request system.

# Features

The goal is to be closest possible of the features offer by [ZX2C4’s pass command line application](https://www.passwordstore.org/):

---
- [ ] pass init [--path=subfolder,-p subfolder] gpg-id...
        Initialize new password storage and use gpg-id for encryption.
        Selectively reencrypt existing passwords using new gpg-id.
    - [ ] Interactive dialog to init a new password storage
    - [ ] Support subfolder
    - [ ] Support reencrypt
---      
- [x] pass [ls] [subfolder]
        List passwords.
    - [x] UI allowing to navigate through the password store showing the available passwords
---
- [ ] pass find pass-names... 
List passwords that match pass-names
   - [ ] Search bar allowing searchs by pass-names
---
- [ ] pass [show] [--clip[=line-number],-c[line-number]] pass-name
        Show existing password and optionally put it on the clipboard.
        If put on the clipboard, it will be cleared in 45 seconds.
     - [ ] Click on a pass show the password
     - [ ] Add option to put the password to the clipboard
     - [ ] Clearing clipboard after 45 secs
     - [ ] Line number option ???
---
- [ ] pass grep search-string
        Search for password files containing search-string when decrypted.
     - [ ] TBD
---
- [ ] pass insert [--echo,-e | --multiline,-m] [--force,-f] pass-name
        Insert new password. Optionally, echo the password back to the console
        during entry. Or, optionally, the entry may be multiline. Prompt before
        overwriting existing password unless forced.
     - [ ] TBD
---
- [ ] pass edit pass-name
        Insert a new password or edit an existing password using editor.
     - [ ] TBD
---
- [ ] pass generate [--no-symbols,-n] [--clip,-c] [--in-place,-i | --force,-f] pass-name [pass-length]
        Generate a new password of pass-length (or 25 if unspecified) with optionally no symbols.
        Optionally put it on the clipboard and clear board after 45 seconds.
        Prompt before overwriting existing password unless forced.
        Optionally replace only the first line of an existing file with a new password.
     - [ ] TBD
---
- [ ] pass rm [--recursive,-r] [--force,-f] pass-name
        Remove existing password or directory, optionally forcefully.
     - [ ] TBD
---
- [ ] pass mv [--force,-f] old-path new-path
        Renames or moves old-path to new-path, optionally forcefully, selectively reencrypting.
     - [ ] TBD
---
- [ ] pass cp [--force,-f] old-path new-path
        Copies old-path to new-path, optionally forcefully, selectively reencrypting.
        
     - [ ] TBD
---
- [ ] pass git git-command-args...
        If the password store is a git repository, execute a git command
        specified by git-command-args.
     - [ ] TBD
---
- [ ] pass help
        Show this text.
        
     - [ ] TBD
---
- [ ] pass version
        Show version information.
     - [ ] TBD
---

# License

    Copyright (C) 2018  Quentin Rouland

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


# Acknowledge
* Ubports
* pass
* clikable
* gpgmepp & gpgme & gpg  

[![Build status](https://ci.appveyor.com/api/projects/status/v44w7opycn8o56w7?svg=true)](https://ci.appveyor.com/project/drako0812/drak-0)

# DRAK-0
A fantasy console written in C++ using SFML and using ChaiScript for scripting.

## Community Links
- [Google Group Forum](https://groups.google.com/forum/#!forum/drak-0)
- [Google+ Community](https://plus.google.com/communities/104848103715411845349?sqinv=R2NnUTc1UmNibjRQX1NzeHRrOGZxQW1PTXBGTWln)

## Building (Visual Studio 2017)
*Currently this is the only build option available.*

The repo contains a Solution file "DRAK-0.sln" and the folder "DRAK-0" contains the actual project "DRAK-0.vcxproj" and its
resources.

You should just be able to build and run it. Easy peasy. :)

### Dependencies

#### Vcpkg Dependencies

1. [SFML](github.com/SFML/SFML) I've switched over to using [vcpkg](github.com/Microsoft/vcpkg) for depending on SFML.
   NOTE: To use them yourself you will need to install vcpkg and install SFML using one or all of the following:

   ```bat
   vcpkg install sfml:x86-windows-static
   vcpkg install sfml:x64-windows-static
   ```

#### Other Dependencies

1. [ChaiScript](github.com/ChaiScript/ChaiScript/tree/v6.0.0) this is added into the repo manually from github.

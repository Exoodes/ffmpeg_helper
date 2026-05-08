# How to build
- `conan install . --build=missing -c tools.system.package_manager:mode=install -c tools.system.package_manager:sudo=True`
- `cmake --preset conan-release`
- `cmake --build --preset conan-release`

## Probe things that could be added
- Video
    - language
    - Title
    - is_default
    - is_forced
- Audio
    - language
    - Title
    - is_default
    - is_forced
    - is_commentary
- Media
    - Title
    - encoder
    - creation_time
    - Chapters
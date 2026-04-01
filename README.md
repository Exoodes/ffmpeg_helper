# How to build
- `conan install . --build=missing -c tools.system.package_manager:mode=install -c tools.system.package_manager:sudo=True`
- `cmake --preset conan-release`
- `cmake --build --preset conan-release`
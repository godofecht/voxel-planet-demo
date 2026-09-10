# Voxel Planet

A spherical voxel planet, meshed with OpenVDB and drawn with raylib, wired
together as an ECS.

## How it is put together

| Piece | File |
|---|---|
| Entity component system | `flecs`, set up in `src/main.cpp` |
| Voxel field and surface extraction | `include/voxel_mesher.h`, `src/voxel_mesher.cpp` |
| Planet entity and voxel generation | `include/planet.h`, `src/planet.cpp` |
| Drawing | `include/render_system.h`, `src/render_system.cpp` |
| Flying camera | `include/camera_controller.h` |
| Frustum culling | `include/frustum.h` |

The meshing is the part worth reading. Rather than emitting a cube per voxel,
`VoxelMesher` keeps an `openvdb::FloatGrid` density field and runs
`openvdb::tools::VolumeToMesh` over it at an iso value, so the planet comes out
as a surface rather than as a pile of boxes. `UpdateDensityField` writes a
value into a sphere of the field, which is what makes the terrain editable.

The planet starts at radius 32 with a voxel scale of 0.5, and remeshes whenever
`needsRemesh` is set.

## Controls

| | |
|---|---|
| Right mouse and move | Look around |
| W A S D | Move forward, left, back, right |
| Space | Up |
| Left Shift | Down |

## Build

This is where the repository currently stands: the build files do not match the
sources, and it will not configure as checked in.

`CMakeLists.txt` finds OpenVDB, TBB and Boost, and compiles `src/main.cpp`
alone. The sources also need:

- **raylib**, included by every header
- **flecs**, included by `systems.h` and `main.cpp`
- the other three translation units, `planet.cpp`, `render_system.cpp` and
  `voxel_mesher.cpp`, which are not in the `add_executable` list

Dependencies come from `vcpkg.json` for the OpenVDB side. raylib and flecs are
not declared anywhere.

Fixing the CMake is the next piece of work here, and until it is done nothing in
this README above the build section has been run end to end. It is described
from the source.

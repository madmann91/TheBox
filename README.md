# TheBox

A 2D physics engine in ANSI C.

## Building

    mkdir build
    cd build
    cmake ..
    make -j
    
## Running

    TheBox [simulation file]
    
## Simulation files

Simulation files control the behaviour of the program. The repository contains a simple simulation example in [simulation.txt](simulation.txt).

The syntax is as follows:

- `simulation [nobj] [timestep] [gravity]`: Creates a simulation with `nobj` objects, sets the gravity to `gravity` and the time interval between frames as `timestep`.
- `window [width] [height] [bpp] [scale] [quality]`: Creates a window of size `width`x`height` and `bpp` bits per pixel, sets the view scale to `scale` and rendering quality to `quality` (between 5 and 100).
- `shape [restitution] [friction] box [half width] [half height]`: Creates a box with size 2*`half width`x2*`half height`, and sets its `friction` and `restitution`.
- `shape [restitution] [friction] circle [radius]`: Creates a circle with radius `radius`, and sets its `friction` and `restitution`.
- `object [shape index] [flags] [mass] [x] [y] [angle] ([x speed] [y speed] [angular velocity])`: Creates an object with the shape indexed by `shape index` (starting from 0), the given flags (STATIC=1, SLEEPING=2), `mass`, `position`, and `velocity`.

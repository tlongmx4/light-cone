# light-cone

An interactive 3D visualization of the light cone in special relativity, written in C++ with [raylib](https://www.raylib.com/).

Every event in spacetime falls into one of five categories relative to a chosen origin event, and the program computes that classification from the spacetime interval rather than from anything about how the scene is drawn.

## What it shows

The cone is the path light takes through spacetime from a single event at the origin. Time runs vertically, two spatial dimensions lie in the horizontal plane, and units are chosen so that the speed of light is 1. That choice is what makes the cone open at exactly 45 degrees: light covers one unit of distance in one unit of time.

| Region | Meaning |
|---|---|
| Here-now | The origin event itself |
| Absolute future | Events a particle leaving the origin could reach |
| Absolute past | Events that could have sent a signal to the origin |
| Null cone | The cone surface, where light itself travels |
| Absolute elsewhere | Events with no causal connection to the origin |

"Absolute" is not decoration. Observers moving at different velocities disagree about distances, durations, and even about which of two events happened first, but they all agree on these five categories. That invariance is what the program is built around.

## The math

With `c = 1`, the spacetime interval between two events is

```
s² = -(Δt)² + (Δx)² + (Δy)² + (Δz)²
```

Its sign gives the classification directly:

- `s² > 0` — spacelike separation, no causal link, **elsewhere**
- `s² = 0` — lightlike separation, exactly on the **null cone**
- `s² < 0` — timelike separation, **inside** the cone

The sign of `Δt` then picks future from past. Five categories out of two numbers.

Unlike distance and duration, `s²` is the same in every reference frame. Boost to any velocity and it comes out unchanged, which is why classification can be done once and trusted regardless of how fast an observer is moving.

## Controls

| Input | Action |
|---|---|
| Left click + drag | Orbit |
| Scroll | Zoom |

## Building

Requires CMake and raylib.

```bash
cmake -B build
cmake --build build
./build/light_cone
```

## Verification

The program runs six assertions against `classify` on startup and prints the results before opening a window. Each case targets one branch:

| Event `(t, x, y, z)` | Expected |
|---|---|
| `1, 0, 0, 0` | FutureInterior |
| `1, 1, 0, 0` | FutureCone |
| `1, 5, 0, 0` | Outside |
| `-1, 0, 0, 0` | PastInterior |
| `-1, 1, 0, 0` | PastCone |
| `0, 0, 0, 0` | Unknown |

The `1, 1, 0, 0` case is the one that matters most. It only lands on the cone if the minus sign sits on the time term, so a sign error in the interval shows up there immediately.

## Roadmap

- Lorentz boost, with the axes tilting under velocity while the cone stays fixed
- A velocity slider, with the interval displayed before and after the boost as a live invariance check
- Worldlines through the cone
- Orthographic toggle, so the 45 degree walls can be read off the axes without perspective distortion

## License

MIT. See [LICENSE](LICENSE).
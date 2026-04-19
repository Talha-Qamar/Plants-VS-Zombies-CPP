# Plants vs Zombies (C++)

A desktop game inspired by Plants vs Zombies, developed in C++ using object-oriented programming and SFML.

## Demo

- LinkedIn gameplay video: [Watch the demo](https://www.linkedin.com/posts/talha-qamar-888530356_cplusplus-oop-gamedevelopment-activity-7347705878063529984-qc6k?utm_source=share&utm_medium=member_desktop&rcm=ACoAAFi2E-gBvRXCSpFJVakPKqvG0Qxmho4dK6s)

## Highlights

- Object-oriented architecture (game loop, entities, levels)
- Multiple plant and zombie types
- Real-time gameplay with SFML rendering
- Resource-based UI/assets (fonts, images, audio)

## Tech Stack

- Language: C++
- Graphics/Windowing: SFML
- IDE/Build: Visual Studio 2022 (MSVC v143)
- Project type: `.vcxproj`

## Project Structure

```text
.
|-- Source.cpp
|-- game.h
|-- levels.h
|-- plants.h
|-- zombies.h
|-- sun.h
|-- Header.h
|-- PZ_Project.vcxproj
|-- PZ_Project.vcxproj.filters
|-- PVZ/               # Core game assets (Audio, backgrounds, Fonts, Plants, Zombies)
|-- fonts/
|-- Images/
`-- OOP_PROJECT_SPRING-2024.pdf
```

## Run Locally

### Prerequisites

- Visual Studio 2022 with C++ workload
- SFML libraries available to Visual Studio

### Build Steps

1. Open `PZ_Project.vcxproj` in Visual Studio.
2. Ensure your SFML include and lib paths are configured correctly (the project expects `$(SolutionDir)\\include` and `$(SolutionDir)\\lib`).
3. Select `x64` + `Debug` (or `Release`) configuration.
4. Build and run.

## Recruiter Notes

This project showcases:

- C++ OOP design and modular game architecture
- Real-time game loop and event handling
- Asset integration and desktop game development workflow

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE).

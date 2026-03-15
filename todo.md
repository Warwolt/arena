Goal:
- Get a basic game up and running (Main menu, level, victory, game over)

# TODO
- Merge "Window" and "Screen" structs together, they represent the same thing
- DebugScene: add static collision resolution test screen
- Disable F12 screenshot in Raylib fork
- Integrate ImGui so we can visualize system states
  - The game is low resolution so we probably should use multi viewports (https://github.com/ocornut/imgui/wiki/Multi-Viewports)
- Settings screen, toggle fullscreen on/off
- When loading already loaded resource, return existing ID (needs HashMap)
- Add bounds checking to ArrayMap and SparseArray by adding a `capacity` parameter to the methods
- Bug: Fix hot reloading requiring 2 reloads before change takes effect? (Note: not sure how to repro this yet)

# Doing
- DebugScene: add static collision detection test screen

# Done
- Add semantic key bindings and use those instead of raw keys
- Add viewport relative mouse input
- Move fixed resolution viewport handling into Window struct
- Bug: Win32_toggle_fullscreen breaks on hot reload due to static variables
- DebugScene, add place to run very simple debug scenarios for various systems
- Basic UI system
- Implement a HashMap data structure (map string to value)
- Add death test support to rktest so we can test stuff using DEBUG_ASSERT
- Replace default Raylib font with DOS font
- Add debug screen for doing writing physics code
- DLL based hot reloading
- Pause menu (render as overlay, stop updating game while paused)
- Add for_each macro for nice iteration
- Basic scene handling
- Rename Map to SparseArray
- Render drop shadows
- Add background image and camera following player
- Store position as part of Entity struct
- Add and remove entities and all their components
- Y-Sort entites, player can walk behind coffee
- Implement integer keyed map data structure
- Move pill player character around
- Fix resolution rendering
- Add format check in CI
- Render animated image
- Render image
- Logging
- Integrate Raylib
- Implement Arena allocator

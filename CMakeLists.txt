cmake_minimum_required(VERSION 3.31)
project(XentuEngine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

set(SOURCES
        lib/miniz/miniz.c

        lib/tmxlite/detail/pugixml.cpp
        lib/tmxlite/FreeFuncs.cpp
        lib/tmxlite/ImageLayer.cpp
        lib/tmxlite/LayerGroup.cpp
        lib/tmxlite/Map.cpp
        lib/tmxlite/Object.cpp
        lib/tmxlite/ObjectGroup.cpp
        lib/tmxlite/ObjectTypes.cpp
        lib/tmxlite/Property.cpp
        lib/tmxlite/TileLayer.cpp
        lib/tmxlite/Tileset.cpp

        src/assets/DataManager.cpp
        src/assets/AssetManager.cpp
        src/audio/AudioManager.cpp
        src/input/InputManager.cpp
        src/input/MouseState.cpp
        src/input/GamepadState.cpp
        src/Engine.cpp
        src/Renderer.cpp
        src/graphics/Rect.cpp
        src/graphics/Sprite.cpp
        src/graphics/SpriteMap.cpp
        src/graphics/SpriteMapGroup.cpp
        src/graphics/TileMap.cpp
        src/graphics/TileMapLayer.cpp
        src/graphics/TileMapObject.cpp
        src/graphics/TextBox.cpp
        src/graphics/Texture.cpp
        src/graphics/Transform.cpp
        src/graphics/Transformable.cpp
        src/graphics/Vertex.cpp
        src/graphics/Viewport.cpp
        src/machines/LuaMachineInterop.cpp
        src/machines/LuaMachine.cpp
        src/Globals.cpp
        src/Machine.cpp
        src/Config.cpp
        src/utils/StringUtils.cpp
        main.cpp
)

# Emscripten-specific settings
if(EMSCRIPTEN)
    add_executable(XentuEngine ${SOURCES})
    # Generates .html, .js, and .wasm files instead of just .js
    set(CMAKE_EXECUTABLE_SUFFIX ".js")

    # this is needed so tmxlite and other libs can load.
    target_include_directories(XentuEngine SYSTEM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)

    # Add Emscripten linker flags (e.g., for WASM or memory growth)
    #target_link_options(XentuWasm2 PRIVATE "-sWASM=1" "-sALLOW_MEMORY_GROWTH=1")
    set_target_properties(XentuEngine PROPERTIES COMPILE_FLAGS "-fexceptions -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_LIBPNG -s SDL2_IMAGE_FORMATS=['png'] -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2 -g -gsource-map -Wdeprecated-literal-operator")
    set_target_properties(XentuEngine PROPERTIES LINK_FLAGS "-fexceptions -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_LIBPNG -s SDL2_IMAGE_FORMATS=['png'] -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2 -sASSERTIONS=2 -sSAFE_HEAP=1 -sSTACK_OVERFLOW_CHECK=1 -s FULL_ES2=1 -s WASM=1 -lnodefs.js -lidbfs.js -s INITIAL_MEMORY=64mb -sEXPORTED_RUNTIME_METHODS=['FS','ccall','cwrap'] -o output.{html,js}")

else ()

    list(APPEND CMAKE_PREFIX_PATH
            "${CMAKE_CURRENT_SOURCE_DIR}/includes/SDL2_image"
            "${CMAKE_CURRENT_SOURCE_DIR}/includes/SDL2_mixer"
            "${CMAKE_CURRENT_SOURCE_DIR}/includes/SDL2_ttf"
    )

    SET(GLEW_USE_STATIC_LIBS TRUE)
    SET(GLEW_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/includes/glew-2.3.1/include")
    SET(GLEW_STATIC_LIBRARY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/includes/glew-2.3.1/lib/Release/x64/glew32s.lib")
    find_package(GLEW REQUIRED)
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_mixer REQUIRED)
    find_package(SDL2_ttf REQUIRED)

    add_executable(XentuEngine ${SOURCES})

    # this is needed so tmxlite and other libs can load.
    target_include_directories(XentuEngine SYSTEM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)

    target_compile_definitions(XentuEngine PRIVATE GLEW_STATIC)
    target_link_libraries(XentuEngine PRIVATE
            GLEW::GLEW
            opengl32
            SDL2::SDL2main
            SDL2::SDL2
            SDL2_image::SDL2_image
            SDL2_mixer::SDL2_mixer
            SDL2_ttf::SDL2_ttf
    )

endif()
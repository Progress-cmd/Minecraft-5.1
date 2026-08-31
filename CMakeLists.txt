cmake_minimum_required(VERSION 3.20)
project(Minecraft VERSION 5.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Dépendances vendorisées (compilées depuis les sources, aucune install système requise) ---

# GLFW : on désactive ses propres exemples/tests/docs, on ne veut que la lib
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)

# FreeType : idem, pas de tests/démos embarqués
add_subdirectory(external/freetype)

find_package(OpenGL REQUIRED)

# --- Sources du projet ---
set(SOURCES
    src/Camera.cpp
    src/Chunk.cpp
    src/EBO.cpp
    src/Generation.cpp
    src/Inputs.cpp
    src/main.cpp
    src/perlinNoise.cpp
    src/Polices.cpp
    src/shaderClass.cpp
    src/stb.cpp
    src/Texture.cpp
    src/VAO.cpp
    src/VBO.cpp
    external/glad/src/glad.c
)

add_executable(Minecraft ${SOURCES})

target_compile_definitions(Minecraft PRIVATE GLM_ENABLE_EXPERIMENTAL)

target_include_directories(Minecraft PRIVATE
    include
    external/glad/include
    external/stb
    external/glm
)

target_link_libraries(Minecraft PRIVATE
    glfw
    freetype
    OpenGL::GL
)

# Copie les assets (rangés dans assets/textures, assets/fonts, assets/shaders) à côté
# de l'exécutable après compilation. Le code les charge par leur nom seul (ex: "arial.ttf"),
# donc il cherche dans son répertoire de travail courant : on aplatit tous les fichiers
# directement à côté du binaire, peu importe leur sous-dossier d'origine dans le dépôt.
file(GLOB_RECURSE ASSET_FILES "${CMAKE_CURRENT_SOURCE_DIR}/assets/*")

add_custom_command(TARGET Minecraft POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${ASSET_FILES} "$<TARGET_FILE_DIR:Minecraft>"
)

﻿# Set C++20, mostly for std::format
set(CMAKE_CXX_STANDARD 20)
# This is needed or CMake will expect a .lib file instead of .dll for our shared libs
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Function to compile a folder into a .c
function(create_resources dir output)
    # Create empty output file
    file(WRITE ${output} "")
    # Collect input files
    file(GLOB bins ${dir}/*.*)
    # Iterate through input files
    foreach(bin ${bins})
        # Get short filename
        string(REGEX MATCH "([^/]+)$" filename ${bin})
        # Replace filename spaces & extension separator for C compatibility
        string(REGEX REPLACE "\\.| |-" "_" filename ${filename})
        # Read hex data from file
        file(READ ${bin} filedata HEX)
        # Convert hex data for C compatibility
        string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," filedata ${filedata})
        # Append data to output file
        file(APPEND ${output} "const unsigned char ${filename}[] = {${filedata}0x00};\nconst unsigned ${filename}_size = sizeof(${filename});\n")
    endforeach()
endfunction()


set(INCLUDE_FOLDERS
                    "GameInteractions"
                    "GameInteractions/GameProcess"

                    "GameSpecific"
                    "GameSpecific/Structures"
                    "GameSpecific/Editors"
                    "GameSpecific/EditorVisuals"
                    "GameSpecific/Extractors"
                    "GameSpecific/Importers"
                    "GameSpecific/Online"
                    "GameSpecific/MovesetConverters"
                    "GameSpecific/AnimExtractors"

                    
                    "GameSpecific/EditorVisuals/EditorVisuals_t7"
                    "GameSpecific/Editors/Editor_t7"
                    
                    "Layouts"
                    "Layouts/Submenus"
                    "Layouts/EditorVisuals"

                    "LocalStorage"
                    "ResourcesClasses"
                    "Utils"

                    "Injected"
                    "Injected/SharedMemory"
                    "Injected/MovesetLoaders"

                    "${CMAKE_SOURCE_DIR}/Sig/include"
                    "${CMAKE_SOURCE_DIR}/steamapi sdk/public/steam"
                    "${CMAKE_SOURCE_DIR}/imgui_markdown"
)

# This will be included in the .exe
set(BINARY_SOURCES
                    # Main sources
                    "main/main.cpp"
                    "main/run_gui.cpp"
                    "main/arguments_handling.cpp"

                    "GameInteractions/GameInteraction.cpp"
                    "GameInteractions/GameExtract.cpp"
                    "GameInteractions/GameImport.cpp"
                    "GameInteractions/GameSharedMem.cpp"
                    "GameInteractions/GameProcess/GameData.cpp"
                    "GameInteractions/GameProcess/GameProcess.cpp"  

                    "LocalStorage/LocalStorage.cpp"

                    # Main Layout
                    "Layouts/MainWindow.cpp"
                    "Layouts/NavigationMenu.cpp"
                    "Layouts/SideMenu.cpp"
                    # Layout: submenus
                    "Layouts/Submenus/Submenu_Extract.cpp"
                    "Layouts/Submenus/Submenu_Import.cpp"
                    "Layouts/Submenus/Submenu_Edition.cpp"
                    "Layouts/Submenus/Submenu_OnlinePlay.cpp"
                    "Layouts/Submenus/Submenu_PersistentPlay.cpp"
                    "Layouts/Submenus/Submenu_About.cpp"
                    "Layouts/Submenus/Submenu_Status.cpp"
                    # Layout: editor window
                    "Layouts/EditorVisuals/EditorVisuals.cpp"
                    "Layouts/EditorVisuals/EditorForm.cpp"
                    "Layouts/EditorVisuals/EditorFormList.cpp"

                    # Helpers and such
                    "Utils/Animations.cpp"
                    "Utils/Animations_T_x64.cpp"
                    "Utils/helpers.cpp"
                    "Utils/imgui_extras.cpp"
                    "Utils/ThreadedClass.cpp"
                    "Utils/Compression.cpp"

                    # Resources-accessing
                    "ResourcesClasses/Keybinds.cpp"
                    "ResourcesClasses/Settings.cpp"
                    "ResourcesClasses/Localization.cpp"
                    "ResourcesClasses/EditorLabel.cpp"
                    "ResourcesClasses/GameAddressesFile.cpp"
                    "ResourcesClasses/GameAddressesWrapper.cpp"
)

# Everything game-specific is in here
set(GAMESPECIFIC_SOURCES
                    # List of games
                    "GameSpecific/Games.cpp"
                    "GameSpecific/BaseAddresses.cpp"
                    # Base classes for our game-specific stuff
                    "GameSpecific/Extractors/Extractor.cpp"
                    "GameSpecific/Importers/Importer.cpp"
                    "GameSpecific/Editors/EditorLogic.cpp"
                    "GameSpecific/Online/Online.cpp"
                    "GameSpecific/MovesetConverters/Aliases.cpp"
                    "GameSpecific/MovesetConverters/MovesetFileConverter.cpp"
                    "GameSpecific/AnimExtractors/AnimExtractors.cpp"
                    
                    # Game-specific
                    "GameSpecific/Editors/TEditor.cpp"
                    ##T8
                    "GameSpecific/Online/Online_t8.cpp"
                    ### Editor logic
                    #"GameSpecific/Editors/Editor_t8/Editor_t8.cpp"
                    #"GameSpecific/Editors/Editor_t8/Editor_t8_Forms.cpp"
                    #"GameSpecific/Editors/Editor_t8/Editor_t8_Forms_DisplayableMovelist.cpp"
                    #"GameSpecific/Editors/Editor_t8/Editor_t8_Creations_DisplayableMovelist.cpp"
                    #"GameSpecific/Editors/Editor_t8/Editor_t8_Commands.cpp"
                    #"GameSpecific/Editors/Editor_t8/Editor_t8_Creations.cpp"
                    #"GameSpecific/Editors/Editor_t8/Editor_t8_Deletion.cpp"
                    #"GameSpecific/Editors/Editor_t8/Editor_t8_Duplication.cpp"
                    #"GameSpecific/Editors/Editor_t8/Editor_t8_ListCreations.cpp"
                    #"GameSpecific/Editors/Editor_t8/Editor_t8_LiveEdition.cpp"
                    #"GameSpecific/Editors/Editor_t8/Editor_t8_LiveEdition_Displayable.cpp"
                    #"GameSpecific/Editors/Editor_t8/Editor_t8_Utils.cpp"
                    #"GameSpecific/Editors/Editor_t8/Editor_t8_References.cpp"

                    ## T7
                    "GameSpecific/Online/Online_t7.cpp"
                    "GameSpecific/Extractors/Extractor_t7/Extractor_t7.cpp"
                    "GameSpecific/Importers/Importer_t7/Importer_t7.cpp"
                    "GameSpecific/Importers/Importer_t7/Importer_t7_Utils.cpp"
                    "GameSpecific/Importers/Importer_t7/Importer_t7_OtherGames.cpp"
                    "GameSpecific/AnimExtractors/AnimExtractor_t7.cpp"
                    ### Editor logic
                    "GameSpecific/Editors/Editor_t7/Editor_t7.cpp"
                    "GameSpecific/Editors/Editor_t7/Editor_t7_Forms.cpp"
                    "GameSpecific/Editors/Editor_t7/Editor_t7_Forms_DisplayableMovelist.cpp"
                    "GameSpecific/Editors/Editor_t7/Editor_t7_Creations_DisplayableMovelist.cpp"
                    "GameSpecific/Editors/Editor_t7/Editor_t7_Commands.cpp"
                    "GameSpecific/Editors/Editor_t7/Editor_t7_Creations.cpp"
                    "GameSpecific/Editors/Editor_t7/Editor_t7_Deletion.cpp"
                    "GameSpecific/Editors/Editor_t7/Editor_t7_Duplication.cpp"
                    "GameSpecific/Editors/Editor_t7/Editor_t7_ListCreations.cpp"
                    "GameSpecific/Editors/Editor_t7/Editor_t7_LiveEdition.cpp"
                    "GameSpecific/Editors/Editor_t7/Editor_t7_LiveEdition_Displayable.cpp"
                    "GameSpecific/Editors/Editor_t7/Editor_t7_Utils.cpp"
                    "GameSpecific/Editors/Editor_t7/Editor_t7_References.cpp"
                    ### Editor layout
                    "GameSpecific/EditorVisuals/EditorVisuals_t7/EditorVisuals_t7.cpp"
                    "GameSpecific/EditorVisuals/EditorVisuals_t7/EditorVisuals_t7_Rendering.cpp"
                    "GameSpecific/EditorVisuals/EditorVisuals_t7/Subwindows/TEditor_MoveReferences.cpp"
                    "GameSpecific/EditorVisuals/EditorVisuals_t7/Subwindows/TEditor_CancelReferences.cpp"
                    "GameSpecific/EditorVisuals/EditorVisuals_t7/Structures/TEditorMove.cpp"
                    "GameSpecific/EditorVisuals/EditorVisuals_t7/Structures/TEditorVoiceclip.cpp"
                    "GameSpecific/EditorVisuals/EditorVisuals_t7/Structures/TEditorExtraproperties.cpp"
                    "GameSpecific/EditorVisuals/EditorVisuals_t7/Structures/TEditorCancels.cpp"
                    "GameSpecific/EditorVisuals/EditorVisuals_t7/Structures/TEditorGroupedCancels.cpp"
                    "GameSpecific/EditorVisuals/EditorVisuals_t7/Structures/TEditorRequirements.cpp"
                    "GameSpecific/EditorVisuals/EditorVisuals_t7/Structures/TEditorHitConditions.cpp"
                    "GameSpecific/EditorVisuals/EditorVisuals_t7/Structures/TEditorReactions.cpp"
                    "GameSpecific/EditorVisuals/EditorVisuals_t7/Structures/TEditorPushback.cpp"
                    "GameSpecific/EditorVisuals/EditorVisuals_t7/Structures/TEditorPushbackExtra.cpp"
                    "GameSpecific/EditorVisuals/EditorVisuals_t7/Structures/TEditorMoveStartProperty.cpp"
                    "GameSpecific/EditorVisuals/EditorVisuals_t7/Structures/TEditorMoveEndProperty.cpp"
                    "GameSpecific/EditorVisuals/EditorVisuals_t7/Structures/TEditorInputSequence.cpp"
                    "GameSpecific/EditorVisuals/EditorVisuals_t7/Structures/TEditorInputStruct.cpp"
                    "GameSpecific/EditorVisuals/EditorVisuals_t7/Structures/TEditorProjectile.cpp"
                    "GameSpecific/EditorVisuals/EditorVisuals_t7/Structures/TEditorThrowCamera.cpp"
                    "GameSpecific/EditorVisuals/EditorVisuals_t7/Structures/DisplayableMovelist/TEditorMovelistDisplayable.cpp"
                    "GameSpecific/EditorVisuals/EditorVisuals_t7/Structures/DisplayableMovelist/TEditorMovelistPlayable.cpp"
                    "GameSpecific/EditorVisuals/EditorVisuals_t7/Structures/DisplayableMovelist/TEditorMovelistInput.cpp"
                    "GameSpecific/EditorVisuals/EditorVisuals_t7/Structures/Subwindows/TEditorMove_Animations.cpp"

                    # TTT2
                    "GameSpecific/Extractors/Extractor_ttt2/Extractor_ttt2.cpp"
                    "GameSpecific/MovesetConverters/Converter_ttt2_to_t7.cpp"
                    "GameSpecific/MovesetConverters/Converter_ttt2_to_t7_Aliases.cpp"
                    "GameSpecific/AnimExtractors/AnimExtractor_ttt2.cpp"

                    # TREV
                    "GameSpecific/MovesetConverters/Converter_trev_to_t7_Aliases.cpp"

                    # T6
                    "GameSpecific/Extractors/Extractor_t6/Extractor_t6.cpp"
                    "GameSpecific/MovesetConverters/Converter_t6_to_t7.cpp"
                    "GameSpecific/MovesetConverters/Converter_t6_to_t7_Aliases.cpp"
                    "GameSpecific/AnimExtractors/AnimExtractor_t6.cpp"

                    # T5
                    "GameSpecific/Extractors/Extractor_t5/Extractor_t5.cpp"
                    "GameSpecific/MovesetConverters/Converter_t5_to_t7.cpp"
                    "GameSpecific/MovesetConverters/Converter_t5_to_t7_Aliases.cpp"
                    "GameSpecific/AnimExtractors/AnimExtractor_t5.cpp"

                    # T5 DR
                    "GameSpecific/Extractors/Extractor_t5/Extractor_t5dr.cpp"
                    "GameSpecific/AnimExtractors/AnimExtractor_t5dr.cpp"
                    "GameSpecific/MovesetConverters/Converter_t5dr_to_t7_Aliases.cpp"
                    "GameSpecific/MovesetConverters/Converter_t5dr_to_t7.cpp"
)                   

# This will be included in the .dll and should only be related to the injected DLL
set(MOVESETLOADER_SOURCES
    "Injected/MovesetLoader.cpp"

    "Injected/InjectionUtils.cpp"
    "Injected/steamHelper.cpp"

    #T8
    "Injected/MovesetLoaders/MovesetLoader_t8.cpp"

    #T7
    "Injected/MovesetLoaders/MovesetLoader_t7.cpp"
    "Injected/MovesetLoaders/MovesetLoader_t7_Importer.cpp"
    "Injected/MovesetLoaders/MovesetLoader_t7_Networking.cpp"

    "ResourcesClasses/GameAddressesFile.cpp"
    "ResourcesClasses/GameAddressesWrapper.cpp"
    "Utils/helpers.cpp"
    "Utils/Compression.cpp"
    
    "GameSpecific/MovesetConverters/Aliases.cpp"

    # Converters
    "GameSpecific/MovesetConverters/Converter_ttt2_to_t7.cpp"
    "GameSpecific/MovesetConverters/Converter_ttt2_to_t7_Aliases.cpp"
    "GameSpecific/Importers/Importer_t7/Importer_t7_Utils.cpp"

    "GameSpecific/MovesetConverters/Converter_trev_to_t7_Aliases.cpp"

    "GameSpecific/MovesetConverters/Converter_t6_to_t7.cpp"
    "GameSpecific/MovesetConverters/Converter_t6_to_t7_Aliases.cpp"

    "GameSpecific/MovesetConverters/Converter_t5_to_t7.cpp"
    "GameSpecific/MovesetConverters/Converter_t5_to_t7_Aliases.cpp"
)

## Setup embedded resources ##
set(EMBEDDED_RESOURCES "${CMAKE_CURRENT_BINARY_DIR}/../EmbeddedResources.c")
create_resources("${CMAKE_CURRENT_SOURCE_DIR}/Resources" ${EMBEDDED_RESOURCES})
# Build file embedder .EXE. This file embedder applies lz4 compression and can be used on files generated at compile them
add_executable (FileEmbedder "External/embedfile.cpp" "Utils/Compression.cpp" )
target_include_directories(FileEmbedder PRIVATE ${INCLUDE_FOLDERS})

# Build injected TKTKMovesetLoader.dll
add_library(TKMovesetLoader SHARED ${MOVESETLOADER_SOURCES} ${EMBEDDED_RESOURCES})
target_include_directories(TKMovesetLoader PRIVATE ${INCLUDE_FOLDERS})
target_compile_definitions(TKMovesetLoader PUBLIC DLL_EXPORTING)
add_dependencies(TKMovesetLoader FileEmbedder)
if (${CMAKE_TKM_BUILD_TYPE} MATCHES "Debug")
    target_compile_definitions(TKMovesetLoader PUBLIC BUILD_TYPE_DEBUG)
endif()


## Don't embed TKMovesetLoader into main binary ##
set(MOVESET_LOADER_EMBEDDED "")

# Build lib containing per-game Import/Export movesets functions
add_library(GameSpecific STATIC ${GAMESPECIFIC_SOURCES} ${EMBEDDED_RESOURCES})
target_include_directories(GameSpecific PRIVATE ${INCLUDE_FOLDERS} )
target_compile_definitions(GameSpecific PUBLIC DLL_EXPORTING)
if (${CMAKE_TKM_BUILD_TYPE} MATCHES "Debug")
    target_compile_definitions(GameSpecific PUBLIC BUILD_TYPE_DEBUG)
endif()

# Build program .exe
enable_language("RC") # Icon and more resources later, if needed
if (${CMAKE_TKM_BUILD_TYPE} MATCHES "Debug")
    set(RC_FILE "app_debug.rc")
else()
    set(RC_FILE "app.rc")
endif()
add_executable (TKMovesets2 WIN32 ${BINARY_SOURCES} ${EMBEDDED_RESOURCES} ${MOVESET_LOADER_EMBEDDED} ${RC_FILE})
if (${CMAKE_TKM_BUILD_TYPE} MATCHES "Debug")
    target_compile_definitions(TKMovesets2 PUBLIC BUILD_TYPE_DEBUG)
endif()

target_include_directories(TKMovesets2 PRIVATE ${INCLUDE_FOLDERS})
add_dependencies(TKMovesets2 GameSpecific)
add_dependencies(TKMovesets2 TKMovesetLoader)
target_link_libraries(TKMovesets2 PRIVATE GameSpecific)

# Copy addresses file to build folder, even if it's already embedded '#
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Resources/game_addresses.ini" "game_addresses.ini" COPYONLY)
# Copies TKMInterface folder containg extra, optional interface data (editor labels, maybe more later down the line)
add_custom_command(TARGET TKMovesets2 PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Resources/TKMInterface" "${CMAKE_CURRENT_BINARY_DIR}/TKMInterface")

## Extra dependencies ##
# Link the other libraries that we use #

# OpenGL (Renderer)
find_package(glad CONFIG REQUIRED)
target_link_libraries(TKMovesets2 PRIVATE glad::glad)

# GLFW (Renderer)
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(TKMovesets2 PRIVATE glfw)

# ImGUI (Interface)
find_package(imgui CONFIG REQUIRED)
target_link_libraries(TKMovesets2 PRIVATE imgui::imgui)
target_link_libraries(GameSpecific PRIVATE imgui::imgui) # Only used for the color-defining includes

# Compression. I tried LZMA but it was too slow even on the fastest preset, so we're using LZ4 for now
## LZ4
find_package(lz4 CONFIG REQUIRED)
target_link_libraries(TKMovesets2 PRIVATE lz4::lz4)
target_link_libraries(FileEmbedder PRIVATE lz4::lz4)
target_link_libraries(TKMovesetLoader PRIVATE lz4::lz4)
## LZMA
find_package(liblzma CONFIG REQUIRED)
target_link_libraries(TKMovesets2 PRIVATE liblzma::liblzma)
target_link_libraries(FileEmbedder PRIVATE liblzma::liblzma)
target_link_libraries(TKMovesetLoader PRIVATE liblzma::liblzma)

# Hook (Polyhook)
find_package(PolyHook_2 CONFIG REQUIRED)
target_link_libraries(TKMovesetLoader PRIVATE PolyHook_2::PolyHook_2)
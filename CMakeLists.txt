cmake_minimum_required(VERSION 3.19)

# set the project name
project(sdl2-template)

# add the executable
add_executable(main "src/main.cpp")

target_compile_features(main PRIVATE cxx_std_23)
set_property(TARGET main PROPERTY CXX_STANDARD 23)
set_property(TARGET main PROPERTY CXX_STANDARD_REQUIRED ON)

if (MSVC)
        set_property(TARGET main PROPERTY WIN32_EXECUTABLE ON)
        set_property(TARGET main PROPERTY VS_DPI_AWARE "PerMonitor")
        target_compile_options(main PRIVATE /Zc:__cplusplus)
endif ()

find_package(SDL2 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

target_link_libraries(main PRIVATE
        SDL2::SDL2 
        SDL2::SDL2main
        imgui::imgui
)

function(copy_resources)
        set(oneValueArgs TARGET TARGET)
        set(multiValueArgs TARGET INPUT)
        set(oneValueArgs DESTINATION)

        cmake_parse_arguments(COPY_RESOURCES "${options}" "${oneValueArgs}"
                "${multiValueArgs}" ${ARGN}
        )

        file(GLOB COPY_RESOURCES_INPUT_FILES ${COPY_RESOURCES_INPUT})

        add_custom_target(${COPY_RESOURCES_TARGET} ALL
                COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:main>/${COPY_RESOURCES_DESTINATION}/"
                COMMAND ${CMAKE_COMMAND} -E copy ${COPY_RESOURCES_INPUT_FILES} "$<TARGET_FILE_DIR:main>/${COPY_RESOURCES_DESTINATION}/"
        )
endfunction()
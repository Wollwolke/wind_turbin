cmake_minimum_required(VERSION 3.26)

project(wind_turbin)

aux_source_directory(./src SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# target_compile_definitions(${PROJECT_NAME} PRIVATE SDL_LEAN_AND_MEAN)

# SDL
set(SDL2_DISABLE_INSTALL ON)
set(SDL2IMAGE_INSTALL OFF)
set(BUILD_SHARED_LIBS FALSE)
add_subdirectory(external/SDL)
add_subdirectory(external/SDL_image)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2main SDL2::SDL2-static SDL2_image::SDL2_image-static)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/output)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/data/ DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/output)

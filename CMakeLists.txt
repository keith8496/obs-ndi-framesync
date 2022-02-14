project(obs-ndi-framesync)

set(obs-ndi-framesync_SOURCES
    src/obs-ndi-framesync.cpp
    src/obs-ndi-framesync-source.cpp)
    
add_library(obs-ndi-framesync MODULE
    ${obs-ndi-framesync_SOURCES})
target_link_libraries(obs-ndi-framesync
    libobs)
    
install_obs_plugin_with_data(obs-ndi-framesync data)

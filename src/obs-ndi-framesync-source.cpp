// Preprocess OBS
#include <obs-module.h>

// OBS-NDI Stuff
#include "obs-ndi-framesync.hpp"
#include "obs-ndi-framesync-source.hpp"



const char *obs_ndi_framesync_source_get_name (void *data)
{
    UNUSED_PARAMETER(data);
    return obs_module_text("NDIFSPlugin.SourceName");
}

void* obs_ndi_framesync_source_create(obs_data_t* settings, obs_source_t* source)
{
    
}

void obs_ndi_framesync_source_destroy(void *data)
{
    
}

void obs_ndi_framesync_source_update(void *data, obs_data_t *settings)
{
    
}

void obs_ndi_framesync_source_video_tick(void *data, float seconds)
{
    
}

struct obs_source_info obs_ndi_framesync_source {
        .id           = "obs-ndi",
        .type         = OBS_SOURCE_TYPE_INPUT,
        .output_flags = OBS_SOURCE_ASYNC_VIDEO | OBS_SOURCE_AUDIO | OBS_SOURCE_DO_NOT_DUPLICATE,
        .get_name     = obs_ndi_framesync_source_get_name,
        .create       = obs_ndi_framesync_source_create,
        .destroy      = obs_ndi_framesync_source_destroy,
        .update       = obs_ndi_framesync_source_update,
        .video_tick   = obs_ndi_framesync_source_video_tick
};

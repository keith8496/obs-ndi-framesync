//
//  obs-ndi-framesync.hpp
//  ALL_BUILD
//
//  Created by Keith Senti on 2/12/22.
//

#ifndef obs_ndi_framesync_hpp
#define obs_ndi_framesync_hpp

#include <stdio.h>
#include "../lib/ndi/Processing.NDI.Advanced.h"

#define blog(level, msg, ...) blog(level, "[obs-ndi-framesync] " msg, ##__VA_ARGS__)

extern const NDIlib_v5* p_NDILib;
extern NDIlib_find_instance_t pNDI_find;

#endif /* obs_ndi_framesync_hpp */

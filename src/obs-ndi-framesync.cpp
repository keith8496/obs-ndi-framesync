// Preprocess System
#include <csignal>
#include <string>
#include <cstddef>
#include <cstring>
#include <cstdint>
#include <cstdio>
#include <atomic>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#else
#include <stdlib.h>
#include <dlfcn.h>
#endif

// Preprocess OBS
#include <obs-module.h>

// OBS-NDI Stuff
#include "obs-ndi-framesync.hpp"


OBS_DECLARE_MODULE()
OBS_MODULE_AUTHOR("Keith Senti")
OBS_MODULE_USE_DEFAULT_LOCALE("obs-ndi-framesync", "en-US")

extern struct obs_source_info obs_ndi_framesync_source;


const NDIlib_v5* p_NDILib = nullptr;
NDIlib_find_instance_t pNDI_find;

bool dynamicLoad()
{
#ifdef _WIN32
    // We check whether the NDI run-time is installed
    const char* p_ndi_runtime_v5 = getenv(NDILIB_REDIST_FOLDER);
    if (!p_ndi_runtime_v5)
    {    // The NDI run-time is not yet installed. Let the user know and take them to the download URL.
        blog(LOG_WARNING, "Please install the NewTek NDI Runtimes to use this application.");
        ShellExecuteA(NULL, "open", NDILIB_REDIST_URL, 0, 0, SW_SHOWNORMAL);
        return false;
    }

    // We now load the DLL as it is installed
    std::string ndi_path = p_ndi_runtime_v5;
    ndi_path += "\\" NDILIB_LIBRARY_NAME;

    // Try to load the library
    HMODULE hNDILib = LoadLibraryA(ndi_path.c_str());

    // The main NDI entry point for dynamic loading if we got the library
    //const NDIlib_v5* (*NDIlib_v5_load)(void) = NULL;
    if (hNDILib)
        *((FARPROC*)&NDIlib_v5_load) = GetProcAddress(hNDILib, "NDIlib_v5_load");

    // If we failed to load the library then we tell people to re-install it
    if (!NDIlib_v5_load)
    {    // Unload the DLL if we loaded it
        if (hNDILib)
            FreeLibrary(hNDILib);

        // The NDI run-time is not installed correctly. Let the user know and take them to the download URL.
        blog(LOG_WARNING, "Please re-install the NewTek NDI Runtimes to use this application.");
        ShellExecuteA(NULL, "open", NDILIB_REDIST_URL, 0, 0, SW_SHOWNORMAL);
        return false;
    }
#else
    std::string ndi_path;
    
    const char* p_NDI_runtime_folder = getenv(NDILIB_REDIST_FOLDER);
    if (p_NDI_runtime_folder)
    {
        ndi_path = p_NDI_runtime_folder;
        ndi_path += NDILIB_LIBRARY_NAME;
    }
    else ndi_path = NDILIB_LIBRARY_NAME;
    
    // Try to load the library
    void *hNDILib = dlopen(ndi_path.c_str(), RTLD_LOCAL | RTLD_LAZY);
    
    // The main NDI entry point for dynamic loading if we got the library
    const NDIlib_v5* (*NDIlib_v5_load)(void) = NULL;
    if (hNDILib)
        *((void**)&NDIlib_v5_load) = dlsym(hNDILib, "NDIlib_v5_load");
    
    // If we failed to load the library then we tell people to re-install it
    if (!NDIlib_v5_load)
    {    // Unload the library if we loaded it
        if (hNDILib)
            dlclose(hNDILib);

        blog(LOG_ERROR, "Please re-install the NewTek NDI Runtimes from %s to use this application.", NDILIB_REDIST_URL);
        return false;
    }
#endif
    
    // Lets get all of the DLL entry points
    p_NDILib = NDIlib_v5_load();
    
    // We can now run as usual
    if (!p_NDILib->initialize())
    {    // Cannot run NDI. Most likely because the CPU is not sufficient (see SDK documentation).
        // you can check this directly with a call to NDIlib_is_supported_CPU()
        blog(LOG_ERROR, "Cannont run NDI.");
        return false;
    }

    // Success. We are done
    blog(LOG_INFO, "Loaded %s.", p_NDILib->version());
    return true;
}


bool ndilib_find()
{
    // Not required, but "correct" (see the SDK documentation.
    if (!p_NDILib->initialize()) return false;
    
    // We are going to create an NDI finder that locates sources on the network.
    //blog(LOG_INFO, "Trying to start NDI Finder.");
    NDIlib_find_create_t find_create_values;
    find_create_values.show_local_sources = true;
    find_create_values.p_groups = NULL;
    find_create_values.p_extra_ips = NULL;
    pNDI_find = p_NDILib->find_create(&find_create_values);
    if (!pNDI_find) {
        blog(LOG_ERROR, "Unable to start NDI Finder.");
        return false;
    }

    // Success. We are done
    blog(LOG_INFO, "Started NDI Finder.");
    return true;
}


bool obs_module_load(void)
{
    // Try to load NDI SDK and Finder
    if (!dynamicLoad()) return false;
    if (!ndilib_find()) return false;
    
    // Register stuff
    obs_register_source(&obs_ndi_framesync_source);
    
    blog(LOG_INFO, "Startup complete.");
    return true;
}


void obs_module_unload()
{
    if (p_NDILib) {
        p_NDILib->find_destroy(pNDI_find);
        p_NDILib->destroy();
    }
}

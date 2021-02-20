//Audio System

////// ------------ RAYLIB

//----- Struct/type

// Not really a good idea since it depend on other structs from Raylib, but at least I can replace them if needed

// // Sound source type
// typedef struct Sound {
//     AudioStream stream;             // Audio stream
//     unsigned int sampleCount;       // Total number of samples
// } Sound;

// // Music stream type (audio file streaming from memory)
// // NOTE: Anything longer than ~10 seconds should be streamed
// typedef struct Music {
//     AudioStream stream;             // Audio stream
//     unsigned int sampleCount;       // Total number of samples
//     bool looping;                   // Music looping enable

//     int ctxType;                    // Type of music context (audio filetype)
//     void *ctxData;                  // Audio context data, depends on type
// } Music;

//----- Load
Music VN_LoadMusicStream(const char *fileName)
{
    return LoadMusicStream(fileName);
}

Sound VN_LoadSound(const char *fileName)
{
    return LoadSound(fileName);
}

//----- Unload
void VN_UnloadMusicStream(Music music)
{
    UnloadMusicStream(music);
}

void VN_UnloadSound(Sound sound)
{
    UnloadSound(sound);
}

//----- Update

void VN_UpdateMusicStream(Music music)
{
    UpdateMusicStream(music);
};

void VN_PlayMusicStream(Music music)
{
    PlayMusicStream(music);
};

void VN_StopMusicStream(Music music)
{
    StopMusicStream(music);
};
void VN_PauseMusicStream(Music music)
{
    PauseMusicStream(music);
};
void VN_ResumeMusicStream(Music music)
{
    ResumeMusicStream(music);
};

//Sounds
void VN_PlaySound(Sound sound)
{
    PlaySound(sound);
}
void VN_StopSound(Sound sound)
{
    StopSound(sound);
}
void VN_PauseSound(Sound sound)
{
    PauseSound(sound);
}
void VN_ResumeSound(Sound sound)
{
    ResumeSound(sound);
}

//----- Misc

void VN_SetMasterVolume(float volume) { SetMasterVolume(volume); }
void VN_SetMusicVolume(Music music, float volume) { SetMusicVolume(music, volume); }

void VN_InitAudioDevice() { InitAudioDevice(); }
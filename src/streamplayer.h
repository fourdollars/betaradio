#ifndef __STREAM_PLAYER_H__
#define __STREAM_PLAYER_H__
#ifdef __cplusplus
extern "C" {
#endif
    typedef enum StreamStatus StreamStatus;
    typedef struct StreamPlayer StreamPlayer;

    enum StreamStatus {
        SP_Null,
        SP_Play,
        SP_Error,
    };

    StreamPlayer* StreamPlayerCreate(void);

    typedef int (*StreamCallback)(StreamPlayer*, StreamStatus, void*);

    struct StreamPlayer {
        void* data;
        void (*Play)(StreamPlayer*, const char* const);
        void (*Stop)(StreamPlayer*);
        void (*Register)(StreamPlayer*, StreamCallback, void*);
        void (*Release)(StreamPlayer*);
    };
#ifdef __cplusplus
}
#endif
#endif /* __STREAM_PLAYER_H__ */

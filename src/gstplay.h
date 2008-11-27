#ifndef __GSTPLAY_H___
#define __GSTPLAY_H___
#ifdef __cplusplus
extern "C" {
#endif
    typedef enum {
        GstNull,
        GstPlay,
        GstError,
    } GstStatus;

    typedef struct _GstPlayer GstPlayer;

    GstPlayer* gstCreate(void);

    typedef int (*GstCallback)(GstPlayer*, GstStatus);

    struct _GstPlayer {
        void* data;
        GstPlayer** self;
        void (*Play)(GstPlayer*, const char* const);
        void (*Stop)(GstPlayer*);
        void (*Register)(GstPlayer*, GstCallback);
        void (*Release)(GstPlayer*);
    };
#ifdef __cplusplus
}
#endif
#endif /* __GSTPLAY_H___ */

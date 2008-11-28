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

    typedef int (*GstCallback)(GstPlayer*, GstStatus, void*);

    struct _GstPlayer {
        void* data;
        GstPlayer** self;
        void (*Play)(GstPlayer*, const char* const);
        void (*Stop)(GstPlayer*);
        void (*Register)(GstPlayer*, GstCallback, void*);
        void (*Release)(GstPlayer*);
    };
#ifdef __cplusplus
}
#endif
#endif /* __GSTPLAY_H___ */

#include <stdbool.h>
#include <sys/cdefs.h>

#ifndef __RADIO_SITE_H__
#define __RADIO_SITE_H__
__BEGIN_DECLS

typedef struct RadioSite RadioSite;

RadioSite* createRadioSite(const char* url);

struct RadioSite {
    int (*getTitle)(RadioSite* self);
    int (*getURL)(RadioSite* self);

    int (*getCategorySize)(RadioSite* self);
    const char* (*getCategory)(RadioSite* self, int category);

    int (*getChannelSize)(RadioSite* self, int category);
    const char* (*getChannelTitle)(RadioSite* self, int category, int channel);
    const char* (*getChannelURL)(RadioSite* self, int category, int channel);

    void (*destroy)(RadioSite* self);
};
__END_DECLS
#endif /* __RADIO_SITE_H__*/

/**
 * Copyright (C) 2010 Shih-Yuan Lee (FourDollars) <fourdollars@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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

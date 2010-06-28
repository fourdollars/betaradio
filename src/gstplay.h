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

#ifndef __GST_PLAYER_H__
#define __GST_PLAYER_H__
#ifdef __cplusplus
extern "C" {
#endif
    typedef enum GstStatus GstStatus;
    typedef struct GstPlayer GstPlayer;

    enum GstStatus {
        GP_Null,
        GP_Play,
        GP_Error,
    };

    GstPlayer* GstPlayerCreate(void);

    typedef int (*GstCallback)(GstPlayer*, GstStatus, void*);

    struct GstPlayer {
        void* data;
        void (*Play)(GstPlayer*, const char*, const char*);
        void (*Stop)(GstPlayer*);
        void (*Register)(GstPlayer*, GstCallback, void*);
        void (*Release)(GstPlayer*);
    };
#ifdef __cplusplus
}
#endif
#endif /* __GST_PLAYER_H__ */

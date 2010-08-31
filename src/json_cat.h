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

#include <features.h>
#include <stdbool.h>
#ifndef __JSON_CAT_H__
#define __JSON_CAT_H__
__BEGIN_DECLS
typedef enum json_cat_type {
    JC_FROM_OBJECT,
    JC_FROM_ARRAY,
} json_cat_type;
typedef struct json_cat json_cat;
extern json_cat* json_cat_create(void);
struct json_cat {
    void* _priv;

    json_cat* (*load) (json_cat* cat, const char* file);
    json_cat* (*feed) (json_cat* cat, const char* fish);
    json_cat* (*http) (json_cat* cat, const char* url);
    json_cat* (*object) (json_cat* cat, const char* string);
    json_cat* (*sibling) (json_cat* cat, const char* string);
    json_cat* (*array) (json_cat* cat, unsigned int index);
    json_cat* (*parent) (json_cat* cat);
    json_cat* (*grandparent) (json_cat* cat);
    json_cat* (*reset) (json_cat* cat);

    bool (*isObject) (json_cat* cat);
    bool (*isArray) (json_cat* cat);

    bool (*isString) (json_cat* cat);
    bool (*isInt) (json_cat* cat);
    bool (*isDouble) (json_cat* cat);
    bool (*isBool) (json_cat* cat);

    void (*destroy) (json_cat* cat);

    const char* (*getString) (json_cat* cat);
    int (*getInt) (json_cat* cat);
    double (*getDouble) (json_cat* cat);
    bool (*getBool) (json_cat* cat);
    unsigned int (*length) (json_cat* cat);
};
__END_DECLS
#endif //__JSON_CAT_H__

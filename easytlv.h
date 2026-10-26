/**
 *  _____              _____ _     _   _ 
 * |  ___|            |_   _| |   | | | |
 * | |__  __ _ ___ _   _| | | |   | | | |
 * |  __|/ _` / __| | | | | | |   | | | |
 * | |__| (_| \__ \ |_| | | | |___\ \_/ /
 * \____/\__,_|___/\__, \_/ \_____/\___/ 
 *                  __/ |                
 *                 |___/                 
 *
 * This library implements the ASN1 Tag-Length-Value encoding standard. For
 * more information about this standard, see the ISO8825-1 Basic Encoding
 * Rules: https://www.itu.int/ITU-T/studygroups/com17/languages/X.690-0207.pdf
 * 
 * Note: This library does not implement indefinite length TLV objects, as
 * described in section 8.1.3
 */

/**
 * Copyright (c) 2020 John Boyd <johnboydiv@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef __EASYTLV_H__
#define __EASYTLV_H__

#include <stdint.h>

#define EASYTLV_VER_MAJOR 1
#define EASYTLV_VER_MINOR 0
#define EASYTLV_VER_PATCH 0


typedef enum {
    ETLV_ERR_UNKNOWN    = -128, // Unknown failure
    ETLV_ERR_BADARG,            // Bad argument
    ETLV_ERR_OVERFLOW,          // Overflow detected
    ETLV_ERR_NOMEM,             // Not enough memory
    ETLV_ERR_INVAL,             // Invalid TLV data
    ETLV_ERR_MSGSIZE,           // TLV data exceeds provided size
    ETLV_ERR_NODATA,            // Not enough data was provided
    ETLV_ERR_NOENT,             // No entry was found
    ETLV_ERR_OK         = 0,    // No error
} ETLVError;

// A token describes a TLV object. Tag field does not need to occupy the
// entire 32b width. For example, a tag of 0x14 will properly be encoded as a
// single byte field.
typedef struct {
    uint32_t    tag;
    uint32_t    len;
    const void* val;
} ETLVToken;


/**
 * Parse TLV encoded data for TLV objects
 *
 * This parser will parse only one level of TLV objects. An object may include
 * TLV encoded data itself (ie nested TLV). To parse nested TLV objects, this
 * parser should be invoked again on the object's value data.
 *
 * In the event of an ETLV_ERR_NOMEM error, the output of `nTok` will still
 * represent the total number of tokens found in the byte array.
 *
 * [output] t       Array of tokens to be populated with parsed data
 * [in/out] nTok    Input size of the array / Output number of tokens parsed
 * [input]  src     Source pointer to TLV data
 * [input]  srcLen  Length source data to parse
 *
 * Returns number of tokens parsed, or negative error
 */
int etlv_parse(ETLVToken* t, int* nTok, const void* src, int srcLen);

/**
 * Serialize an array of TLV objects
 *
 * [output] dest    Destination to receive serialized data
 * [in/out] len     Input size of the destination / Output serialized length
 * [input]  t       Array of tokens to be serialized
 * [input]  nTok    Number of tokens to be serialized
 *
 * Returns the length of the serialized data, or negative error
 */
int etlv_serialize(void* dest, int* len, const ETLVToken* t, int nTok);

/**
 * Find the first occurance of a tag in a TLV encoded payload
 *
 * [output] t       Token information for the found tag (if found)
 * [input]  src     Source pointer to TLV data
 * [input]  srcLen  Length source data to search
 *
 * Returns the byte offset of the found token, or negative error
 */
int etlv_find(ETLVToken* t, uint32_t tag, const void* src, int srcLen);

#endif /* __EASYTLV_H__ */

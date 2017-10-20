/*
*			GPAC - Multimedia Framework C SDK
*
*			Authors: Jean Le Feuvre
*			Copyright (c) Telecom ParisTech 2000-2012
*					All rights reserved
*
*  This file is part of GPAC / Crypto Tools sub-project
*
*  GPAC is free software; you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation; either version 2, or (at your option)
*  any later version.
*
*  GPAC is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; see the file COPYING.  If not, write to
*  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
*
*/

#ifndef _GF_CRYPT_DEV_H_
#define _GF_CRYPT_DEV_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <gpac/crypt.h>


	/*the smallest version of the lib: only AES-128-CTR and AES-CBC supported*/
#define GPAC_CRYPT_ISMA_ONLY


	typedef void(*mcryptfunc)(void*, void*);
	typedef GF_Err(*mcrypt_setkeystream)(void *, const void *, int, const void *, int);
	typedef GF_Err(*mcrypt_setkeyblock) (void *, const void *, int);
	typedef GF_Err(*mcrypt_docrypt) (void *, const void *, int);

	/*private - do not use*/
	typedef struct _tag_crypt_stream
	{
		GF_CRYPTO_ALGO algo;
		GF_CRYPTO_MODE mode;
		u32 mode_version;

		/* Internam context*/
		void *context;
		/* holds the key */
		void *keyword_given;

		/*modes access*/
		GF_Err(*_init_crypt) (GF_Crypt*, void*, const void*);
		void(*_deinit_crypt) (GF_Crypt*);
		void(*_end_crypt) (GF_Crypt*);
		void(*_set_key)(GF_Crypt*);
		GF_Err(*_crypt) (GF_Crypt*, void*, int);
		GF_Err(*_decrypt) (GF_Crypt*, void*, int);
		GF_Err(*_set_state) (GF_Crypt*, void*, int);
		GF_Err(*_get_state) (GF_Crypt*, void*, int*);

		/*all below are static vars for mode and algo - sizes are in bytes*/
		u32 algo_block_size;
		u32 key_size;
		u32 algo_IV_size;

		u32 mode_size;
		Bool is_block_algo, is_block_algo_mode, is_block_mode, has_IV;
	} GF_CryptStream;


#define rotl32(x,n)   (((x) << ((u32)(n))) | ((x) >> (32 - (u32)(n))))
#define rotr32(x,n)   (((x) >> ((u32)(n))) | ((x) << (32 - (u32)(n))))
#define rotl16(x,n)   (((x) << ((u16)(n))) | ((x) >> (16 - (u16)(n))))
#define rotr16(x,n)   (((x) >> ((u16)(n))) | ((x) << (16 - (u16)(n))))

	/* Use hardware rotations.. when available */
#ifdef swap32
# define byteswap32(x) swap32(x)
#else
# ifdef swap_32
#  define byteswap32(x) swap_32(x)
# else
#  ifdef bswap_32
#   define byteswap32(x) bswap_32(x)
#  else
#   define byteswap32(x)	((rotl32(x, 8) & 0x00ff00ff) | (rotr32(x, 8) & 0xff00ff00))
#  endif
# endif
#endif

#ifdef swap16
# define byteswap16(x) swap16(x)
#else
# ifdef swap_16
#  define byteswap16(x) swap_16(x)
# else
#  ifdef bswap_16
#   define byteswap16(x) bswap_16(x)
#  else
#   define byteswap16(x)	((rotl16(x, 8) & 0x00ff) | (rotr16(x, 8) & 0xff00))
#  endif
# endif
#endif

	GFINLINE static
		void memxor(unsigned char *o1, unsigned char *o2, int length)
	{
		int i;

		for (i = 0; i < length; i++) {
			o1[i] ^= o2[i];
		}
		return;
	}


#define Bzero(x, y) memset(x, 0, y)


#ifdef __cplusplus
}
#endif

#endif	/*_GF_CRYPT_DEV_H_*/

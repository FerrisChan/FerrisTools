#ifndef MD5_H
#define MD5_H

#ifdef __alpha
typedef unsigned int uint32;
#else
typedef unsigned long uint32;
#endif

#ifdef __cplusplus
#define CONST const
#else
#define CONST
#endif

struct MD5Context {
        uint32 buf[4];
        uint32 bits[2];
        unsigned char in[64];
};

#ifdef __cplusplus
extern "C"{
#endif
void MD5Init(struct MD5Context* ctx);
void MD5Update(struct MD5Context* ctx,unsigned CONST char* buf,unsigned len);
void MD5Final(unsigned char digest[16],CONST struct MD5Context* ctx);
void MD5Transform(uint32 buf[4],uint32 in[16]);
#ifdef __cplusplus
}
#endif

/*
 * This is needed to make RSAREF happy on some MS-DOS compilers.
 */
typedef struct MD5Context MD5_CTX;

#endif /* !MD5_H */


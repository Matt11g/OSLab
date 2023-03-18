#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
   assert(s != NULL);
   size_t len = 0;
   while (*(s++) != '\0') len++;
   return len;
}

char *strcpy(char *dst, const char *src) {
   assert(dst != NULL && src != NULL);
   char *temp = dst;
   while ((*(dst++) = *(src++)) != '\0');
   return temp;
}

char *strncpy(char *dst, const char *src, size_t n) {//???
   assert(dst != NULL && src != NULL);
   char *temp = dst;
   size_t len = n;
   while (len-- && ((*(dst++) = *(src++)) != '\0'));
   while (len--) *(dst++) = '\0';
   return temp;
}

char *strcat(char *dst, const char *src) {
  assert(dst != NULL && src != NULL);
  char *temp = dst;
  while (*dst != '\0') ++dst;
  while ((*(dst++) = *(src++)));
  return temp;
}

int strcmp(const char *s1, const char *s2) {
  assert(s1 != NULL && s2 != NULL);
  while (*s1 != '\0' && *s2 != '\0' && *s1 == *s2){s1++; s2++;}
  return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

int strncmp(const char *s1, const char *s2, size_t n) {
  assert(s1 != NULL && s2 != NULL);
  if (n == 0) return 0;//!!!
  while (--n && *s1 && *s1 == *s2){s1++; s2++;}
  return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

void *memset(void *s, int c, size_t n) {
  assert(s != NULL);
  char *temp = (char *)s;
  while(n--){
     *(temp++) = (char)c;
  }
  return s;
}

void *memmove(void *dst, const void *src, size_t n) {
  assert(dst != NULL && src != NULL);//???
  void *ret = dst;
  char *dst_temp = (char *)dst;
  const char *src_temp = (const char *)src;
  if (dst <= src || (char *)dst >= (char *)src + n)
  {
     while (n--) *(dst_temp++) = *(src_temp++);
  }
  else
  {
     dst_temp += n - 1;
     src_temp += n - 1;
     while (n--) *(dst_temp--) = *(src_temp--);
  }
  return ret;
}

void *memcpy(void *out, const void *in, size_t n) {
  assert(out != NULL && in != NULL);//???
  char *temp_out = (char *)out;
  const char *temp_in = (const char *)in;
  while (n--) *(temp_out++) = *(temp_in++);
  return out;
}

int memcmp(const void *s1, const void *s2, size_t n) {
   if (n == 0) return 0;
   while (--n && *(char *)s1 == *(char *)s2){
     s1 = (char *)s1 + 1;
     s2 = (char *)s2 + 1;
   }
   return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

#endif

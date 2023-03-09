#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) { 
   va_list ap;
   va_start(ap, fmt);
   char buf[2048];
   int len = vsprintf(buf, fmt, ap);
   va_end(ap);
   char *p = buf;
   while (*p != '\0') putch(*p++);
   return len;
}

char hex[] = "0123456789abcdef";
int vsprintf(char *out, const char *fmt, va_list ap) {
  char *str = out;
  char *s;
  int d, idx = -1, base;
	uint32_t ptr;
  char tmp[64 + 3];
  for (str = out; *fmt; ++fmt)
  {
     if (*fmt != '%')
     {
       *str++ = *fmt;
       continue;
     }
     switch (*(++fmt)){
     case 's':
       s = va_arg(ap, char *);
       assert(s != NULL);
       while (*s != '\0') *str++ = *s++;
       break;
     case 'd':
       idx = -1;
       base = 10;
       d = va_arg(ap, int);
       if (d < 0) {*str++ = '-'; d = -d;}
       if (d == 0) tmp[++idx] = '0';
       while (d != 0)
       {
         tmp[++idx] = (char)(d % base + '0');
	       d /= base;
       }
       for (int i = idx; i >= 0; i--) *str++ = tmp[i];
       break;
		 case 'p':
			 idx = -1;
			 base = 16;
       ptr = va_arg(ap, uint32_t);
			 if (ptr == 0) tmp[++idx] = '0';
			 while (ptr != 0)
			 {
         tmp[++idx] = hex[ptr % base];
				 ptr /= base;
			 }
       for (int i = idx; i >= 0; i--) *str++ = tmp[i];
			 break;
     default: break;
     }
  }
  *str = '\0';
  int len = 0;
  char *temp = out;
  while (*temp++ != '\0') len++;
  return len;
}

int sprintf(char *out, const char *fmt, ...) {
   va_list ap;
   va_start(ap, fmt);
   int len = vsprintf(out, fmt, ap);
   va_end(ap);
   return len;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif

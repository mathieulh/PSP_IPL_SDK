void *memset(void *dst,int code,int size)
{
	unsigned char *p1 = (unsigned char *)dst;
	while(size--)
	{
		*p1++ = code;
	}
	return p1;
}

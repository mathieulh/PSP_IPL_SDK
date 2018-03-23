void *memcpy(void *dst,void *src,int size)
{
	unsigned char *p1 = (unsigned char *)dst;
	unsigned char *p2 = (unsigned char *)src;
	while(size--)
	{
		*p1++ = *p2++;
	}
	return p1;
}

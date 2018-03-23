int memcmp(const char *p1,const char *p2,int size)
{
	char diff;
	while(size--)
	{
	diff = *p1++ - *p2++;
	if(diff) return diff;
	}
	return 0;
}

int ms_load_file(const char *path,void *top_addr);
int ms_save_file(const char *path,const void *data,int size);
void *ms_load_ipl(const char *path);

int ms_fat_init(void);


extern void init_display ();
extern int Convert(unsigned int Data, char * charData);
extern void PreparDispl(int Leng, int N, char * charData);
void GLCD_DisplayU32(unsigned int ln, unsigned int col, unsigned char fi, unsigned int Data);
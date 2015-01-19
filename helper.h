#define dimof(x) (sizeof(x) / sizeof(x[0]))

void *LoadFile(const char *fileName, int* size = nullptr);
bool SaveFile(const char *fileName, const uint8_t* buf, int size);
void GoMyDir();

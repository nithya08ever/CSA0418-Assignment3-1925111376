#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main(void)
{
    void *handle = dlopen("./libencrypt.so", RTLD_LAZY);

    if (!handle) {
        fprintf(stderr, "Library error: %s\n", dlerror());
        return 1;
    }

    dlerror();

    void (*encrypt_message)(const char *) =
        (void (*)(const char *))dlsym(handle, "encrypt_message");

    char *error = dlerror();

    if (error) {
        fprintf(stderr, "%s\n", error);
        dlclose(handle);
        return 1;
    }

    printf("Runtime library loaded successfully\n");
    encrypt_message("BANK123");

    dlclose(handle);
    return 0;
}

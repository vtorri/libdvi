
#include <stdio.h>

#include <Dvi.h>

int main(int argc, char *argv[])
{
    Dvi_Document *doc;

    if (argc < 2)
        return -1;

    if (!dvi_init())
        return -1;
    dvi_log_level_set(3);
    doc = dvi_document_new(argv[1]);

    if (!doc)
        printf(" * errr\n");
    else
        dvi_document_del(doc);

    dvi_shutdown();

    return 0;
}

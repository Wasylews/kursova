;;; TODO: structure declaration, array of structure
struct book {
    char author[20];
    char title[20];
    char *genre[10];

    int pages_count;
    int book_count;

    float price;
}

book *arr;

;;; TODO: some sorting algo, quicksort for example and binary search
;;; TODO: read/write arr to/from file (fscanf()/fprintf())

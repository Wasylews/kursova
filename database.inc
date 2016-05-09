.data
plug db 'print all records...', ENDL, 0

.code

; bool db_read_record(void *p, FILE handle) -> ax
db_read_record proc c
    arg @p, @handle

    puts plug

    ; TODO: read record from file and store in p

    mov ax, 0
    ret
db_read_record endp


; db_write_record(void *p, FILE handle)
db_write_record proc c
    arg @p, @handle

    puts plug

    ; TODO: write record from p to file
    ret
db_write_record endp


; db_remove_record(int recnum, FILE handle)
db_remove_record proc c
    arg @recnum, @handle

    ; TODO: remove recnum row from file
    ret
db_remove_record endp


; db_update_record(int recnum, void *p, FILE handle)
db_update_record proc c
    arg @recnum, @p, @handle

    ; TODO: update record @recnum by p structure
    ret
db_update_record endp


; db_search_record(int column, char *filter, FILE handle)
db_search_record proc c
    arg @column, @filter, @handle

    ; TODO: print records where filter in column
    ret
db_search_record endp


; db_sort_record(int column, FILE handle)
db_sort_record proc c
    arg @column, @handle

    ; TODO: sort records by column
    ret
db_sort_record endp

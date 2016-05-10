.data
plug db 'print all records...', ENDL, 0

.code

; db_read_column(char *s, int size, char del, FILE handle)
db_read_column proc c
    arg @s, @size, @del, @handle
    uses si, cx

    mov si, @s
    mov cx, @size

@@read_loop:
    push @handle
    call fgetc
    add sp, 2

    test ax, ax            ; check for EOF
    je @@eof_return

    cmp al, byte ptr[@del] ; read until delimiter
    je @@stop_reading

    cmp al, 0ah            ; or until \n
    je @@stop_reading


    mov byte ptr[si], al
    inc si

    loop @@read_loop
@@stop_reading:
    ; add \0 to end of string
    mov byte ptr[si], 0

@@eof_return:
    ret
db_read_column endp

_read_column macro handle: req, del: req, size: req, p: req
ifnb <handle, del, size, p>
    push handle
    push del
    push size
    push p
    call db_read_column
    add sp, 8
endif
endm


; bool db_read_record(book *p, FILE handle) -> ax
db_read_record proc c
    arg @p, @handle

    ; FIXME: store data in @p as structure. Global array?
    _read_column @handle ';' 20 @p          ; author
    _read_column @handle ';' 20 [@p+20]     ; title
    _read_column @handle ';' 10 [@p+40]     ; genre
    _read_column @handle ';' 20 [@p+50]     ; publisher
    ; TODO: read year, pages_count and price
    ; mov ax, 0
    ret
db_read_record endp


; db_write_record(book *p, FILE handle)
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


; db_update_record(int recnum, book *p, FILE handle)
db_update_record proc c
    arg @recnum, @p, @handle

    ; TODO: update record recnum by p structure
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

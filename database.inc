.code
; int db_fetch(FILE handle, book *database) -> ax
db_fetch proc c
    arg @handle, @database
    local @@temp: byte: 256
    uses bx, si, di, cx

    mov bx, type book
    mov si, @database

    lea di, @@temp
    xor cx, cx
@@read_loop:
    push @handle
    push 256
    push di
    call fgets
    add sp, 6

    test ax, ax
    je @@done

    push si
    push di
    call parse_record
    add sp, 4

    inc cx
    add si, bx
    jmp @@read_loop

@@done:
    mov ax, cx
    ret
db_fetch endp


; void db_commit(FILE handle, const book* database)
db_commit proc c
    arg @handle, @database

    ret
db_commit endp

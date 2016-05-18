.data
rec db 256 dup('?')

.code
; int db_fetch(FILE handle, book *database) -> ax
db_fetch proc c
    arg @handle, @database
    uses bx, cx, si, dx

    mov bx, type book
    mov dx, @database

    lea si, rec
    xor cx, cx
@@read_loop:
    push @handle
    push 256
    push si
    call fgets
    add sp, 6

    test ax, ax
    je @@done

    push dx
    push si
    call parse_record
    add sp, 4

    inc cx
    add dx, bx
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

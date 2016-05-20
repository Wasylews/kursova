.data
file_fmt db '%s;%s;%s;%s;%s;%s;%s;', ENDL, 0
rec db 256 dup('?')

.code
; int db_fetch(FILE handle, book *database) -> ax
db_fetch proc c
    arg @handle, @database
    uses bx, cx, dx, si

    mov dx, type book
    mov bx, @database

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

    push bx
    push si
    call parse_record
    add sp, 4

    inc cx
    add bx, dx
    jmp @@read_loop

@@done:
    mov ax, cx
    ret
db_fetch endp


; void db_commit(FILE handle, const book* database, const int n)
db_commit proc c
    arg @handle, @database, @n
    uses dx, si, cx
    mov cx, @n

    mov dx, type book
    mov si, @database
@@print_loop:
    push si
    push offset file_fmt
    push @handle
    call print_record
    add sp, 6

    add si, dx
    loop @@print_loop
    ret
db_commit endp

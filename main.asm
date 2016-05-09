.model small
.386

.stack 256h

LOCALS

.data
filename db 'db.txt', 0
handle dw ?

.code

include stdio.inc
include menu.inc
include database.inc

main proc c
    mov ax, @data
    mov ds, ax

    push RW
    push offset filename
    call fopen
    add sp, 4

    cmp ax, 0
    jne @@open_success


    puts filename
    push EXIT_FAILURE
    call exit
    add sp, 2

@@open_success:
    mov handle, ax

    call show_main_menu

    ; switch al
    cmp al, 1               ; case 1
    je @@show_data

    ; default(7)
    jmp @@exit

@@show_data:
    push handle
    call db_show_table
    add sp, 2
    jmp @@break

@@break:
    push handle
    call fclose
    add sp, 2

@@exit:
    _exit
main endp

end main

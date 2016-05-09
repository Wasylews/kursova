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

    call show_main_menu

    ; switch al
    cmp al, 1               ; case 1
    je @@show_data

    ; default
    jmp @@exit

@@show_data:
    push handle
    call db_show_table
    add sp, 2
    jmp @@break

@@break:
    ;

@@exit:
    _exit
main endp

end main

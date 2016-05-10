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

@@main_loop:
    call show_main_menu

    ; switch al
    cmp al, 1               ; case 1
    je @@show_data

    cmp al, 2               ; case 2
    je @@add_data

    cmp al, 3               ; case 3
    je @@remove_data

    cmp al, 4               ; case 4
    je @@edit_data

    cmp al, 5
    je @@search_data        ; case 5

    cmp al, 6               ; case 6
    je @@sort_data

    cmp al, 7
    je @@exit

@@show_data:
    push handle
    call show_table
    add sp, 2
    jmp @@break

@@add_data:
    push handle
    call add_record
    add sp, 2
    jmp @@break

@@remove_data:
    push handle
    call remove_record
    add sp, 2
    jmp @@break

@@edit_data:
    push handle
    call edit_record
    add sp, 2
    jmp @@break

@@search_data:
    push handle
    call search_record
    add sp, 2
    jmp @@break

@@sort_data:
    push handle
    call sort_record
    add sp, 2
    jmp @@break

@@break:
    jmp @@main_loop

@@exit:
    push handle
    call fclose
    add sp, 2

    _exit
main endp

end main

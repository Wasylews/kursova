.model small
.386

.stack 256h

.data
filename db 'db.txt', 0

record_count dw ?
handle dw ?

book struc
    m_author db 20 dup ('?')
    m_title db 50 dup ('?')
    m_genre db 20 dup ('?')
    m_publisher db 30 dup ('?')

    m_year dw ?
    m_pages_count dw ?
    m_price dd ?
book ends


database book 100 dup(<>)

.code
LOCALS
include stdio.inc
include record.inc
include database.inc
include menu.inc

main proc c
    mov ax, @data
    mov ds, ax

    push RO
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

    push offset database
    push handle
    call db_fetch
    add sp, 4

    mov record_count, ax

    push handle
    call fclose
    add sp, 2

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
    je @@quit

@@show_data:
    push record_count
    push offset database
    call show_table
    add sp, 4
    jmp @@break

@@add_data:
    ; TODO
    jmp @@break

@@remove_data:
    ; TODO
    jmp @@break

@@edit_data:
    ; TODO
    jmp @@break

@@search_data:
    ; TODO
    jmp @@break

@@sort_data:
    ; TODO
    jmp @@break

@@break:
    jmp @@main_loop

@@quit:
    _exit
main endp

end main


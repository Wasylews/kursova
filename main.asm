.model small
.386

.stack 256h

.data
error db 'Cannot open database', 0
filename db 'db.txt', 0

db_size dw 0
handle dw ?

book struc
    m_author db 50 dup ('?')
    m_title db 50 dup ('?')
    m_genre db 50 dup ('?')
    m_publisher db 50 dup ('?')

    m_year db 5 dup('?')
    m_pages_count db 5 dup('?')
    m_price db 6 dup('?')
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

    ; open database file for read
    push RO
    push offset filename
    call fopen
    add sp, 4

    ; check for errors
    test ax, ax
    jne @@open_success

    ; if can't open file print error and exit
    puts error
    push EXIT_FAILURE
    call exit
    add sp, 2

@@open_success:
    mov handle, ax

    ; read file into book array
    push offset database
    push handle
    call db_fetch
    add sp, 4

    mov db_size, ax

    ; file is no longer needed, close it
    push handle
    call fclose
    add sp, 2

@@main_loop:
    ; show main menu
    push 7
    push 1
    push offset main_menu
    call range_check_input
    add sp, 6

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
    je @@show_about

    cmp al, 7
    je @@quit

@@show_data:
    ; print all records
    push db_size
    push offset database
    call show_table
    add sp, 4
    jmp @@break

@@add_data:
    push db_size
    push offset database
    call add_record
    add sp, 4
    inc db_size
    jmp @@break

@@remove_data:
    ; remove record by number
    push db_size
    push offset database
    call remove_record
    add sp, 4
    test ax, ax
    je @@cancel
    dec db_size
@@cancel:
    jmp @@break

@@edit_data:
    ; edit selected record
    push db_size
    push offset database
    call edit_record
    jmp @@break

@@search_data:
    ; print records, that match with search string
    push db_size
    push offset database
    call search_record
    add sp, 4
    jmp @@break

@@show_about:
    puts about_info

@@break:
    jmp @@main_loop

@@quit:
    ; rewrite database file
    push WO
    push offset filename
    call fopen
    add sp, 4

    ; check for errors
    test ax, ax
    jne @@ok

    ; if can't open file print error and exit
    puts error
    push EXIT_FAILURE
    call exit
    add sp, 2
@@ok:
    mov handle, ax
    ; write database
    push db_size
    push offset database
    push handle
    call db_commit
    add sp, 6
    ; and close file
    push handle
    call fclose
    add sp, 2

    _exit
main endp

end main


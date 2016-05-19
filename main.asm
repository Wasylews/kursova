.model small
.386

.stack 256h

.data
filename db 'db.txt', 0
error db 'Cannot open database', 0

db_size dw 0
handle dw ?

book struc
    m_author db 50 dup ('?')
    m_title db 50 dup ('?')
    m_genre db 50 dup ('?')
    m_publisher db 50 dup ('?')

; TODO: use strings? In filter_record you must convert int/float to string
    m_year db 5 dup('?')
    m_pages_count db 5 dup('?')
    m_price db 6 dup('?')

; TODO: skip removed records, change indexes, move to end?
    removed db 0
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
    cmp ax, 0
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
    je @@sort_data

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

@@sort_data:
    ; TODO
    jmp @@break

@@break:
    jmp @@main_loop

@@quit:
    _exit
main endp

end main


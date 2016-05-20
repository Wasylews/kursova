.data
main_menu db '1) Show all records', ENDL
          db '2) Append record', ENDL
          db '3) Remove record', ENDL
          db '4) Edit record', ENDL
          db '5) Search record', ENDL
          db '6) Sort table by column', ENDL
          db '7) Exit', ENDL
          db 'Choose what to do: ', 0

record_removed db 'Cannot process removed record.', ENDL, 0
error_fmt db 'Wrong input. Enter number from %d to %d: ', 0

enter_record_num db 'Enter record number: ', 0
filter_by db 'Filter by: ', 0

remove_prompt db '1) Yes', ENDL
              db '2) No', ENDL
              db 'Remove? ', 0

column_prompt db '1) Author', ENDL
              db '2) Title', ENDL
              db '3) Genre', ENDL
              db '4) Publisher', ENDL
              db '5) Year', ENDL
              db '6) Pages count', ENDL
              db '7) Price', ENDL
              db 'Choose column: ', 0

table_fmt db '%s | %s | %s | %s | %s | %s | %s', ENDL, 0

temp db 80 dup('?')

.code
; int range_check_input(char *prompt, int min, int max) -> al
range_check_input proc c
    arg @prompt, @min, @max
    uses dx

    mov dx, @prompt
    puts dx

@@repeat_input:
    call read_int

    cmp ax, @min
    jb @@wrong_input        ; if al < 1

    cmp ax, @max
    jbe @@return            ; al <= 7

@@wrong_input:
    push @max
    push @min
    push offset error_fmt
    push STDOUT
    call fprintf
    add sp, 8
    jmp @@repeat_input

@@return:
    ret
range_check_input endp


; show_table(book *database, const int n)
show_table proc c
    arg @database, @n
    uses bx, si, cx
    mov cx, @n

    mov bx, type book
    mov si, @database
@@print_loop:
    push si
    push offset table_fmt
    push STDOUT
    call print_record
    add sp, 6

    add si, bx
    loop @@print_loop
    ret
show_table endp


; filter_record(const int column, char *what, book *database, const int n)
filter_record proc c
    arg @column, @what, @database, @n
    uses dx, si, cx, di
    mov cx, @n

    mov dx, type book
    mov si, @database
@@filter_loop:
    push @column
    push si
    call get_member
    add sp, 4

    ; partial matching
    push @what
    push ax
    call strstr
    add sp, 4

    test ax, ax
    je @@skip

    push si
    push offset table_fmt
    push STDOUT
    call print_record
    add sp, 6

@@skip:
    add si, dx
    loop @@filter_loop
    ret
filter_record endp


; search_record(book *database, const int n)
search_record proc c
    arg @database, @n
    uses si, cx

    ; ask user in which column to search
    push 7
    push 1
    push offset column_prompt
    call range_check_input
    add sp, 6

    mov cx, ax

    puts filter_by
    lea si, temp
    push si
    call gets
    add sp, 2

    push @n
    push @database
    push si
    push cx
    call filter_record
    add sp, 8

    ret
search_record endp


; left_shift(const int from, book *database, const int n)
left_shift proc c
    arg @from, @database, @n
    uses cx, dx, si

    mov cx, @from

    ; get pointer to database[from]
    mov ax, @from
    mov dx, type book
    mul dx
    mov si, @database
    add si, ax

@@shift_loop:
    cmp cx, @n
    je @@done
; TODO: copy_record(si, si+1)

    add si, dx
    inc cx
    jmp @@shift_loop

@@done:
    ret
left_shift endp


; remove_record(book *database, const int n)
remove_record proc c
    arg @database, @n
    uses dx, si, cx

    ; ask for a record id
    push @n
    push 0
    push offset enter_record_num
    call range_check_input
    add sp, 6

    mov cx, ax

    mov dx, type book
    mul dx
    mov si, @database
    add si, ax

@@valid_record:
    push si
    push offset table_fmt
    push STDOUT
    call print_record
    add sp, 6

    ; confrim removing
    push 2
    push 1
    push offset remove_prompt
    call range_check_input
    add sp, 6

    cmp ax, 1
    jne @@cancel

    push @n
    push @database
    push cx
    call left_shift
    add sp, 6
@@cancel:
    ret
remove_record endp


; edit_record(book *database, const int n)
edit_record proc c
    arg @database, @n
    uses dx, si

    push @n
    push 0
    push offset enter_record_num
    call range_check_input
    add sp, 6

    mov dx, type book
    mul dx
    mov si, @database
    add si, ax

    push si
    push offset table_fmt
    push STDOUT
    call print_record
    add sp, 6

    push si
    call read_record
    add sp, 2

@@exit:
    ret
edit_record endp


; add_record(book *database, const int n)
add_record proc c
    arg @database, @n
    uses dx, si

    mov ax, @n
    mov dx, type book
    mul dx
    mov si, @database
    add si, ax

    push si
    call read_record
    add sp, 2
    ret
add_record endp


; sort_record(book *database, const int n)
sort_record proc c
    arg @database, @n
    uses si, dx, cx

    mov dx, type book
    mov si, @database
    mov cx, @n

    ; ask user by which column sort
    push 7
    push 1
    push offset column_prompt
    call range_check_input
    add sp, 6

@@print_loop:
; TODO: sort records
    add si, dx
    loop @@print_loop

    ret
sort_record endp

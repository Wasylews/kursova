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
              db '5) Price', ENDL
              db 'Choose column: ', 0

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
    uses dx, si, cx
    mov cx, @n

    mov dx, type book
    mov si, @database
@@print_loop:
    cmp [si].removed, 1
    je @@skip

    push si
    call print_record
    add sp, 2

@@skip:
    add si, dx
    loop @@print_loop
    ret
show_table endp


; filter_record(const int column, char *what, book *database, const int n)
filter_record proc c
    arg @column, @what, @database, @n
    uses dx, si, cx
    mov cx, @n

    mov dx, type book
    mov si, @database
@@filter_loop:
    cmp [si].removed, 1
    je @@skip

; TODO: strcmp([si].column, what)
    push si
    call print_record
    add sp, 2

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
    push 5
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


; remove_record(book *database, const int n)
remove_record proc c
    arg @database, @n
    uses dx, si

    ; ask for a record id
    push @n
    push 0
    push offset enter_record_num
    call range_check_input
    add sp, 6

    mov dx, type book
    mul dx
    mov si, @database
    add si, ax

    cmp [si].removed, 1
    jne @@valid_record
    puts record_removed
    jmp @@cancel

@@valid_record:
    push si
    call print_record
    add sp, 2

    ; confrim removing
    push 2
    push 1
    push offset remove_prompt
    call range_check_input
    add sp, 6

    cmp ax, 1
    jne @@cancel

    mov [si].removed, 1
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

    cmp [si].removed, 1
    jne @@valid_record
    puts record_removed
    jmp @@exit

@@valid_record:
    push si
    call print_record
    add sp, 2

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

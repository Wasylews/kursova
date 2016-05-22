.data
main_menu db '1) Show all records', ENDL
          db '2) Append record', ENDL
          db '3) Remove record', ENDL
          db '4) Edit record', ENDL
          db '5) Search record', ENDL
          db '6) About', ENDL
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

about_info db 'BookShop', ENDL
           db 'Version: 1.0', ENDL
           db 'Copyright (C) Sabov W.', ENDL, 0


table_fmt db 'Author: %s', ENDL
          db 'Title: %s', ENDL
          db 'Genre: %s', ENDL
          db 'Publisher: %s', ENDL
          db 'Year: %s', ENDL
          db 'Pages count: %s', ENDL
          db 'Price: %s', ENDL, 0

search_fmt db '%s | %s | %s | %s | %s | %s | %s', ENDL, 0

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


cls macro
    push dx
    mov ah, 6
    mov al, 0
    mov bh, 7
    mov cx, 0
    mov dh, 24
    mov dl, 79
    int 10h

    mov ah, 2
    mov bh, 0
    mov dh, 0
    mov dl, 0
    int 10h
    xor ax, ax
    pop dx
endm


; show_table(book *database, const int n)
show_table proc c
    arg @database, @n
    uses bx, si, dx
    mov bx, type book
    mov si, @database

    xor dx, dx
@@view_loop:
    cls

    push si
    push offset table_fmt
    push STDOUT
    call print_record
    add sp, 6

    _getch
    cmp al, 1bh
    je @@exit_view

    cmp ah, 48h
    je @@scroll_up

    cmp ah, 50h
    je @@scroll_down

@@scroll_up:
    test dx, dx
    jle @@back_skip

    dec dx
    sub si, bx
@@back_skip:
    jmp @@next

@@scroll_down:
    inc dx
    cmp dx, @n
    je @@first

    add si, bx
    jmp @@next

@@first:
    xor dx, dx
    mov si, @database

@@next:
    jmp @@view_loop

@@exit_view:
    cls
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
    push offset search_fmt
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


; left_shift(const int from, const int to, book *database)
left_shift proc c
    arg @from, @to, @database
    uses si, bx, cx

    mov bx, @from
    mov cx, @to
    dec cx

@@shift_loop:
    cmp bx, cx
    je @@done

    push @database
    push bx
    call db_record

    mov si, ax

    inc bx
    push @database
    push bx
    call db_record

    ; copy record from si+1 to si
    push type book
    push ax
    push si
    call memcpy
    add sp, 6

    jmp @@shift_loop

@@done:
    ret
left_shift endp


; remove_record(book *database, const int n)
remove_record proc c
    arg @database, @n
    uses cx
    ; ask for a record id
    mov cx, @n
    dec cx

    push cx
    push 0
    push offset enter_record_num
    call range_check_input
    add sp, 6

    mov cx, ax

    push @database
    push ax
    call db_record

@@valid_record:
    push ax
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

    push offset database
    push @n
    push cx
    call left_shift
    add sp, 6

    mov ax, 1
    jmp @@done

@@cancel:
    mov ax, 0

@@done:
    ret
remove_record endp


; edit_record(book *database, const int n)
edit_record proc c
    arg @database, @n
    uses si, cx
    ; ask for a record id
    mov cx, @n
    dec cx

    push cx
    push 0
    push offset enter_record_num
    call range_check_input
    add sp, 6

    push @database
    push ax
    call db_record
    mov si, ax

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

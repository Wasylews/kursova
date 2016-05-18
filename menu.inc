.data
main_menu db '1) Show all records', ENDL
          db '2) Append record', ENDL
          db '3) Remove record', ENDL
          db '4) Edit record', ENDL
          db '5) Search record', ENDL
          db '6) Sort table by column', ENDL
          db '7) Exit', ENDL
          db 'Choose what to do: ', 0

main_menu_error db 'Wrong input. Try again: ', 0

enter_record_num db 'Enter record number: ', 0
filter_by db 'Filter by: ', 0

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
range_check_input macro prompt: req, min: req, max:req
ifnb <prompt, min, max>
    puts prompt
@@repeat_input:
    call read_int

    cmp ax, min
    jb @@wrong_input        ; if al < 1

    cmp ax, max
    jbe @@return            ; al <= 7

@@wrong_input:
    puts main_menu_error
    jmp @@repeat_input

@@return:
endif
endm


; show_table(book *database, const int n)
show_table proc c
    arg @database, @n
    uses dx, si, cx
    mov cx, @n

    mov dx, type book
    mov si, @database
@@print_loop:
    push si
    call print_record
    add sp, 2

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
    ; TODO: strcmp([si].column, what)
    push si
    call print_record
    add sp, 2

    add si, dx
    loop @@filter_loop
    ret
filter_record endp


; search_record(book *database, const int n)
search_record proc c
    arg @database, @n
    local @@column
    uses si

    range_check_input column_prompt 1 5
    mov @@column, ax

    puts filter_by
    lea si, temp
    push si
    call gets
    add sp, 2

    push @n
    push @database
    push si
    push @@column
    call filter_record
    add sp, 8

    ret
search_record endp


; remove_record(book *database, const int n)
remove_record proc c
    arg @database, @n
    uses dx, si

    range_check_input enter_record_num 0 @n

    mov dx, type book
    mul dx
    mov si, @database
    add si, ax

    ; TODO: mark database[n] as removed, teach show_table skip such records
    ret
remove_record endp


; edit_record(book *database, const int n)
edit_record proc c
    arg @database, @n
    uses dx, si

    range_check_input enter_record_num 0 @n

    mov dx, type book
    mul dx
    mov si, @database
    add si, ax

    push si
    call print_record
    add sp, 2

    push si
    call read_record
    add sp, 2
    ret
edit_record endp

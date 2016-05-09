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
              db 'Choose column: ', 0
.code

; inr read_int() -> ax
read_int proc c
    local @@temp

    push @@temp
    call gets
    add sp, 2

    push @@temp
    call atoi
    add sp, 2
    ret
read_int endp


; int show_main_menu() -> al
show_main_menu proc c
    puts main_menu
@@repeat_input:
    call read_int

    cmp al, 1
    jb @@wrong_input        ; if al < 1

    cmp al, 7
    jbe @@return            ; al <= 7

@@wrong_input:
    puts main_menu_error
    jmp @@repeat_input

@@return:
    ret
show_main_menu endp


; show_table(FILE handle)
show_table proc c
    arg @handle
    local @@record

@@read_loop:
    push @handle
    push @@record
    call db_read_record
    add sp, 4

    cmp ax, 0
    je @@done

    ; TODO: print record
@@done:
    ret
show_table endp


; remove_record(FILE handle)
remove_record proc c
    arg @handle

    puts enter_record_num
    call read_int

    push @handle
    push ax
    call db_remove_record
    add sp, 4

    ret
remove_record endp


; search_record(FILE handle)
search_record proc c
    arg @handle
    local @@temp

    puts column_prompt
    call read_int

    puts filter_by
    push @@temp
    call gets
    add sp, 2

    push @handle
    push @@temp
    push ax
    call db_search_record
    add sp, 6

    ret
search_record endp


; sort_record(FILE handle)
sort_record proc c
    arg @handle
    local @@temp

    puts column_prompt
    call read_int

    push @handle
    push ax
    call db_sort_record
    add sp, 6

    ret
sort_record endp


; add_record(FILE handle)
add_record proc c
    arg @handle
    local @@temp

    ; TODO: input author, title, genre etc from keyboard
    ret
add_record endp


; edit_record(FILE handle)
edit_record proc c
    arg @handle
    local @@temp

    ; TODO: see add_record
    ret
edit_record endp

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


; int show_main_menu() -> al
show_main_menu proc c
    range_check_input main_menu 1 7
    ret
show_main_menu endp


; show_table(book *database, const int n)
show_table proc c
    arg @database, @n
    uses bx, si, cx

    mov cx, @n

    mov bx, type book
    mov si, @database
    ; FIXME: loop doesn't stop?
@@print_loop:
    push si
    call print_record
    add sp, 2

    add si, bx
    loop @@print_loop
    ret
show_table endp

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

.code

; int show_main_menu() -> al
show_main_menu proc c
    puts main_menu

@@repeat_input:
    getc
    push ax
    putc ENDL
    pop ax

    cmp al, '1'
    jb @@wrong_input        ; if al < '1'

    cmp al, '7'
    jbe @@return            ; al <= '7'

@@wrong_input:
    puts main_menu_error
    jmp @@repeat_input

@@return:
    sub al, '0'
    ret
show_main_menu endp

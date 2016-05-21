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

table_fmt db '%s | %s | %s | %s | %s | %s | %s', ENDL, ENDL, 0

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


; void swap(a, b, elem_size)
swap proc c
    arg @a, @b, @n
    uses ax, cx, si, di
    mov cx, @n
    mov si, @a
    mov di, @b
@@cycle:
    cmp cx, 0
    je @@end
    mov al, [di]
    mov ah, [si]
    mov [di], ah
    mov [si], al
    inc si
    inc di
    dec cx
    jmp @@cycle
@@end:
    ret
swap endp


book_cmp proc c
    arg @a, @b, @column
    uses si

    push @column
    push @a
    call get_member
    add sp, 4
    mov si, ax

    push @column
    push @b
    call get_member
    add sp, 4

    push si
    push ax
    call strcmp
    add sp, 4

    ret
book_cmp endp


; void insert_sort(void *arr, size_t arr_len, size_t size_elem, size_t column,
;                  int (*comparator)(const *void, const *void, size_t column))
insert_sort proc c
    arg @arr, @arr_len, @size_elem, @column, @comparator
    uses ax, bx, cx, dx, si, di
    mov cx, @arr_len
    dec cx
    mov bx, @arr
@@cycle1:
    cmp cx, 0
    je @@end
    mov si, bx
    mov di, bx
    sub di, @size_elem
@@cycle2:
    push @column
    push si
    push di
    call @comparator
    add sp, 6

    test ax, ax
    jle @@ns0

    push @size_elem
    push si
    push di
    call swap
    add sp, 6

    sub si, @size_elem
    sub di, @size_elem
    cmp di, @arr
    jnb @@cycle2
@@ns0:
    dec cx
    add bx, @size_elem
    jmp @@cycle1
@@end:
    ret
insert_sort endp


; sort_record(book *database, const int n)
sort_record proc c
    arg @database, @n

    ; ask user by which column sort
    push 7
    push 1
    push offset column_prompt
    call range_check_input
    add sp, 6

    push offset book_cmp
    push ax
    push type book
    push @n
    push @database
    call insert_sort
    add sp, 10

    ret
sort_record endp

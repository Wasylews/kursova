.data
sep db ' | ', 0

p_author db 'author: ', 0
p_title db 'title: ', 0
p_genre db 'genre: ', 0
p_publisher db 'publisher: ', 0
p_year db 'year: ', 0
p_pages db 'pages count: ', 0
p_price db 'price: ', 0

buf db 10 dup('?')


.code
; print_record(book rec)
print_record proc c
    arg @rec
    uses di, dx

    mov di, @rec

    lea dx, [di].m_author
    puts dx
    puts sep

    lea dx, [di].m_title
    puts dx
    puts sep

    lea dx, [di].m_genre
    puts dx
    puts sep

    lea dx, [di].m_publisher
    puts dx
    puts sep

    lea dx, [di].m_year
    puts dx
    puts sep

    lea dx, [di].m_pages_count
    puts dx
    puts sep

    lea dx, [di].m_price
    puts dx

    putc ENDL
    ret
print_record endp


; inr read_int() -> ax
read_int proc c
    uses dx
    lea dx, buf
    push dx
    call gets
    add sp, 2

    push dx
    call atoi
    add sp, 2
    ret
read_int endp



; read_record(book *rec)
read_record proc c
    arg @rec
    uses si, di

    mov si, @rec

    puts p_author
    lea di, [si].m_author
    push di
    call gets
    add sp, 2

    puts p_title
    lea di, [si].m_title
    push di
    call gets
    add sp, 2

    puts p_genre
    lea di, [si].m_genre
    push di
    call gets
    add sp, 2

    puts p_publisher
    lea di, [si].m_publisher
    push di
    call gets
    add sp, 2

    puts p_year
    lea di, [si].m_year
    push di
    call gets
    add sp, 2

    puts p_pages
    lea di, [si].m_pages_count
    push di
    call gets
    add sp, 2

    puts p_price
    lea di, [si].m_price
    push di
    call gets
    add sp, 2

    ret
read_record endp


; get_field(const int column, const char *rec, char *dest) -> ax(dest)
get_field proc c
    arg @column, @rec, @dest
    uses cx, bx, si

    mov cx, @column
    mov ax, @rec

@@find_loop:
    test cx, cx
    je @@exit_loop

    push ';'
    push ax
    call strchr
    add sp, 4

    inc ax
    dec cx
    jmp @@find_loop

@@exit_loop:
    mov bx, ax

    push ';'
    push ax
    call strchr
    add sp, 4

    sub ax, bx

    mov si, @dest
    push ax
    push bx
    push si
    call strncpy
    add sp, 6

    ret
get_field endp


; parse_record(const char *rec, book *dest)
parse_record proc c
    arg @rec, @dest
    uses si, di

    mov si, @dest

    ; get author
    lea di, [si].m_author
    push di
    push @rec
    push 0
    call get_field
    add sp, 6

    ; get title
    lea di, [si].m_title
    push di
    push @rec
    push 1
    call get_field
    add sp, 6

    ; get genre
    lea di, [si].m_genre
    push di
    push @rec
    push 2
    call get_field
    add sp, 6

    ; get publisher
    lea di, [si].m_publisher
    push di
    push @rec
    push 3
    call get_field
    add sp, 6

    ; get year
    lea di, [si].m_year
    push di
    push @rec
    push 4
    call get_field
    add sp, 6

    ; get pages_count
    lea di, [si].m_pages_count
    push di
    push @rec
    push 5
    call get_field
    add sp, 6

    ; get price
    lea di, [si].m_price
    push di
    push @rec
    push 6
    call get_field
    add sp, 6
    ret
parse_record endp


; char *get_member(book *record, int index) -> ax
get_member proc c
    arg @record, @index
    uses bx
    mov bx, @record

    xor ax, ax
    cmp @index, 1
    jne @@if_title
    lea ax, [bx].m_author
    jmp @@done
@@if_title:
    cmp @index, 2
    jne @@if_genre
    lea ax, [bx].m_title
    jmp @@done
@@if_genre:
    cmp @index, 3
    jne @@if_publisher
    lea ax, [bx].m_genre
    jmp @@done
@@if_publisher:
    cmp @index, 4
    jne @@if_year
    lea ax, [bx].m_publisher
    jmp @@done
@@if_year:
    cmp @index, 5
    jne @@if_pages
    lea ax, [bx].m_year
    jmp @@done
@@if_pages:
    cmp @index, 6
    jne @@if_price
    lea ax, [bx].m_pages_count
    jmp @@done
@@if_price:
    lea ax, [bx].m_price

@@done:
    ret
get_member endp

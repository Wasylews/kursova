.data
sep db ' | ', 0

p_author db 'author: ', 0
p_title db 'title: ', 0
p_genre db 'genre: ', 0
p_publisher db 'publisher: ', 0
p_year db 'year: ', 0
p_pages db 'pages count: ', 0
p_price db 'price: ', 0


.code
; print_int(int x)
print_int proc c
    arg @x
    local @@temp: byte: 5
    uses si

    lea si, @@temp

    push 10
    push si
    push @x
    call itoa
    add sp, 6

    puts si
    ret
print_int endp


; print_float(float x)
print_float proc c
    arg @x
    local @@temp: byte: 6
    uses si

    lea si, @@temp

    push 100
    push si
    push @x
    call ftoa
    add sp, 6

    puts si
    ret
print_float endp


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

    push [di].m_year
    call print_int
    add sp, 2
    puts sep

    push [di].m_pages_count
    call print_int
    add sp, 2
    puts sep

    lea dx, [di].m_price
    push dx
    call print_float
    add sp, 2

    putc ENDL
    ret
print_record endp


; inr read_int() -> ax
read_int proc c
    local @@temp: byte: 5
    uses si

    lea si, @@temp
    push si
    call gets
    add sp, 2

    push si
    call atoi
    add sp, 2
    ret
read_int endp


; read_float(float *dest)
read_float proc c
    arg @dest
    local @@temp: byte: 6
    uses si

    lea si, @@temp
    push si
    call gets
    add sp, 2

    push @dest
    push si
    call atof
    add sp, 4

    ret
read_float endp


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
    call read_int
    mov [si].m_year, ax

    puts p_pages
    call read_int
    mov [si].m_pages_count, ax

    puts p_price
    lea di, [si].m_price
    push di
    call read_float
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
    uses si, di, bx, cx
    local @@temp: byte: 5

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

    lea di, @@temp
    ; get year
    push di
    push @rec
    push 4
    call get_field
    add sp, 6

    ; convert to integer
    push di
    call atoi
    add sp, 2
    mov [si].m_year, ax

    ; get pages_count
    push di
    push @rec
    push 5
    call get_field
    add sp, 6

    ; convert to integer
    push di
    call atoi
    add sp, 2
    mov [si].m_pages_count, ax

    ; get price
    push di
    push @rec
    push 6
    call get_field
    add sp, 6

    ; convert to float
    lea bx, [si].m_price
    push bx
    push di
    call atof
    add sp, 4
    ret
parse_record endp

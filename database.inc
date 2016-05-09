.data
plug db 'print all records...', ENDL, 0

; struct book
author db 20 dup('')
title db 20 dup('')
genre db 10 dup('')

pages_count dd 0
book_count dd 0
; price dd 0.0

.code
include stdio.inc

; db_show_table(FILE handle)
db_show_table proc c
    arg @handle

    puts plug

    ret
db_show_table endp

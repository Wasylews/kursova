;;; TODO: structure declaration, array of structure
; struct book {
;     char author[20];
;     char title[20];
;     char *genre[10];

;     int pages_count;
;     int book_count;

;     float price;
; }

; book *arr;


.model small
.386

.stack 256h

.data
filename db 'LICENSE', 0
handle dw ?
buffer db 20 dup('')

.code

LOCALS

include stdio.inc

main proc c
    mov ax, @data
    mov ds, ax

    puts filename

@@exit:
    _exit
main endp

end main

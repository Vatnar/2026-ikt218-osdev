[bits 32]
; global functions
global gdt_flush           ; void gdt_flush(uint16_t limit, uint32_t base, uint32_t offset);
global gdt_reload_segments ; void gtd_reload_segments(void);
section .data
gdtr DW 0 ; For limit storage
     DD 0 ; For base storage

section .text

; https://wiki.osdev.org/GDT_Tutorial
gdt_flush:
   MOV   AX, [esp + 4]
   MOV   [gdtr], AX
   MOV   EAX, [ESP + 8]
   MOV   [gdtr + 2], EAX
   LGDT  [gdtr]
   RET



; https://wiki.osdev.org/GDT_Tutorial
gdt_reload_segments:
   ; Reload CS register containing code selector:
   JMP   0x08:.reload_cs ; 0x08 is a stand-in for your code segment
.reload_cs:
   ; Reload data segment registers:
   MOV   AX, 0x10 ; 0x10 is a stand-in for your data segment
   MOV   DS, AX
   MOV   ES, AX
   MOV   FS, AX
   MOV   GS, AX
   MOV   SS, AX
   RET

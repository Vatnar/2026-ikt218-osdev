[bits 32]
; global functions
global gdt_flush           ; void gdt_flush(uint16_t limit, uint32_t base, uint32_t offset);
global gdt_reload_segments ; void gtd_reload_segments(void);

; https://wiki.osdev.org/GDT_Tutorial
gdt_flush:
   MOV EAX, [ESP + 4]
   LGDT  [EAX]
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

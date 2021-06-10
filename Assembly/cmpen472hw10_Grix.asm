;*******************************************************
;* CMPEN 472, HW10 1024 data transfer, MC9S12C128 Program
;* Nov. 4,2016 Kyusun Choi
;* Nov. 7,2016 Kyusun Choi
;* Nov. 2,2018 Kyusun Choi
;* Nov. 4,2018 Nathanel Grix
;* 
;* This program is a 1024 data transfer program as follows: 
;*   1. Program starts with print messages on HyperTerminal,
;*      it's a typewriter program at 9600 baud.
;*   2. When a user hits Enter key first time,
;*      the HCS12 board will change it baud rate to 115.2K baud.
;*      So the communication stops due to baud rate difference.   
;*   3. Next, the user quit Hyper Terminal and start Hyper Terminal 
;*      again with 115.2K baud. 
;*   4. Then user may hit any key, it's a typewriter program at 115.2K baud.
;*      But hitting the Enter key will terminate the typewriter mode and
;*      the HCS12 board enter to non-Terminal mode, waiting for SW1 press.  
;*   5. At this time, user quit Hyper Terminal again.
;*      And start SB Data Receive program.  Then type the COM port 
;*      number and hit enter key.  The SB Data Receive program will be waiting.
;*   6. User push SW1 on the HCS12 board to receive 1024 data, in to a file
;*      named RxData3.txt  which may be looked at or plotted using Excel sheet.
;*   7. User may repeat the step 6 above as many times as he/she like.
;*      User plots or prints the data to verify the correct data transmit.
;* 
;*******************************************************
; export symbols
            XDEF        Entry        ; export 'Entry' symbol
            ABSENTRY    Entry        ; for assembly entry point

; include derivative specific macros

PTIP        EQU         $0259        ; PORT P input register
DDRP        EQU         $025A        ; PORT P data direction register
PERP        EQU         $025C        ; PORT P pull device enable register
PPSP        EQU         $025D        ; PORT P polarity select register

SCIBDH      EQU         $00c8        ; Serial port (SCI) Baud Rate Register H
SCISR1      EQU         $00cc        ; Serial port (SCI) Status Register 1
SCIDRL      EQU         $00cf        ; Serial port (SCI) Data Register

ATDCTL2     EQU         $0082
ATDCTL3     EQU         $0083
ATDCTL4     EQU         $0084
ATDCTL5     EQU         $0085
ATDSTAT0    EQU         $0086
ATDDR0H     EQU         $0090
ATDDR0L     EQU         $0091
ATDDR7H     EQU         $009e
ATDDR7L     EQU         $009f

TIOS        EQU         $0040        ; Timer Input Capture (IC) or Output Compare (OC) select
TIE         EQU         $004C        ; Timer interrupt enable register
TCNTH       EQU         $0044        ; Timer free runing main counter
TSCR1       EQU         $0046        ; Timer system control 1
TSCR2       EQU         $004D        ; Timer system control 2
TFLG1       EQU         $004E        ; Timer interrupt flag 1
TC2H        EQU         $0054        ; Timer channel 2 register

CR          equ         $0d          ; carriage return, ASCII 'Return' key
LF          equ         $0a          ; line feed, ASCII 'next line' character

;*******************************************************
; variable/data section
            ORG     $3000            ; RAMStart defined as $3000
                                     ; in MC9S12C128 chip
                                     
ctr125u     DS.W    1                ; 16bit interrupt counter for 125 uSec. of time


msg1        DC.B    'Well>', $00
msg2        DC.B    'Baud rate changed! Please reopen the HyperTerminal with 115.2Kbaud', $00
msg3        DC.B    'Please connect the audio cable to HCS12 board', $00

; following escape sequence works on Hyper Terminal (but NOT the TestTerminal)
ResetTerm       DC.B    $1B, 'c', $00                 ; reset terminal to default setting
ClearScreen     DC.B    $1B, '[2J', $00               ; clear the Hyper Terminal screen

;*******************************************************
; interrupt vector section

            ORG     $3FEA            ; Timer channel 2 interrupt vector setup
            DC.W    oc2isr

;*******************************************************
; code section
            ORG     $3100
Entry
            LDS     #Entry           ; initialize the stack pointer

            LDAA    #%00000011
            STAA    PERP             ; enable the pull up/down feature at PORTP bit 0 and 1
            BCLR    PPSP,%00000011   ; select pull up feature at PORTP bit 0 and 1 for the
                                     ; Push Button Switch 1 and 2.
            BCLR    DDRP,%00000011   ; Push Button Switch 1 and 2 at PORTP bit 0 and 1
                                     ; set PORTP bit 0 and 1 as input

            ldx     #ResetTerm       ; reset Hyper Terminal to default values
            jsr     printmsg
            ldx     #ClearScreen     ; clear the Hyper Terminal Screen first
            jsr     printmsg

;            ldx     #msg1            ; print the first message, 'Hello'
;            jsr     printmsg
;            jsr     nextline
            ldx     #msg2            ; print the second message, user instruction
            jsr     printmsg
            jsr     nextline
            jsr     delay10ms        ; wait to finish sending characters
            
            LDX     #$000D             ; Change the SCI port baud rate to 115.2K
            STX     SCIBDH

mloop1      jsr     getchar
            ;cmpa    #0
            ;beq     mloop1
            ;jsr     putchar          ; type writer, with echo print
            cmpa    #CR
            bne     mloop1           ; if Enter/Return key is pressed, move the
            ldaa    #LF              ; cursor to next line
            jsr     putchar
            ldx     #msg3            ; print 'connect the audio cable'
            jsr     printmsg
            jsr     nextline
            ldx     #msg1
            jsr     printmsg
            
mloop2      jsr     getchar
            ;cmpa    #0
            ;beq     mloop2
            cmpa    #CR
            bne     mloop2           ; if Enter/Return key is pressed, move the
            jsr     putchar          ; type writer, with echo print
            ldaa    #LF              ; cursor to next line
            jsr     putchar
            
; ATD initialization
            LDAA    #%11000000       ; Turn ON ADC, clear flags, Disable ATD interrupt
            STAA    ATDCTL2
            LDAA    #%00001000       ; Single conversion per sequence, no FIFO
            STAA    ATDCTL3
            LDAA    #%01000111       ; 10bit, ADCLK=24MHz/16=1.5MHz, sampling time=8*(1/ADCLK)
            STAA    ATDCTL4
            jsr     go2ADC
            jsr     nextline
            ldx     #msg1
            jsr     printmsg

mloop3      jsr     getchar
            cmpa    #'a'
            beq     lastPrint
            cmpa    #CR
            bne     mloop3           ; if Enter/Return key is pressed, move the
            jsr     putchar          ; type writer, with echo print
            ldaa    #LF              ; cursor to next line
            jsr     putchar
            jsr     go2ADC
            ldx     #msg1
            jsr     printmsg
            bra     mloop3
;            ldx     #msg3            ; print 'Disconnect Hyper Term.'
;            jsr     printmsg
;            jsr     nextline
;            ldx     #msg4            ; print 'Start SB Data Receive program.'
;            jsr     printmsg
;            jsr     nextline
;            ldx     #msg5            ; print 'Then press the switch SW1, for 1024 point data.'
;            jsr     printmsg
;            jsr     nextline

lastPrint   jsr     putchar
            jsr     nextline
            ldx     #msg4
            jsr     printmsg
            jsr     nextline
            ldx     #msg5
            jsr     printmsg
            jsr     nextline
            ldx     #msg6
            jsr     printmsg
            jsr     nextline
            ldx     #msg7
            jsr     printmsg
            jsr     nextline
            
loopTx      LDAA    PTIP            ; read push button SW1 at PORTP0
            ANDA    #%00000001      ; check the bit 0 only
            BNE     loopTx          ; SW1 not pushed
            
            jsr     StartTimer2oc
            ldx     #0              ; SW1 pushed
            
loop1024    cpx     #1030           ; 1031 bytes will be sent, the receiver at Windows PC 
            beq     stopcheck       ;   will only take 1028 bytes.  The SB Data Receive 
            jsr     Upload          ;   to make the file RxData3.txt with exactly 1024 data 
            inx                     ;   points.  (Only the lower 8bit of X is sent {tfr x,a}.)
            bra     loop1024        ; Assume the SB Data Receive program running on Windows PC 
            
stopcheck   SEI
            bra     loopTx


;*******************************************************
; subroutine section

;***************Update Display**********************
;* Program: Update count down timer display if 1 second is up
;* Input:   ctr2p5m variable
;* Output:  timer display on the Hyper Terminal
;* Registers modified: CCR
;* Algorithm:
;    Check for 1 second passed
;      if not 1 second yet, just pass
;      if 1 second has reached, then update display, toggle LED, and reset ctr2p5m
;**********************************************
Upload
            psha
            pshx                   ; xxaspb

            LDAA  #%00000111       ; left justified, unsigned, single conversion,
            STAA  ATDCTL5          ; single channel, CHANNEL 7, start the conversion

getready    ldaa  ATDSTAT0         ; Wait until ATD conversion finish
            anda  #%10000000       ; check SCF bit, wait for ATD conversion to finish
            beq   getready

            ldaa  ATDDR0H          ; pick up the upper 8bit result

            ldx   #0               ; interrupt counter reached 8000 count, 1 sec up now
            stx   ctr125u          ; clear the interrupt count to 0, for the next 1 sec.

            jsr   putchar

            
UpDone      
            pulx
            pula
            rts
;***************end of Update Display***************

;***************StartTimer2oc************************
;* Program: Start the timer interrupt, timer channel 2 output compare
;* Input:   Constants - channel 2 output compare, 125usec at 24MHz
;* Output:  None, only the timer interrupt
;* Registers modified: D used and CCR modified
;* Algorithm:
;             initialize TIOS, TIE, TSCR1, TSCR2, TC2H, and TFLG1
;**********************************************
StartTimer2oc
            PSHD
            LDAA   #%00000100
            STAA   TIOS              ; set CH2 Output Compare
            STAA   TIE               ; set CH2 interrupt Enable
            LDAA   #%10010000        ; enable timer and set Fast Flag Clear
            STAA   TSCR1
            LDAA   #%00000000        ; TOI Off, TCRE Off, TCLK = BCLK/1
            STAA   TSCR2             ;   not needed if started from reset

            LDD     #3000            ; 125usec with (24MHz/1 clock)
            ADDD    TCNTH            ;    for first interrupt
            STD     TC2H             ;    + Fast clear timer CH2 interrupt flag

            PULD
            BSET   TFLG1,%00000100   ; initial Timer CH2 interrupt flag Clear, not needed if fast clear set
            CLI                      ; enable interrupt
            RTS
;***************end of StartTimer2oc*****************

;***********Timer OC2 interrupt service routine***************
oc2isr
            ldd   #3000              ; 125usec with (24MHz/1 clock)
            addd  TC2H               ;    for next interrupt
            std   TC2H               ;    + Fast clear timer CH2 interrupt flag
            ldx   ctr125u            ; 125uSec => 8.000KHz rate
            inx
            stx   ctr125u            ; every time the RTI occur, increase interrupt count
oc2done     RTI
;***********end of Timer OC2 interrupt service routine********

;***********single AD conversiton*********************
; This is a sample, non-interrupt, busy wait method
;
go2ADC
            PSHA                   ; Start ATD conversion
            LDAA  #%00000111       ; left justified, unsigned, single conversion,
            STAA  ATDCTL5          ; single channel, CHANNEL 7, start the conversion
                                   ; CHANNEL 5 is connected to the trim potentiometer

adcwait     ldaa  ATDSTAT0         ; Wait until ATD conversion finish
            anda  #%10000000       ; check SCF bit, wait for ATD conversion to finish
            beq   adcwait

            ldaa  #'$'             ; print the ATD result, in hex
            jsr   putchar

            ldaa  ATDDR0H          ; pick up the upper 8bit result
            jsr   printHx          ; print the ATD result
            jsr   nextline

            PULA
            RTS
;***********end of AD conversiton**************            

;***********printHx***************************
; prinHx: print the content of accumulator A in Hex on SCI port
printHx     psha
            lsra
            lsra
            lsra
            lsra
            cmpa   #$09
            bhi    alpha1
            adda   #$30
            jsr    putchar
            bra    low4bits
alpha1      adda   #$37
            jsr    putchar            
low4bits    pula
            anda   #$0f
            cmpa   #$09
            bhi    alpha2
            adda   #$30
            jsr    putchar
            rts
alpha2      adda   #$37
            jsr    putchar
            rts
;***********end of printhx***************************

;***********printmsg***************************
;* Program: Output character string to SCI port, print message
;* Input:   Register X points to ASCII characters in memory
;* Output:  message printed on the terminal connected to SCI port
;* 
;* Registers modified: CCR
;* Algorithm:
;     Pick up 1 byte from memory where X register is pointing
;     Send it out to SCI port
;     Update X register to point to the next byte
;     Repeat until the byte data $00 is encountered
;       (String is terminated with NULL=$00)
;**********************************************
NULL            equ     $00
printmsg        psha                   ;Save registers
                pshx
printmsgloop    ldaa    1,X+           ;pick up an ASCII character from string
                                       ;   pointed by X register
                                       ;then update the X register to point to
                                       ;   the next byte
                cmpa    #NULL
                beq     printmsgdone   ;end of strint yet?
                bsr     putchar        ;if not, print character and do next
                bra     printmsgloop
printmsgdone    pulx 
                pula
                rts
;***********end of printmsg********************

;***************putchar************************
;* Program: Send one character to SCI port, terminal
;* Input:   Accumulator A contains an ASCII character, 8bit
;* Output:  Send one character to SCI port, terminal
;* Registers modified: CCR
;* Algorithm:
;    Wait for transmit buffer become empty
;      Transmit buffer empty is indicated by TDRE bit
;      TDRE = 1 : empty - Transmit Data Register Empty, ready to transmit
;      TDRE = 0 : not empty, transmission in progress
;**********************************************
putchar     brclr SCISR1,#%10000000,putchar   ; wait for transmit buffer empty
            staa  SCIDRL                      ; send a character
            rts
;***************end of putchar*****************

;****************getchar***********************
;* Program: Input one character from SCI port (terminal/keyboard)
;*             if a character is received, other wise return NULL
;* Input:   none    
;* Output:  Accumulator A containing the received ASCII character
;*          if a character is received.
;*          Otherwise Accumulator A will contain a NULL character, $00.
;* Registers modified: CCR
;* Algorithm:
;    Check for receive buffer become full
;      Receive buffer full is indicated by RDRF bit
;      RDRF = 1 : full - Receive Data Register Full, 1 byte received
;      RDRF = 0 : not full, 0 byte received
;**********************************************

getchar     brclr SCISR1,#%00100000,getchar7
            ldaa  SCIDRL
            rts
getchar7    clra
            rts
;****************end of getchar**************** 

;****************nextline**********************
nextline    ldaa  #CR              ; move the cursor to beginning of the line
            jsr   putchar          ;   Cariage Return/Enter key
            ldaa  #LF              ; move the cursor to next line, Line Feed
            jsr   putchar
            rts
;****************end of nextline***************

;****************delay10ms**********************
delay10ms:  pshx
            ldx   #$FFFF           ; count down X, $FFFF may be more than 10ms 
d10msloop   nop                    ;   X <= X - 1
            dex                    ; simple loop
            bne   d10msloop
            pulx
            rts
;****************end of delay10ms***************

msg4        DC.B    'Please disconnect the HyperTermina', $00
msg5        DC.B    'Start NCH Tone Generator program', $00
msg6        DC.B    'Start SB Data Receive program', $00
msg7        DC.B    'Then press the switch SW1, for 1024 point analog to digital conversions', $00


            END                    ; this is end of assembly source file
                                   ; lines below are ignored - not assembled
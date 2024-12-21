#!/usr/bin/env bash

### Turn input into x1 x2 y1 y2 tx ty
lctr=0
while read button val x y; do
    lctr=$(( $lctr + 1 ))
    if ! [[ "${button}" ]]; then
        x1=""
        x2=""
        y1=""
        y2=""
        tx=""
        ty=""
    
    elif [[ "${button}" == 'Prize:' ]]; then
        tx="${val//X=/}"
        tx="${tx//,/}"
        ty="${x//Y=/}"
        echo $x1 $x2 $y1 $y2 $tx $ty
    elif [[ "${val}" == "A:" ]]; then
        x1="${x//X\+/}"
        x1="${x1//,/}"
        y1="${y//Y\+/}"
    elif [[ "${val}" == "B:" ]]; then
        x2="${x//X\+/}"
        x2="${x2//,/}"
        y2="${y//Y\+/}"
    fi
    


done < input.txt
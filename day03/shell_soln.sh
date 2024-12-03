#!/usr/bin/env bash
part1() {
    sum=0
    while read tok; do
        t=${tok//mul\(/}
        left=${t%,*}
        t=${t//)/}
        right=${t#*,}
        sum=$(( $sum + $left * $right ))
    done < <( grep -o -e "mul([0-9]*,[0-9]*)" input.txt )
    echo $sum
}

part2() {
    sum=0
    go=1
    while read tok; do
        if [[ "${tok}" == "do()" ]]; then
            go=1
            continue
        fi
        if [[ "${tok}" == "don't()" ]]; then
            go=0
        fi
        if [[ "${go}" == 1 ]]; then
            t=${tok//mul\(/}
            left=${t%,*}
            t=${t//)/}
            right=${t#*,}
            sum=$(( $sum + $left * $right ))
        fi
    done < <( grep -o -e "mul([0-9]*,[0-9]*)"  -e 'do()' -e "don't()" input.txt )
    echo $sum

}

part1
part2